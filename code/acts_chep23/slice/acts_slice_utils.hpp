#pragma once

#include <iostream>
#include <string>
#include <filesystem>
#include "../../common/utils/printer.hpp"
#include "../../common/utils/host_name.hpp"
#include "../../common/utils/global_file.hpp"

struct acts_slice_benchmark_t
{
  static inline void run_benchmark(std::string base_log_path, void(*fct)(float))
  {
    float offset      = 15000;
    std::size_t step  = 1000;

    float istart = offset;
    float istop  = 30000 + offset;
    // float istart = offset + 15000;
    // float istop  = istart + 1;

    std::string output_fname = base_log_path + "_" + get_computer_name() + "_step" + std::to_string(step) + ".txt";

    print::line("Will output to file:  " + output_fname);
    print::line("In directory:         " + std::filesystem::current_path().string());
    print::line("");

    write_f.open(output_fname);
    write_f << "1\n";

    for (float i = istart; i < istop; i += step) {
      fct(i);
    }
    write_f.close();
  }
};