
#pragma once
#include <iostream>
#include <filesystem>
#include <vector>
#include "../../common/utils/utils.hpp"
#include "../../common/lorentz-euler/lorentz-euler_utlils.hpp"
// #include "../acts_field/standalone/field_standalone.hpp"
#include "../global_variables.hpp"



namespace lorentz
{

std::vector<std::vector<std::uint64_t>> get_parameter_ranges()
{
  // Reduced set of parameters
  return
  {
      {512},
      {8192}, // , 2048, 4096, 8192, 16384, 32768, 65536
      {1800} // ,   192,  6144, 65536, 131072
  };
}


struct parameters_t {
    std::size_t particles, steps, imom;
};


template<typename FIELD_T>
void bench_with_field(std::string arch_name, std::string field_name, void(*bench_iteration)(std::size_t, std::size_t, std::size_t, std::string))
{
  auto params = get_parameter_ranges();

  std::string params_str = "";
  for (std::size_t i = 0; i < params.size(); ++i) {
    const std::vector<std::uint64_t>& param = params[i];
    if (param.size() == 1) {
      params_str += "_" + std::to_string(param[0]);
    } else {
      params_str += "_" + std::to_string(param[0]) + "-" + std::to_string(param[param.size()-1]);
    }
  }

  std::string prefix;

  

  #if SMEM_ENABLE_LOG
    prefix = "with_mtest_lorentz";
  #else
    prefix = "lorentz";
  #endif

  #if USE_OPTIMIZED_MATRIX
    prefix = prefix + "_opti";
  #else
    prefix = prefix + "";
  #endif

  #if USE_NANOBENCH
    prefix = prefix + "_nanob";
  #endif
  
  #if USE_SYSCLOCK
    prefix = prefix + "_sclock";
  #endif

  prefix += "_it" +std::to_string(lorentz_iteration_result_t::iteration_base);

  std::string output_fname = "IMG_ONLY_" + prefix + "_" + arch_name + "_" + get_computer_name() + params_str + "_" + field_name + ".txt";
  std::string output_img_fname = "IMG_" + prefix + "_" + arch_name + "_" + get_computer_name() + params_str + "_" + field_name + ".bmp";

  print::line("Will output to file:  " + output_fname);
  print::line("In directory:         " + std::filesystem::current_path().string());
  print::line("");
  lorentz_iteration_result_t::print_header();

  write_f.open(output_fname);
  write_f << std::to_string(lorentz_iteration_result_t::version) << "\n"; // file version

  #if SMEM_ENABLE_LOG
    std::string mem_test_fname = "MEM_TEST_lorentz_" + arch_name + "_" + get_computer_name() + params_str + "_" + field_name + ".txt";
    mem_test_write_f.open(mem_test_fname);
  #endif

  uint count = 0;
  for (uint a = 0; a < params[0].size(); ++a) {
    for (uint b = 0; b < params[1].size(); ++b) {
      for (uint c = 0; c < params[2].size(); ++c) {
        //             (particles_n,  steps_n,      imom_n);
        bench_iteration(params[0][a], params[1][b], params[2][c], output_img_fname);
        count++;
      }
    }
  }
  print::line("count = " + std::to_string(count));

  write_f.close();

  #if SMEM_ENABLE_LOG
    mem_test_write_f.close();
  #endif
}



} // namespace lorentz


