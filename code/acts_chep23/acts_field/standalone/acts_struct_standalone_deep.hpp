#pragma once

#include <iostream>
#include <cstddef>
#include <fstream>
#include <vector>
#include <array>
#include <memory>
#include <cmath>
#include "../data_structures_common.hpp"
// #include "data_structures_standalone.hpp"
#include "../../../common/utils/printer.hpp"

struct acts_data_t
{
  #define ACTS_DATA_DEFAULT_VALUE pt3D<float>({5000, 6000, 7000})
  #define ACTS_DATA_USE_CLASSIC_IF false

  inline bool should_display(bool modify = false);
  inline void read_acts_file();

  template<std::size_t DEEP>
  inline pt3D<float> at(float p0, float p1, float p2) { return at_affine<DEEP>(p0, p1, p2); }
  template<std::size_t DEEP>
  inline pt3D<float> at_affine(float p0, float p1, float p2);
  template<std::size_t DEEP>
  inline pt3D<float> at_linear(float p0, float p1, float p2);
  template<std::size_t DEEP>
  inline pt3D<float> at_strided(const pt3D<std::size_t>& c);
  template<std::size_t DEEP>
  inline pt3D<float> at_array(std::size_t c); //  { return data[c]; }

  // 0 -> ACTS_DATA_DEFAULT_VALUE
  // 1 -> affine
  // 2 -> affine + linear
  // 3 -> affine + linear + strided
  // 4 -> affine + linear + strided + array
  std::size_t requested_depth = 10;

  // acts_data_t()
  // {
  //   #if ACTS_DATA_USE_CLASSIC_IF
  //     print::str("   acts_data_t with classical if   ");
  //   #else
  //     print::str("   acts_data_t with template constexpr if   ");
  //   #endif
  // }

  inline void init_affine_matrix();
  inline void init_strides();
  inline std::size_t strided_to_linear(const pt3D<std::size_t>& c);

  static const std::size_t dims = 3;
  SCALAR_TYPE matrix[dims * (dims + 1)];
  std::size_t stride_sizes[dims];
  
  std::size_t strides[dims-1]; // dim2*dim1, dim2

  std::size_t read_numel;
  std::unique_ptr<pt3D<float>[]> data;
  uint S_DISPLAY_COUNT_AFFINE = 0;
  uint S_DISPLAY_COUNT_MAX = 0;
  kwk::matrix<float> m_transform_global;

  const bool ENABLE_DISPLAY = false;

  pt3D<float> max_value, min_value;
  inline void print_extremums();

};

#define ENABLE_DISPLAY false

bool acts_data_t::should_display(bool modify)
{
  // if constexpr (ENABLE_DISPLAY) {}
  if (modify) ++S_DISPLAY_COUNT_AFFINE;
  if (S_DISPLAY_COUNT_AFFINE <= S_DISPLAY_COUNT_MAX) return true;
  else                                               return false;
}

void acts_data_t::read_acts_file()
{
  const char* env_p = std::getenv("COVFIE_BENCHMARK_FIELD");
  if(!env_p) {
    print::err("Please set the COVFIE_BENCHMARK_FIELD environment variable to the path of the atlas.cvf file. (containing the ACTS magnetic field rendered by covfie)");
    std::exit(EXIT_FAILURE);
  }
  
  std::string fname(env_p); // "/home/data_sync/academique/These/covfie_sylvain/atlas.cvf";
  std::ifstream ifs(fname, std::ifstream::binary);

  if (!ifs.good()) {
    std::cout << "Failed to open input file " << fname << "!";
    std::exit(1);
  }

  ifs.read(reinterpret_cast<char *>(&matrix), sizeof(matrix));
  ifs.read(reinterpret_cast<char *>(&stride_sizes), sizeof(stride_sizes));
  ifs.read(reinterpret_cast<char *>(&read_numel), sizeof(read_numel));
  data = std::make_unique<pt3D<float>[]>(read_numel);
  ifs.read(reinterpret_cast<char *>(data.get()), read_numel * sizeof(pt3D<float>));
  ifs.close();

  init_strides();
  init_affine_matrix();

  #if ENABLE_DISPLAY
  constexpr auto n = dims;
  constexpr auto m = dims + 1;
  // float arr[dims][dims+1];
  std::cout << "Affine matrix:\n";
  for (std::size_t i = 0; i < n; ++i) {
    for (std::size_t j = 0; j < m; ++j) {
      auto e = matrix[i * m + j];
      std::cout << print::pad_right(std::to_string(e), 20) << "";
    }
    std::cout << "\n";
  }
  std::cout << "\n";
  std::cout << "\n";

  kwk::matrix<SCALAR_TYPE> kmt(n, m, matrix);
  kmt.print();

  std::cout << "Dimension sizes:  ";
  for (std::size_t i = 0; i < dims; ++i) {
    std::cout << stride_sizes[i] << " ";
  }
  std::cout << "\n";
  #endif

  std::size_t excepted_numel = 0;
  for (std::size_t i = 0; i < dims; ++i) {
    if (i == 0) excepted_numel  = stride_sizes[i];
    else        excepted_numel *= stride_sizes[i];
  }

  if (excepted_numel != read_numel) {
    std::cout << "ERROR : excepted numel(" << excepted_numel << ") does not match read numel(" << read_numel << ")\n";
  }
  #if ENABLE_DISPLAY
  else {
    std::cout << "OK, matching expected and read numel: " << read_numel << " \n";
  }
  #endif

  // kwk::matrix<SCALAR_TYPE> mt(2, 3);
  // mt(0, 0) = 4;
  // std::cout << mt(0, 0) << "\n";
  // mt.print();
}


// inline pt3D<float> acts_data_t::at(const pt3D<float> &p)
// {
//   // for (uint i = 0; i < 3; ++i) if (p[i] > max_value[i]) max_value[i] = p[i];
//   // for (uint i = 0; i < 3; ++i) if (p[i] < min_value[i]) min_value[i] = p[i];

//   return at_affine(p);
// }


template<std::size_t DEEP>
inline pt3D<float> acts_data_t::at_affine(float p0, float p1, float p2)
{
  // 0 means 'return default value'
  // depth is 1 here, continue only if needed
  #if ACTS_DATA_USE_CLASSIC_IF
    if (requested_depth < 1) return ACTS_DATA_DEFAULT_VALUE;
  #else
    if constexpr (DEEP < 1)  return ACTS_DATA_DEFAULT_VALUE;
  #endif
  
  // ==== AFFINE ====
  #if ENABLE_DISPLAY
  bool display = should_display(true);
  #endif

  // std::size_t sd0 = dims;
  // std::size_t sd1 = dims + 1;
  // kwk::matrix<float> m_transform(sd0, sd1, matrix);
  // (2023-04-16) use m_transform_global for better performance
  // (it's useless to allocate it all over again)


  #if ENABLE_DISPLAY
  if (display) m_transform.print();
  #endif

  std::vector<float> kv{p0, p1, p2};
  
  auto kres = m_transform_global * kv; // m_transform

  #if ENABLE_DISPLAY
  if (display) {
    for (uint i = 0; i < sd0; ++i) {
      std::cout << "kk(" << i << ") = " << kres.at(i) << "\n";
    }
  }
  #endif

  // ==== LINEAR ====
  // auto val = m_backend.at(nc); // linear backend
  return acts_data_t::at_linear<DEEP>(kres[0], kres[1], kres[2]);
}

template<std::size_t DEEP>
inline pt3D<float> acts_data_t::at_linear(float p0, float p1, float p2)
{
  // depth is 2 here, continue only if needed
  // if (requested_depth < 2) return ACTS_DATA_DEFAULT_VALUE;
  #if ACTS_DATA_USE_CLASSIC_IF
    if (requested_depth < 2) return ACTS_DATA_DEFAULT_VALUE;
  #else
    if constexpr (DEEP < 2)  return ACTS_DATA_DEFAULT_VALUE;
  #endif

  std::size_t i = static_cast<std::size_t>(p0);
  std::size_t j = static_cast<std::size_t>(p1);
  std::size_t k = static_cast<std::size_t>(p2);

  float a = p0 - i;
  float b = p1 - j;
  float c = p2 - k;

  float ra = 1.f - a;
  float rb = 1.f - b;
  float rc = 1.f - c;

  #if ENABLE_DISPLAY
  bool display = should_display();
  #endif

  pt3D<float> pc[8];
  // std::array<std::array<float, 3>, 8>> pc;

  #if ENABLE_DISPLAY
  if (display) std::cout << "Linear: ";
  #endif

  for (std::size_t n = 0; n < 8; ++n) {

    std::size_t pp0 = i + ((n & 4) ? 1 : 0);
    std::size_t pp1 = j + ((n & 2) ? 1 : 0);
    std::size_t pp2 = k + ((n & 1) ? 1 : 0);
    pt3D<std::size_t> pp = {pp0, pp1, pp2};

    #if ENABLE_DISPLAY
    if (display) std::cout << pp << " ";
    #endif

    pc[n] = at_strided<DEEP>(pp);
  }

  #if ENABLE_DISPLAY
  if (display) std::cout << "  :  = ";
  #endif

  pt3D<float> rv; // 3D
  for (std::size_t q = 0; q < 3; ++q) // 3D
  {
    rv[q] = ra * rb * rc * pc[0][q] + ra * rb * c * pc[1][q] +
            ra * b * rc * pc[2][q] + ra * b * c * pc[3][q] +
            a * rb * rc * pc[4][q] + a * rb * c * pc[5][q] +
            a * b * rc * pc[6][q] + a * b * c * pc[7][q];
  }
  
  #if ENABLE_DISPLAY
  if (display) std::cout << "(" << rv[0] << "," << rv[1] << "," << rv[2] << ")\n";
  #endif

  return rv;
}

template<std::size_t DEEP>
inline pt3D<float> acts_data_t::at_strided(const pt3D<std::size_t>& c)
{
  // depth is 3 here, continue only if needed
  // if (requested_depth < 3) return ACTS_DATA_DEFAULT_VALUE;
  #if ACTS_DATA_USE_CLASSIC_IF
    if (requested_depth < 3) return ACTS_DATA_DEFAULT_VALUE;
  #else
    if constexpr (DEEP < 3)  return ACTS_DATA_DEFAULT_VALUE;
  #endif

  #if ENABLE_DISPLAY
  bool display = should_display();
  #endif

  // TODO : compute strides in advance
  // float idx = 0;
  // for (std::size_t k = 0; k < dims; ++k) {
  //   float tmp = c[k];
  //   for (std::size_t l = k + 1; l < dims; ++l) {
  //     tmp *= stride_sizes[l];
  //   }
  //   idx += tmp;
  // }

  #if ENABLE_DISPLAY
  if (display) std::cout << "Strided access at " << idx << "\n";
  #endif

  auto value = at_array<DEEP>(strided_to_linear(c)); // idx

  #if ENABLE_DISPLAY
  if (display) std::cout << "Value = " << value << "\n";
  #endif

  return value;
}

template<std::size_t DEEP>
inline pt3D<float> acts_data_t::at_array(std::size_t c)
{ 
  // depth is 4 here, continue only if needed
  // if (requested_depth < 4) return ACTS_DATA_DEFAULT_VALUE;
  #if ACTS_DATA_USE_CLASSIC_IF
    if (requested_depth < 4) return ACTS_DATA_DEFAULT_VALUE;
  #else
    if constexpr (DEEP < 4)  return ACTS_DATA_DEFAULT_VALUE;
  #endif

  return data[c];
}



inline void acts_data_t::init_affine_matrix()
{
  // ==== AFFINE ====
  #if ENABLE_DISPLAY
  bool display = should_display(true);
  #endif

  std::size_t sd0 = dims;
  std::size_t sd1 = dims + 1;
  m_transform_global.init(sd0, sd1, matrix);

  #if ENABLE_DISPLAY
  if (display) m_transform_global.print();
  #endif
}

inline std::size_t acts_data_t::strided_to_linear(const pt3D<std::size_t>& c)
{
  return c[0] * strides[0] + c[1] * strides[1] + c[2];
}

inline void acts_data_t::init_strides()
{
  strides[0] = stride_sizes[2] * stride_sizes[1];
  strides[1] = stride_sizes[2];

/*

idx = 0



k = 1
  tmp = c[1]
  l = k+1 = 2
    tmp *= stride_sizes[2]
  l = 3 quit loop
  idx += tmp  i.e. idx += stride_sizes[2] * c[1]

k = 2


*/

    // strides[k] = strides[k-1] * stride_sizes[k];

  // TODO : compute strides in advance
  // float idx = 0;
  // for (std::size_t k = 0; k < dims; ++k) {
  //   float tmp = c[k];
  //   for (std::size_t l = k + 1; l < dims; ++l) {
  //     tmp *= stride_sizes[l];
  //   }
  //   idx += tmp;
  // }
/*
idx = 0
k = 0
  tmp = c[0]
  for l = 1; l < 3
  l = 1
    tmp *= stride_sizes[1]
  l = 2
    tmp *= stride_sizes[2]
  idx += tmp

k = 1
  tmp = c[1]
  for l = 2; l < 3
  l = 2
    tmp *= stride_sizes[2]
  idx += tmp

k = 2
  tmp = c[2]
  for l = 3; l < 3
  idx += tmp

So:
idx = c[0] * stride_sizes[1] * stride_sizes[2] + c[1] * stride_sizes[2] + c[3]


What I think will be the result :
idx += c[0] * stride[0] + c[1] * stride[1] * stride[0] + c[2] * stride[2] * stride[1] * stride[0]

*/


}

pt3D<float> max_value, min_value;
inline void acts_data_t::print_extremums()
{
  std::cout << "=== FIELD EXTREMUMS ===\n";
  std::cout << "Max value: (" << max_value[0] << "," << max_value[1] << "," << max_value[2] << ")\n";
  std::cout << "Min value: (" << min_value[0] << "," << min_value[1] << "," << min_value[2] << ")\n";
}