#include <iostream>
#include <cstddef>
// #include <fstream>
// #include <vector>
// #include <memory>
#include <cmath>
#include <chrono>
#include "bitmap_standalone.hpp"
// #include "data_structures.hpp"
// #include "acts_struct.hpp"
#include "../../acts_field/standalone/acts_struct_standalone.hpp"
// #include "../../acts_field/standalone/data_structures_standalone.hpp"

#include "../../../common/utils/utils.hpp"
#include "../../../common/program_args.hpp"

// #include "../../../common/acts_slice/acts_slice_utils.hpp"
#include "../acts_slice_utils.hpp"
#include "../../global_variables.hpp"

#include "bitmap_standalone.hpp"


acts_slice::bench_result_single render_slice_iteration(float z_value, acts_data_t& a, bool save_image)
{
  std::size_t image_width  = 1024;
  std::size_t image_height = 1024;

  bench::chrono_t chrono;
  chrono.Init();

  char* img = new char[image_width * image_height];

  // std::unique_ptr<char[]> img =
  //   std::make_unique<char[]>(image_width * image_height);

  for (uint x = 0; x < image_width; ++x)
  {
    for (uint y = 0; y < image_height; ++y) 
    {
      float fx = x / static_cast<float>(image_width);
      float fy = y / static_cast<float>(image_height);

      // pt3D<float> asked{fx * 20000.f - 10000.f, fy * 20000.f - 10000.f, z_value};
      pt3D<float> p = a.at(fx * 20000.f - 10000.f, fy * 20000.f - 10000.f, z_value);
      // Do something with p
      img[image_height * x + y] =
                static_cast<char>(std::lround(
                    255.f *
                    std::min(
                        std::sqrt(p[0] * p[0] + p[1] * p[1] + p[2] * p[2]), 1.0f
                    )
                ));
    }
  }

  // Checking image integrity
  std::uint64_t int_chk = 0;
  for (std::size_t x = 0; x < image_width; ++x) {
    for (std::size_t y = 0; y < image_height; ++y) {
      int_chk += static_cast<std::uint8_t>(img[image_height * x + y]); //img[image_height * x + y]
    }
  }

  double elapsed_time = chrono.ElapsedTime();

  /*
  Structure du fichier (texte) :
  Autant de fois qu'il y a d'évènements (nouvelle ligne) :
    z_value | elapsed_time_us | check_string
  */
  // write_f
  // << z_value << " " 
  // << static_cast<uint64_t>(elapsed_time * 1000000) << " " // microseconds
  // << int_chk << "\n";

  acts_slice::bench_result_single res;
  res.elapsed_us  = static_cast<uint64_t>(elapsed_time * 1000000); // microseconds
  res.int_chk     = int_chk;
  
  // std::cout << " took " << static_cast<std::size_t>(elapsed_time*1000) << "ms\n";

  if (save_image)
  {
    std::string img_path = "output_image/slice_standalone_" + std::to_string(static_cast<int>(z_value/100)) + ".bmp";
    // a.print_extremums();
    render_bitmap(
          img,//.get(),
          image_width,
          image_height,
          img_path
      );
  }
  delete[] img;
  return res;
}

void render_slice(float z_value)
{
  acts_data_t a;
  a.read_acts_file();

  acts_slice::bench_result_single r;
  std::uint64_t int_chk;
  std::vector<std::size_t> times; // measured times, microseconds
  for (std::size_t i = 0; i < program_args::iteration_count; ++i)
  {
    r = render_slice_iteration(z_value, a, (i == 0));
    if (i == 0)
    {
      int_chk = r.int_chk;
    }
    else
    {
      if (int_chk != r.int_chk)
      {
        printer_t::error
        (
          "int_chk differs for the same z_value, iteration("
          + std::to_string(i) + ")."
        );
        std::terminate();
      }
    }
    times.push_back(r.elapsed_us);
  }


  write_f
  << z_value << " " 
  << int_chk << "\n";
  for (auto e : times)
  {
    write_f << e << " ";
  }
  write_f << "\n";

  double moy_elapsed_us = 0;
  if (times.size() != 0){
    for (std::size_t i = 0; i < times.size(); ++i) {
      moy_elapsed_us += times[i];
    }
    moy_elapsed_us /= times.size();
  }

  print::str(print::pad_right(z_value, 8));
  print::str(print::pad_right(static_cast<std::size_t>(moy_elapsed_us / 1000), 10));
  print::line("");

}

int main(int argc, char** argv)
{
  printer_t::line("\n");
  printer_t::head("HANDMADE STANDALONE (inline)");

  program_args::parse(argc, argv, true);
  

  printer_t::line("Elapsed time, milliseconds:");
  print::str(print::pad_right("z_value", 8));
  print::str(print::pad_right("time(ms)", 10));
  print::line("");

  // if (program_args::bench_field_acts) 
  // {
  //   lorentz::bench_with_field<os_acts_field>("kiwaku", "acts-field", lorentz_bench_t::execute_custom<os_acts_field, 4>); // , &acts_field
  // }

  // if (program_args::bench_field_constant) 
  // {
  //   lorentz::bench_with_field<os_constant_field>("kiwaku", "constant-field", lorentz_bench_t::execute_custom<os_constant_field, 20>); // , &constant_field
  // }

  acts_slice::benchmark_t::run_benchmark("slice_standalone", render_slice);
  
  printer_t::line("\n");

  return 0;
}
