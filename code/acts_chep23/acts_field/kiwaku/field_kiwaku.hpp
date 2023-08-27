#pragma once

#include <array>
#include "acts_struct_kiwaku.hpp"
#include "../data_structures_common.hpp"

// Generic views and tables are now common


struct os_constant_field
{
public:
  os_constant_field(pt3D<float>&& v)
  {
    value = std::move(v);
  }

  os_constant_field()
  {
    value = pt3D<float>{0.f, 0.f, 2.f};
  }

  pt3D<float> at(float, float, float) // const std::array<float, 3>&
  {
    return value;
  }
  
private:
  pt3D<float> value;
};

// The magnetic field map of the ATLAS detector
struct os_acts_field
{
public:
  os_acts_field()
  {
    data.read_acts_file();
  }

  pt3D<float> at(float p0, float p1, float p2)
  {
    return data.inline_at({p0, p1, p2});
  }
  
private:
  acts_data_t data;
};