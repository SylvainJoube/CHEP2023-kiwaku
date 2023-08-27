#include <iostream>
#include <filesystem>
#include "../../../common/utils/utils.hpp"
#include "../../../common/lorentz-euler/lorentz-euler_utlils.hpp"
#include "../lorentz_bench_common.hpp"
#include "../lorentz_common.hpp"
#include "../../acts_field/standalone/field_standalone.hpp"

int main([[maybe_unused]] int argc, [[maybe_unused]] char ** argv)
{
  print::line("");
  print::head("STANDALONE Lorentz-Euler STANDALONE");

  lorentz::bench_with_field<os_acts_field>("standalone", "acts-field", lorentz_bench_t::execute_custom<os_acts_field, 4>); // , &acts_field
  // lorentz::bench_with_field<os_constant_field>("standalone", "constant-field", lorentz_bench_t::execute_custom<os_constant_field, 20>); // , &constant_field

  return 0;
}
