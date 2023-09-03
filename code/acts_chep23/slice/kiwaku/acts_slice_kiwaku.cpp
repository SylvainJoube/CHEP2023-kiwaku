#include <iostream>
#include <cstddef>
// #include <fstream>
// #include <vector>
// #include <memory>
#include <cmath>
#include <chrono>
#include <iostream>
#include <kwk/kwk.hpp>

#include "bitmap_kiwaku.hpp"
// #include "../standalone/bitmap_standalone.hpp"
#include "../../acts_field/kiwaku/acts_struct_kiwaku.hpp"
// #include "../../acts_field/kiwaku/data_structures_kiwaku.hpp"

#include "../../../common/utils/utils.hpp"
#include "../../../common/program_args.hpp"
#include "../acts_slice_utils.hpp"

/*
CASSIDI:
$ clang++-15 acts_field_kiwaku.cpp bitmap.cpp acts_struct.cpp -o exe -O3 -std=c++20 -I"/home/joube/kiwaku/include" && ./exe 

CHATON:
$ clang++-15 acts_field_kiwaku.cpp bitmap.cpp acts_struct.cpp -o exe -O3 -std=c++20 -I"/home/data_not_sync/logiciels/kiwaku/include" && ./exe 

$ mencoder mf:// *.bmp -mf fps=10:type=bmp -ovc x264 -x264encopts bitrate=1200:threads=2 -o outputfile.mkv

There should be no space between the // and the *, but -Werror treats it as an error "/ * within block comment" so...
*/

acts_slice::bench_result_single render_slice_iteration_kiwaku(float z_value, bool use_inline, acts_data_t& a, bool save_image)
{
  uint const width  = 1024;
  uint const height = 1024;

  bench::chrono_t chrono;
  std::string chk_str;
  chrono.Init();

  auto img = kwk::table{kwk::type = kwk::int8, kwk::of_size(width, height)};

  if (use_inline)
  {
    kwk::for_each_index([ fw = static_cast<float>(width)
                        , fh = static_cast<float>(height)
                        , z_value
                        , &a
                        ]
    (auto& e, auto x, auto y) 
    {
      float fx = x / fw;
      float fy = y / fh;

      pt3D<float> asked{fx * 20000.f - 10000.f, fy * 20000.f - 10000.f, z_value};

      pt3D<float> p = a.inline_at(asked);
      // Do something with p
      e = static_cast<char>(std::lround(
                    255.f *
                    std::min(
                        std::sqrt(p[0] * p[0] + p[1] * p[1] + p[2] * p[2]), 1.0f
                    )
                ));
    }
    , img
    );
  } else {
    kwk::for_each_index([ fw = static_cast<float>(width)
                        , fh = static_cast<float>(height)
                        , z_value
                        , &a
                        ]
    (auto& e, auto x, auto y) 
    {
      float fx = x / fw;
      float fy = y / fh;

      pt3D<float> asked{fx * 20000.f - 10000.f, fy * 20000.f - 10000.f, z_value};
      pt3D<float> p = a.at(asked);

      // Do something with p
      e = static_cast<char>(std::lround(
                    255.f *
                    std::min(
                        std::sqrt(p[0] * p[0] + p[1] * p[1] + p[2] * p[2]), 1.0f
                    )
                ));
    }
    , img
    );
  }

  // Checking image integrity
  std::uint64_t int_chk = 0;
  kwk::for_each([&int_chk](auto& e) 
  {
    int_chk += static_cast<std::uint8_t>(e);
  }
  , img);

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


  if (save_image)
  {
    std::string inline_str = use_inline ? "inline" : "noinline";
    std::string img_path = "output_image/slice_kiwaku_" + std::to_string(static_cast<int>(z_value/100)) + ".bmp";
    // a.print_extremums();
    render_bitmap(img, img_path);
  }

  // The kwk::table img will be automatically destroyed at the end of this function
  return res;
}


void render_slice_kiwaku(float z_value, bool use_inline)
{
  acts_data_t a;
  a.read_acts_file();

  acts_slice::bench_result_single r;
  std::uint64_t int_chk;
  std::vector<std::size_t> times; // measured times, microseconds
  for (std::size_t i = 0; i < program_args::iteration_count; ++i)
  {
    r = render_slice_iteration_kiwaku(z_value, use_inline, a, (i == 0));
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



void print_bench_header()
{
  printer_t::line("Elapsed time, milliseconds:");
  print::str(print::pad_right("z_value", 8));
  print::str(print::pad_right("time", 10));
  print::line("");
  // for (int i = 0; i < 5; ++i) {
  //   print::str(print::pad_right("deep_" + std::to_string(i), 10));
  // }
  // print::line("");
}

int main()
{
  // bool use_inline;

  auto iteration_inline   = [](float i){ render_slice_kiwaku(i, true);  };
  auto iteration_noinline = [](float i){ render_slice_kiwaku(i, false); };

  printer_t::line("\n");
  printer_t::head("KIWAKU INLINE");
  print_bench_header();
  acts_slice::benchmark_t::run_benchmark("slice_kiwaku_inline", iteration_inline);

  printer_t::line("\n");
  printer_t::head("KIWAKU NOT INLINE");
  print_bench_header();
  acts_slice::benchmark_t::run_benchmark("slice_kiwaku_noinline", iteration_noinline);

  printer_t::line("\n");

  return 0;
}
