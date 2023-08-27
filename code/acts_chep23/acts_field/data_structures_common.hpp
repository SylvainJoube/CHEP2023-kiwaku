#pragma once

// #include "utils.hpp"
#include <vector>
#include <iostream>
// #include <array>
#include "../../common/utils/printer.hpp"

namespace kwk
{

  #define SCALAR_TYPE float

  template <typename T = SCALAR_TYPE>
  struct matrix
  {
    std::size_t d0, d1;
    std::vector<T> elems;

    inline void init(std::size_t d0_, std::size_t d1_)
    {
      d0 = d0_;
      d1 = d1_;
      elems.resize(d0 * d1);
      // std::cout << "kwk::matrix init*: d0_" << d0_ << "  d1_" << d1_ << "\n";
    }

    inline void init(std::size_t d0_, std::size_t d1_, T* array)
    {
      init(d0_, d1_);
      for (uint i = 0; i < d0*d1; ++i) elems[i] = array[i];
    }

    inline explicit matrix(std::size_t d0_, std::size_t d1_)           { init(d0_, d1_); }
    inline explicit matrix(std::size_t d0_, std::size_t d1_, T* array) { init(d0_, d1_, array); }

    
    // If default constructor is used,
    // use init(std::size_t d0_, std::size_t d1_, T* array)
    inline explicit matrix() = default;
    
    inline T &operator()(std::size_t x, std::size_t y)
    {
      // return elems.at(x + y * d0);
      return elems[x* d1 + y];
    }

    inline T operator()(std::size_t x, std::size_t y) const
    {
      return elems[x* d1 + y];
    }

    inline T & at(std::size_t x, std::size_t y)
    {
      // return elems[x]* d1 + y);
      return elems[x* d1 + y];
    }

    inline T at(std::size_t x, std::size_t y) const
    {
      return elems[x* d1 + y];
    }

    inline void print(/*std::ostream os,*/ uint pad_length = 20)
    {
      for (uint i = 0; i < d0; ++i)
      {
        for (uint j = 0; j < d1; ++j)
        {
          std::cout << print::pad_right(std::to_string(this->operator()(i, j)), pad_length) << " ";
        }
        std::cout << "\n";
      }
    }

    template <typename U>
    inline std::vector<U> operator*(const std::vector<U> &v) const
    {
      // Créer un objet kwk::matrix fait elems.resize(d0 * d1);
      // avec std::vector<float> elems;
      auto N = d0;
      kwk::matrix<U> r(N + 1, 1);


      for (uint i = 0; i < N; ++i) {
        r.at(i, 0) = v[i];
      }

      r.at(N, 0) = 1.;

      auto res = operator*(r);

      std::vector<U> vres;
      vres.resize(N + 1);
      for (uint i = 0; i < N; ++i) {
        vres[i] = res(i, 0);
      }

      // v.size = 3 ; vres.size = 4
      // std::cout << "kwk::matrix operator*: v.size" << v.size() << "  vres.size()" << vres.size() << "\n";

      return vres;
    }

    // template <std::size_t P>matrix<N, P, T, I>
    //  M P T I
    //  returns a N * P matrix
    //  this (N*M) * other (M*P)
    //  (d0*d1) * (d1*d2)
    template <typename U>
    inline kwk::matrix<U> operator*(const kwk::matrix<U> &o) const
    {
      // matrix<N, P, T, I> r;
      std::size_t N = d0;
      std::size_t M = d1;
      std::size_t P = o.d1;
      if (d1 != o.d0)
        std::cout << ">>>>>>>>>> ERROR d1 != o.d0\n";

      kwk::matrix<U> r(N, P);

      for (uint i = 0; i < N; ++i)
      {
        for (uint j = 0; j < P; ++j)
        {
          T t = 0.;

          for (uint k = 0; k < M; ++k)
          {
            t += this->operator()(i, k) * o.operator()(k, j);
          }
          r(i, j) = t;
        }
      }
      // std::cout << "r size (" << N << ", " << P << ")\n";
      return r;
    }
  };

  template <std::size_t d0, std::size_t d1>
  struct matrix_optimized
  {
    using T = SCALAR_TYPE;

    std::array<T, d0 * d1> elems;

    inline void fill(T* array)
    {
      for (uint i = 0; i < d0*d1; ++i) elems[i] = array[i];
    }

    // If default constructor is used, use fill(T* array)
    inline explicit matrix_optimized() = default;
    inline explicit matrix_optimized(T* array) { fill(array); }
    
    inline T& operator()(std::size_t x, std::size_t y)       { return elems[x * d1 + y]; } // elems.at(x + y * d0);
    inline T  operator()(std::size_t x, std::size_t y) const { return elems[x * d1 + y]; }
    inline T& at(std::size_t x, std::size_t y)       { return operator()(x, y); }
    inline T  at(std::size_t x, std::size_t y) const { return operator()(x, y); }

    inline void print(/*std::ostream os,*/ uint pad_length = 20)
    {
      for (uint i = 0; i < d0; ++i) {
        for (uint j = 0; j < d1; ++j) {
          std::cout << print::pad_right(std::to_string(this->operator()(i, j)), pad_length) << " ";
        }
        std::cout << "\n";
      }
    }

    // template <typename U>
    // matrix * vector
    inline std::array<float, d0 + 1> operator*(const std::array<float, d0> &v) const
    {
      // auto N = d0;
      kwk::matrix_optimized<d0 + 1, 1> r; // 4 * 1

      for (uint i = 0; i < d0; ++i) {
        r.at(i, 0) = v[i];
      }

      r.at(d0, 0) = 1.; // index 3: 0 (4th pos)

      auto res = operator*(r);

      std::array<float, d0 + 1> vres; // size = 4

      for (uint i = 0; i < d0; ++i) {
        vres[i] = res(i, 0);
      }

      return vres;
    }

    // template <std::size_t P>matrix<N, P, T, I>
    //  M P T I
    //  returns a N * P matrix
    //  this (N*M) * other (M*P)
    //  (d0*d1) * (d1*d2)
    // Matrix * matrix
    template <std::size_t SC>
    inline kwk::matrix_optimized<d0, SC> operator*(const kwk::matrix_optimized<d1, SC> &o) const
    {
      // matrix<N, P, T, I> r;
      // std::size_t N = d0;
      // std::size_t M = d1;
      // std::size_t P = SC; //o.d1; 
      // if constexpr(d1 != o.d0) It will not compile if (o.d0 != d1), since o has to be of size <d1, SC>.
      //   std::cout << ">>>>>>>>>> ERROR d1 != o.d0\n";

      kwk::matrix_optimized<d0, SC> r;

      for (std::size_t i = 0; i < d0; ++i) {
        for (std::size_t j = 0; j < SC; ++j) {
          float t = 0.;
          for (std::size_t k = 0; k < d1; ++k) {
            t += this->operator()(i, k) * o.operator()(k, j);
          }
          r(i, j) = t;
        }
      }
      // std::cout << "r size (" << N << ", " << P << ")\n";
      return r;
    }
  };


}


template<typename T>
struct  pt3D
{
  std::array<T, 3> data;
  inline T& operator[](std::size_t i) { return data[i]; }
  inline T  operator[](std::size_t i) const { return data[i]; }
  inline auto begin()       { return data.begin(); }
  inline auto begin() const { return data.begin(); }
  inline auto end()         { return data.end(); }
  inline auto end() const   { return data.end(); }
};

template<typename T>
inline constexpr auto operator*(pt3D<T> a, T b) noexcept
{
  for (auto &e : a) e *= b;
  return a;
}

template<typename T>
inline constexpr auto operator+(pt3D<T> a, pt3D<T> b) noexcept
{
  for(int i=0;i<3;i++) a[i] += b[i];
  return a;
}

template<typename T>
inline constexpr auto operator*(T b, pt3D<T> a) noexcept
{
  return a * b;
}

template<typename T>
inline std::ostream& operator<<(std::ostream &s, const pt3D<T> &p)
{
  return s << "(" << p[0] << "," << p[1] << "," << p[2] << ")";
}