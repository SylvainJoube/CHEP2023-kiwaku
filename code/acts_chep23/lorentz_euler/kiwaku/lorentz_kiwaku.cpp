#include <iostream>
#include <filesystem>
#include "../../../common/utils/utils.hpp"
#include "../../../common/lorentz-euler/lorentz-euler_utlils.hpp"
#include "../../../common/program_args.hpp"
#include "../lorentz_bench_common.hpp"
#include "../lorentz_common.hpp"

#include "../../acts_field/kiwaku/field_kiwaku.hpp"


// ==== Dans covfie :
// cmake --build build -- -j $(nproc) && build/benchmarks/cpu/benchmark_cpu_custom
/*


=== nope ===
for basic benchmark functionalities : sudo apt install libbenchmark-dev
I will not be using "/home/data_not_sync/logiciels/benchmark/build" anymore,
since the newest version has deprecated some elements and generates huge
waning messages that mey interfere with my ability to see other errors.
=== nope ===


CHATON
export CMAKE_PREF="/home/data_not_sync/logiciels/benchmark/build"
export COVFIE_BENCHMARK_FIELD="/home/data_sync/academique/These/covfie_sylvain/atlas.cvf"

BLOP
export CMAKE_PREF=""
export COVFIE_BENCHMARK_FIELD="/home/blop/uploads/covfie_sylvain/atlas.cvf"

export CXX=clang++-15 \
&& cmake -S covfie_sylvain -B build -DCOVFIE_BUILD_EXAMPLES=On -DCOVFIE_BUILD_TESTS=Off -DCOVFIE_BUILD_BENCHMARKS=On -DCOVFIE_PLATFORM_CPU=On \
                                    -DCMAKE_BUILD_TYPE=Release -DCMAKE_CXX_STANDARD=20 -DCMAKE_CUDA_STANDARD=17 -DCOVFIE_PLATFORM_CUDA=Off \
                                    -DCMAKE_PREFIX_PATH=$CMAKE_PREF



Thinkpad sans cmake car à La Pierre :
  export COVFIE_BENCHMARK_FIELD="/home/data_sync/academique/These/covfie_sylvain/atlas.cvf"
ou  
  export COVFIE_BENCHMARK_FIELD="/home/data_sync/academique/These/chep23-pap/data/atlas.cvf"
cd /home/data_sync/academique/These/chep23-pap/code/acts_chep23/lorentz_euler/kiwaku
dpcpp lorentz_kiwaku.cpp ../../acts_field/kiwaku/acts_struct_kiwaku.cpp -o e.exe -O3 -std=c++20 -I/home/data_sync/academique/These/kiwaku_2023/kiwaku/include/ && ./e.exe





cmake --build build -- -j $(nproc) && build/benchmarks/cpu/benchmark_cpu_custom

*/

// Cassidi
/*
export CXX=clang++-15 \
&& cmake -S covfie -B build -DCOVFIE_BUILD_EXAMPLES=On -DCOVFIE_BUILD_TESTS=Off -DCOVFIE_BUILD_BENCHMARKS=On -DCOVFIE_PLATFORM_CPU=On \
                            -DCMAKE_BUILD_TYPE=Release -DCMAKE_CXX_STANDARD=20 -DCMAKE_CUDA_STANDARD=17 -DCOVFIE_PLATFORM_CUDA=Off \
                            -DCMAKE_PREFIX_PATH="/home/joube/tests_2023-03-10/benchmark/build"
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
Trucs que je peux faire pour finir Lorentz-Euler et avoir des résultats propres :
- Lancer tous les tests sans gbench (via 3 boucles for pour toutes les configurations), vérifier les AccessCount=268.435M AccessRate=150.376M/s ArithmeticIntensity=1.75 LoadBandwidth=1.80451G/s LoadBytes=3.22123G
- Sauvegarder l'output réduit de chaque test pour en vérifier le résultat entre mes méthodes (+covfie original) (dans un fichier)
- Sauvegarder les temps pris pour chaque test, pour les plot via matplotlib pour comparer. Potentiellement prendre le log s'il y a des grands écarts, ou normaliser (temps / (particules * steps))
- Il faut manifestement avoir un particles * steps >= 100 000.
- Comparer les résultats et les temps pris, potentiellement matplotlib (avec en x les benchs et en y les temps, 3 courbes) ?

Pour lancer les mêmes tests :
- Faire une boucle de 3 éléments (particles, steps, imom) et comparer les résultats entre les 3 méthodes.

Structure du fichier (texte) :
Autant de fois qu'il y a d'évènements (nouvelle ligne) :
  particles | steps | imom | repeat_count | elapsed_time | check_string

Le champ "check_string" doit être d'un seul mot (sans espace) et permettre de vérifier
que les résultats sont identiques entre l'implémentation de Stephen et la mienne.

*/


int main([[maybe_unused]] int argc, [[maybe_unused]] char ** argv)
{
  // TODO: os_constant_field does not use Kiwaku (acts-field does, however)

  print::line("");
  print::head("KIWAKU Lorentz-Euler KIWAKU");

  program_args::parse(argc, argv, true);

  if (program_args::bench_field_acts) 
  {
    lorentz::bench_with_field<os_acts_field>("kiwaku", "acts-field", lorentz_bench_t::execute_custom<os_acts_field, 4>); // , &acts_field
  }

  if (program_args::bench_field_constant) 
  {
    lorentz::bench_with_field<os_constant_field>("kiwaku", "constant-field", lorentz_bench_t::execute_custom<os_constant_field, 20>); // , &constant_field
  }

  return 0;
}
