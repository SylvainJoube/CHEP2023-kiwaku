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
#include "../acts_slice_utils.hpp"

// joube@ls-cassidi:~/shared/covfie-stephen$ cmake --build build -- -j $(nproc) && build/examples/cpu/render_slice_cpu --input atlas.cvf --output my_image.bmp --z 0


// using field_t1 =
// covfie::field : une table
// covfie::backend::affine : backend (owning data)
// covfie::backend::linear : interpolation linéaire des coordonnées
// covfie::backend::strided : qui a un stride comme nos tables et vues
// covfie::vector::ulong3 : en 3D
// covfie::backend::array<covfie::vector::float3>> : dont chaque cellule est une structure constituée de 3 floats

// using field_t1 = covfie::field<covfie::backend::affine<
//       covfie::backend::linear<covfie::backend::strided<
//       covfie::vector::ulong3,
//       covfie::backend::array<covfie::vector::float3>>>>>;

// using data_t = kwk::make_table_t<kwk::as<pt3D<float>>, kwk::_3D, kwk::affine_transform(kwk::linear)>;

/*
C'est un array<float3>
qui est strided avec 3 dimensions (ulong3)
qui a une interpolation linéaire des coordonnées
qui subit une transformation affine sur ses coordonnées
encapsulé par un covfie::field.

Lecture du fichier dans cet ordre :
- (dim est supposé connu, le nombre de dimensions du stride)
- lecture de la transformation affine ((dim)*(dim+1)*sizeof(float))
- lecture des tailles des (dim) dimensions
- lecture du nombre d'éléments total (devrait être dim1*dim2*...*dimN)
- puis ce sont les éléments les uns à la suite des autres

Commande pour relancer l'exemple :
cmake --build build -- -j $(nproc) && build/examples/cpu/render_slice_cpu --input atlas.cvf --output my_image.bmp --z 0

CASSIDI:
$ clang++-15 acts_field_kiwaku.cpp bitmap.cpp acts_struct.cpp -o exe -O3 -std=c++20 -I"/home/joube/kiwaku/include" && ./exe 

CHATON:
$ clang++-15 acts_field_kiwaku.cpp bitmap.cpp acts_struct.cpp -o exe -O3 -std=c++20 -I"/home/data_not_sync/logiciels/kiwaku/include" && ./exe 

$ mencoder mf:// *.bmp -mf fps=10:type=bmp -ovc x264 -x264encopts bitrate=1200:threads=2 -o outputfile.mkv

There should be no space between the // and the *, but -Werror treats it as an error "/ * within block comment" so...

*/


double render_slice_kiwaku(float z_value, bool use_inline)
{
  acts_data_t a;
  a.read_acts_file();

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

  double elapsed_time = chrono.ElapsedTime();

  // Checking image integrity
  std::uint64_t int_chk = 0;
  kwk::for_each([&int_chk](auto& e) 
  {
    int_chk += static_cast<std::uint8_t>(e);
  }
  , img);

  /*
  Structure du fichier (texte) :
  Autant de fois qu'il y a d'évènements (nouvelle ligne) :
    z_value | elapsed_time_us | check_string
  */
  write_f
  << z_value << " " 
  << static_cast<uint64_t>(elapsed_time * 1000000) << " " // microseconds
  << int_chk << "\n";

  // std::cout << "---> z_value(" << z_value << ")";
  // std::cout << " took " << static_cast<std::size_t>(elapsed_time*1000) << "ms\n";

  // if (write_image) {
    std::string inline_str = use_inline ? "inline" : "noinline";
    // Pointer version
    // render_bitmap(static_cast<char*>(img.get_data().get()), width, height, "img_out/2023-03-26_jimg_" + std::to_string(z_value/1000) + ".bmp");
    // Kiwaku version:
    render_bitmap(img, "../slice/kiwaku/img_out/kiwaku_" + inline_str + "_" + std::to_string(static_cast<int>(z_value/100)) + ".bmp");
  // }

  print::str(print::pad_right(z_value, 8));
  print::str(print::pad_right(static_cast<std::size_t>(elapsed_time * 1000), 10));
  print::line("");

  return elapsed_time;
}


// void render_slice_kiwaku(float z_value, bool use_inline)
// {
//   // std::cout << "---> z_value(" << z_value << ")";

//   double time[5];
//   // Per line: z_value time_depth4 chk_depth4 ... time_depth0 chk_depth0\n 
//   write_f << z_value;

//   time[4] = render_slice_depth_kiwaku<4>(z_value, true , use_inline);

//   if (use_inline) {
//     time[3] = render_slice_depth_kiwaku<3>(z_value, false, use_inline);
//     time[2] = render_slice_depth_kiwaku<2>(z_value, false, use_inline);
//     time[1] = render_slice_depth_kiwaku<1>(z_value, false, use_inline);
//     time[0] = render_slice_depth_kiwaku<0>(z_value, false, use_inline);
//   } else {
//     for (uint i = 0; i < 4; ++i) time[i] = 0; // DEEP not supported for the noinline version
//   }
//   write_f << "\n";

//   print::str(print::pad_right(z_value, 10));

//   for (int i = 0; i < 5; ++i) {
//     print::str(print::pad_right(static_cast<std::size_t>(time[i] * 1000), 10));
//   }
//   print::line("");
// }

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
  acts_slice_benchmark_t::run_benchmark("slice_kiwaku_inline", iteration_inline);

  printer_t::line("\n");
  printer_t::head("KIWAKU NOT INLINE");
  print_bench_header();
  acts_slice_benchmark_t::run_benchmark("slice_kiwaku_noinline", iteration_noinline);

  printer_t::line("\n");

  return 0;
}
