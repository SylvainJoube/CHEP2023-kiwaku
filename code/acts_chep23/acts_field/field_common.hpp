#pragma once

#include "data_structures_common.hpp"

template <typename Data_T>
struct os_view_t
{
public:

  explicit os_view_t(Data_T* d) : data(d) {}

  pt3D<float> at(float p0, float p1, float p2) // const std::array<float, 3>& pos
  {
    return data->at(p0, p1, p2);
  }
  
private:
  Data_T* data;
};

template <typename Data_T>
struct os_table_t
{
public:
  // Constructor implicitly constructs the fiels
  os_table_t() {}

  inline pt3D<float> at(float p0, float p1, float p2) // const std::array<float, 3>& pos
  {
    return data.at(p0, p1, p2);
  }
  
  inline pt3D<float> at_strided(unsigned long p0, unsigned long p1, unsigned long p2) // const std::array<float, 3>& pos
  {
    return data.at_strided(p0, p1, p2);
  }

private:
  Data_T data;
};