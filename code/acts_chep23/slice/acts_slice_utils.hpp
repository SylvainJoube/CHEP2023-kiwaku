#pragma once

#include <iostream>
#include <string>
#include <filesystem>
#include "../../common/utils/printer.hpp"
#include "../../common/utils/host_name.hpp"
#include "../../common/utils/global_file.hpp"
#include "../../common/program_args.hpp"

namespace acts_slice
{
  struct benchmark_t
  {
    static inline void run_benchmark(std::string base_log_path, void(*fct)(float))
    {
      float offset      = 15000;
      std::size_t step  = 1000;

      float istart = offset;
      float istop  = 30000 + offset;
      // float istart = offset + 15000;
      // float istop  = istart + 1;

      std::string output_fname = base_log_path + "_" + program_args::computer_name_str + "_step" + std::to_string(step) + ".bench.txt";

      print::line("Will output to file:  " + output_fname);
      print::line("In directory:         " + std::filesystem::current_path().string() + "/output_bench");
      print::line("");

      write_f.open("output_bench/" + output_fname);
      write_f << "2\n";

      for (float i = istart; i < istop; i += step) {
        fct(i);
      }
      write_f.close();
    }
  };

  struct bench_result_single
  {
    std::size_t elapsed_us;
    std::uint64_t int_chk;
  };

} // namespace acts_slice