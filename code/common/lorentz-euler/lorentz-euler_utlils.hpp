#pragma once

#include <iostream>
#include <string>
#include <vector>
#include "../utils/printer.hpp"

struct lorentz_iteration_result_t
{
  // File version
  static const std::size_t version = 2;
  
  // Min number of iterations for any given configuration (particles, steps, imom)
  static const std::size_t iteration_base = 10;

  std::size_t particles, steps, imom;
  
  // How many times the computing was done for the timing to be meaningful.
  std::size_t repeat_iterations;

  std::string chk_str;
  // std::uint64_t elapsed_time_us;
  // double elapsed_time_s;
  
  std::vector<double> times_s; // reprendre d'ici
  std::vector<std::size_t> cycles;

  // Print in a fild ostream, without pretty formatting
  inline friend std::ostream& operator<<(std::ostream& os, lorentz_iteration_result_t const& b) {
    os
    << b.particles << " " 
    << b.steps << " " 
    << b.imom << " "
    << b.repeat_iterations << " "
    << b.chk_str << "\n";

    // Write times_s
    for (std::size_t i = 0; i < b.times_s.size(); ++i) {
      os << static_cast<uint64_t>(b.times_s[i] * 1000000); // microseconds
      if (i + 1 !=  b.times_s.size()) {
        os << " ";
      }
    }
    os << "\n";

    // Write cycles
    for (std::size_t i = 0; i < b.cycles.size(); ++i) {
      os << b.cycles[i];
      if (i + 1 !=  b.cycles.size()) {
        os << " ";
      }
    }
    os << "\n";

    return os;
  }

  void print_results(std::ostream& os)
  {
    double moy_elapsed_s = 0;
    if (times_s.size() != 0){
      for (std::size_t i = 0; i < times_s.size(); ++i) {
        moy_elapsed_s += times_s[i];
      }
      moy_elapsed_s /= times_s.size();
    }
    
    std::size_t moy_cycles = 0;
    if (cycles.size() != 0) {
      for (std::size_t i = 0; i < cycles.size(); ++i) {
        moy_cycles += cycles[i];
      }
      moy_cycles /= cycles.size();
    }

    os
        << pad_right(std::to_string(particles), 10) 
        << pad_right(std::to_string(steps), 10) 
        << pad_right(std::to_string(imom), 10) 
        << pad_right(std::to_string(particles*steps), 14) 
        << pad_right(std::to_string(static_cast<double>(static_cast<std::size_t>(moy_elapsed_s * 1000000))/1000000), 14) 
        << pad_right(std::to_string(static_cast<std::size_t>(moy_cycles)), 14) 
        << pad_right(std::to_string(repeat_iterations), 12) 
        << "\n";
  }

  void print_results() {
    print_results(std::cout);
  }

  static void print_header(std::ostream& os) {
    os
        << pad_right("particles", 10)
        << pad_right("steps", 10) 
        << pad_right("imom", 10) 
        << pad_right("total pa*st", 14) 
        << pad_right("time (moy)", 14) 
        << pad_right("cycles (moy)", 14) 
        << pad_right("mul", 12) 
        << "\n";
  }
  static void print_header() {
    print_header(std::cout);
  }
};