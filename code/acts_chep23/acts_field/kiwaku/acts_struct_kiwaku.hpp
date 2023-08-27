#pragma once

#include <iostream>
#include <cstddef>
#include <fstream>
#include <vector>
#include <memory>
#include <cmath>
#include "../data_structures_common.hpp"
// #include "data_structures_kiwaku.hpp"
#include <kwk/kwk.hpp>
#include "../../global_variables.hpp"

struct acts_data_t
{
  #define ACTS_DATA_DEFAULT_VALUE pt3D<float>({5000, 6000, 7000})
  #define ACTS_DATA_USE_CLASSIC_IF false

  using data_t = kwk::make_table_t<kwk::as<pt3D<float>>, kwk::_3D>;

  bool should_display(bool modify = false);
  void read_acts_file();
  // Kiwaku noinline: DEEP not supported.
  inline pt3D<float> at(const pt3D<float> &p)
  {
    #if USE_OPTIMIZED_MATRIX
      return at_affine2(p);
    #else
      return at_affine(p);
    #endif
  }

  pt3D<float> at_affine(const pt3D<float> &p);
  pt3D<float> at_affine2(const pt3D<float> &p);
  pt3D<float> at_linear(const pt3D<float> &p);

  inline pt3D<float> inline_at(const pt3D<float> &p)
  {
    #if USE_OPTIMIZED_MATRIX
      return inline_at_affine2(p);
    #else
      return inline_at_affine(p);
    #endif
  }

  inline pt3D<float> inline_at_affine(const pt3D<float> &p);
  inline pt3D<float> inline_at_affine2(const pt3D<float> &p);
  inline pt3D<float> inline_at_linear(const pt3D<float> &p);

  inline void init_affine_matrix();
  void display_pos(int x, int y, int z);

  static const std::size_t dims = 3;
  SCALAR_TYPE matrix[dims * (dims + 1)];
  std::size_t dimensions[dims];
  std::size_t read_numel;

  kwk::matrix<float> m_transform_global; // Calculated by read_acts_file
  kwk::matrix_optimized<dims, dims + 1> m_transform_optimized;
  
  data_t data;
  uint S_DISPLAY_COUNT_AFFINE = 0;
  uint S_DISPLAY_COUNT_MAX = 0;

  const bool ENABLE_DISPLAY = false;

};


inline pt3D<float> acts_data_t::inline_at_affine(const pt3D<float> & p)
{

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

  std::vector<float> kv(p.begin(),p.end());
  
  auto kres = m_transform_global * kv;

  #if ENABLE_DISPLAY
  if (display) {
    for (uint i = 0; i < sd0; ++i) {
      std::cout << "kk(" << i << ") = " << kres.at(i) << "\n";
    }
  }
  #endif

  // ==== LINEAR ====
  return acts_data_t::inline_at_linear(pt3D<float>{ kres[0], kres[1], kres[2]} );
}



inline pt3D<float> acts_data_t::inline_at_affine2(const pt3D<float> & p)
{
  // ==== AFFINE ====
  std::array<float, 3> kv{p[0], p[1], p[2]};
  auto kres = m_transform_optimized * kv; // m_transform

  // ==== LINEAR ====
  return acts_data_t::inline_at_linear(pt3D<float>{ kres[0], kres[1], kres[2]} );
}

inline pt3D<float> acts_data_t::inline_at_linear(const pt3D<float> &p)
{
  return kwk::linear.interpolate(data, p[0], p[1], p[2]);
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
  m_transform_optimized.fill(matrix);

  #if ENABLE_DISPLAY
  if (display) m_transform_global.print();
  #endif
}