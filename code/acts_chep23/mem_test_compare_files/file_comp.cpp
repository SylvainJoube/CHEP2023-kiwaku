#include "text_file.hpp"


// clang++-15 file_comp.cpp -o e -O3 -std=c++20 && ./e

int main()
{
  text_file_r standalone("MEM_TEST_lorentz_standalone_chaton_4096-4096_1024-2048_128-128_acts-field.txt");
  text_file_r covfie("MEM_TEST_lorentz-euler_covfie_chaton_4096-4096_1024-2048_128-128_InterpolateLin-LayoutStride-field.txt");

  std::size_t count = 0;
  while (true) {
    std::string ls = standalone.read_line();
    std::string lc = covfie.read_line();
    if (ls != lc) std::cout << "PAS PAREIL !! @line " << count << "\n";
    if (ls == "") break;
    count++;
    if (count % 10000000 == 0) {
      std::cout << "c" << count << "... " << ls << "==" << lc << "\n";
      // std::cout << "c" << count << "...\n";
     }
  }
  std::cout << "Final count " << count << "\n";
}

