#include <iostream>
#include <filesystem>
#include "../../../common/utils/utils.hpp"
#include "../../../common/lorentz-euler/lorentz-euler_utlils.hpp"
#include "../../../common/program_args.hpp"
#include "../lorentz_bench_common.hpp"
#include "../lorentz_common.hpp"

#include "../../acts_field/standalone/field_standalone.hpp"

/*

CHATON
export CMAKE_PREF="/home/data_not_sync/logiciels/benchmark/build"
export COVFIE_BENCHMARK_FIELD="/home/data_sync/academique/These/covfie_sylvain/atlas.cvf"

BLOP
export CMAKE_PREF=""
export COVFIE_BENCHMARK_FIELD="/home/blop/uploads/covfie_sylvain/atlas.cvf"


Thinkpad sans cmake car à La Pierre :
  export COVFIE_BENCHMARK_FIELD="/home/data_sync/academique/These/covfie_sylvain/atlas.cvf"
ou  
  export COVFIE_BENCHMARK_FIELD="/home/data_sync/academique/These/chep23-pap/data/atlas.cvf"
cd /home/data_sync/academique/These/chep23-pap/code/acts_chep23/lorentz_euler/kiwaku
clang++ lorentz_standalone.cpp ../../acts_field/standalone/acts_struct_standalone.cpp -o e.exe -O3 -std=c++20 -I/home/data_sync/academique/These/kiwaku_2023/kiwaku/include/ && ./e.exe

*/


// ==== En standalone dans mon repo
// Avec le flag "DCMAKE_CXX_STANDARD=20" et "-DCMAKE_BUILD_TYPE=Release" ajouté :
/*

export CXX=clang++-15 \
&& cd .. && rm -fr build 
&& cmake -B build -S cmake -DCMAKE_PREFIX_PATH="/home/data_not_sync/logiciels/benchmark/build" \
-DCMAKE_CXX_STANDARD=20 -DCMAKE_BUILD_TYPE=Release \
&& cd build && make \
&& ./lorentz_standalone.exe


New command to use to compile the project
cmake -B build -S cmake -DCMAKE_CXX_STANDARD=20 -DCMAKE_BUILD_TYPE=Release && cd build && make

*/

// Cassidi
// export CXX=clang++-15 && cd .. && rm -fr build && cmake -B build -S cmake -DCMAKE_PREFIX_PATH="/home/joube/tests_2023-03-10/benchmark/build" -DCMAKE_CXX_STANDARD=20 -DCMAKE_BUILD_TYPE=Release && cd build && make


/*
Structure du fichier (texte) :
Autant de fois qu'il y a d'évènements (nouvelle ligne) :
  particles | steps | imom | repeat_count | elapsed_time | check_string
*/


int main([[maybe_unused]] int argc, [[maybe_unused]] char ** argv)
{
  print::line("");
  print::head("STANDALONE Lorentz-Euler STANDALONE");

  program_args::parse(argc, argv, true);

  if (program_args::bench_field_acts) 
  {
    lorentz::bench_with_field<os_acts_field>("standalone", "acts-field", lorentz_bench_t::execute_custom<os_acts_field, 4>); // , &acts_field
  }

  if (program_args::bench_field_constant) 
  {
    lorentz::bench_with_field<os_constant_field>("standalone", "constant-field", lorentz_bench_t::execute_custom<os_constant_field, 20>); // , &constant_field
  }
  return 0;
}
