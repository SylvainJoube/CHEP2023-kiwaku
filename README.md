# Kiwaku, a C++20 library for multidimensional arrays: Application to ACTS tracking

This repository contains the necessary code for testing the Kiwaku benchmark code described in the paper called `Kiwaku, a C++20 library for multidimensional arrays: Application to ACTS tracking`.

## Credits

TODO Thanks Covfie + links

## Prerequisites

You will need a C++20 compatible compiler such as `clang++-12` and `git`.

### Without CPM

To execute Kiwaku code you will need a clone on the [Kiwaku repository](https://github.com/jfalcou/kiwaku).

```bash
git clone https://github.com/jfalcou/kiwaku.git
cd kiwaku
git checkout 5ad368b
```

If you are using vscode, you can add this to your `.vscode/c_cpp_properties.json`:

```json
{
    "configurations": [
        {
            "includePath": [
                "/home/data_sync/academique/These/kiwaku_2023/kiwaku/include"
            ],
            "compilerPath": "/usr/bin/clang++-12",
            "cppStandard": "c++20",
            "intelliSenseMode": "linux-gcc-x64"
        }
    ]
}
```

## Compiling the benchmarks

```bash
cd code/acts_chep23/

export CPP_COMPILER="clang++-12"
export KIWAKU_INCLUDE_DIR="/home/data_sync/academique/These/kiwaku_2023/kiwaku/include/"

# Compiling the Lorentz-Euler standalone code
$CPP_COMPILER lorentz_euler/standalone/lorentz_standalone.cpp -o lorentz-euler_standalone.exe -O3 -std=c++20

# Compiling the Lorentz-Euler Kiwaku code
$CPP_COMPILER lorentz_euler/kiwaku/lorentz_kiwaku.cpp acts_field/kiwaku/acts_struct_kiwaku.cpp -o lorentz-euler_kiwaku.exe -O3 -std=c++20 -I$KIWAKU_INCLUDE_DIR

# Compiling the ACTS magnetic-field slicing standalone code
$CPP_COMPILER slice/standalone/acts_slice_standalone.cpp -o acts_slice_standalone.exe -O3 -std=c++20

# Compiling the ACTS magnetic-field slicing Kiwaku code
$CPP_COMPILER slice/kiwaku/acts_slice_kiwaku.cpp acts_field/kiwaku/acts_struct_kiwaku.cpp -o acts_slice_kiwaku.exe -O3 -std=c++20 -I$KIWAKU_INCLUDE_DIR

```

## Running the benchmarks

If not done yet, go to directory `code/acts_chep23/`.

When ran without arguments, the default configuration is loaded. If you need to specify a custom configuration, here are the accepted command-line arguments:

- `"-field"` or `"-F"` allows you to choose the field type you want for your benchmarks. By default both `"acts-field"` and `"constant-field"` will be used (input `"all"`), each one with its own output file `.bench.txt`. Allowed inputs: `"all"`, `"acts"`, `"constant"`.

- `"-computer"` or `"-C"` allows you to choose the computer name given to the output benchmark file. By default, the name `"generic"` will be used. Using the input `"real"` or `"real_name"` will change the name into the real system name of the computer, as returned by the linux command `"hostname"`.

- `"-timer"` or `"-T"` allows you to choose the timing method. By default, the `std::chrono::system_clock` will be used (input `"sclock"`), but you can use `nanobench` to get cpu cycles instead (input `"nanobench"`). If so, you will need to run the benchmark as admin, and keep the global variable `"COVFIE_BENCHMARK_FIELD"` by running the benchmark as follows: `"sudo --preserve-env ./lorentz-euler_kiwaku.exe -timer nanobench"`. ou don't need admin rights to use the `system_clock`: `"./lorentz-euler_kiwaku.exe -timer sclock"`. Allowed inputs: `"sclock"`, `"nanobench"`.

Should you want to change the `steps`, `particles` and `imom` bench values, please directly change the code written on file `code/acts_chep23/lorentz_euler/lorentz_bench_common.hpp`, function `std::vector<std::vector<std::uint64_t>> get_parameter_ranges()` from namespace `lorentz`.

- TODO: `"-iteration_count"` per configuration (default = 10)

```bash

# The ACTS field location is necessary to run the ACTS-related benchmarks
export COVFIE_BENCHMARK_FIELD="/home/data_sync/academique/These/covfie_sylvain/atlas.cvf"

# Running the Lorentz-Euler standalone benchmark with default parameters
./lorentz-euler_standalone.exe

# Running the Lorentz-Euler standalone benchmark with custom parameters
./lorentz-euler_standalone.exe -field all -computer real -timer sclock

# Running with nanobench (cpu cycles instead of time)
sudo -E ./lorentz-euler_standalone.exe -field acts -timer nanobench

# Running the Lorentz-Euler Kiwaku benchmark
./lorentz-euler_kiwaku.exe

# Running the ACTS magnetic-field slicing Kiwaku benchmark
./acts_slice_kiwaku.exe

# Running the ACTS magnetic-field slicing standalone benchmark
./acts_slice_standalone.exe


./lorentz-euler_kiwaku.exe && ./lorentz-euler_standalone.exe && ./acts_slice_kiwaku.exe && ./acts_slice_standalone.exe
```



## Plot the benchmark results for the Lorentz-Euler algorithm

Benchmark output files must be placed in the `output_bench` directory for the `plot.py` python script to access them. Command line arguments can be provided to the plot script. The input file names needed by the script varies depending on its command line arguments.

By default the Lorentz-Euler algorithm ran by the command `python3 plot.py -algorithm lorentz` will ask for the following file names: 
```
Associated file names:
          Kiwaku: output_bench/lorentz_sclock_it10_512-8192_1024_128_acts-field_kiwaku_generic.bench.txt
      Standalone: output_bench/lorentz_sclock_it10_512-8192_1024_128_acts-field_standalone_generic.bench.txt
    Out plot img: output_plot/lorentz_sclock_it10_512-8192_1024_128_acts-field_generic_relative.bench.png
```

- `"lorentz"` means we want the output file corresponding to the Lorentz-Euler benchmark.
- `"sclock"` means we want the benchmark using `std::chrono::system_clock` as opposed to `nanobench` cpu cycles. 
- `"it10"` means each test is repeated 10 times, for consistency. The mean will be taken by the plot, and violin plots will be drawn for each particle count value. **TODO You can change this setting TODO**. 
- The three following values can be changed by editing the benchmark C++ code. Keep in mind you will also need to change them in the `plot.py` script.
  - `"512-8192"` is the particle number range for the benchmark. 
  - `"1024"` is the "steps" parameter, meaning the number of times each particle will be moved in the magnetic field. 
  - `"128"` is the "imom" value, the initial kinetic moment for each particle. 
- `"acts-field"` refers to the magnetic field used. 
- `"generic"` indicates the computer name used for the benchmark.

A full explanation for the input command line arguments for the plot code can be displayed by calling `python3 plot.py --help`. Ift is reproduced as follows.

* `"-algorithm"` or `"-A"` chooses which algorithm to plot: `"lorentz"` or `"slice"`
* `"-field"` or `"-F"` chooses which field to plot: `"acts"` or `"constant"` (default: `"acts"`)
* `"-computer"` or `"-CPT"` sets the computer name used for the benchmark output file (default: `"generic"`)
* `"-timer"` or `"-T"` tets the time measurement method used for the benchmark output file: `"sclock"` for system clock timer or `"nanobench"` for the nanobench measurements. (default: `"sclock"`)
* `"-compare"` or `"-CMP"` Plot strategy. `"relative"`: plots by comparing the Kiwaku curve with the standalone. `"absolute"`: plots absolute time/cycles values. (default: `"relative"`)
* For the C++ benchmark code, the following three arguments can only be changed by editing the C++ files (please see the `Running the benchmarks` section for more info).
  * `"-particles"` or `"-LP"` For Lorentz-Euler algorithm only. Sets the range for particles, as written on the output benchmark file. Format: `"min-max"` or `"value"` in case `(min==max)`. Example: `"512-8192"` or `"512`". (default: `"512-8192"`)
  * `"-steps"` or `"-LS"` For Lorentz-Euler algorithm only. Sets the range for steps (number of move iterations for each particle), as written on the output benchmark file. Format: `"min-max"` or `"value"` in case `(min==max)`. Example: `"512-8192"` or `"512"`. (default: `"1024"`)
  * `"-imom"` or `"-LM"` For Lorentz-Euler algorithm only. Sets the range for imom (initial kinetic momentum for each particle), as written on the output benchmark file. Format: `"min-max"` or `"value"` in case `(min==max)`. Example: `"512-8192"` or `"512"`. (default: `"128"`)

















A single cmake project to handle the lorentz_euler and acts_slice benchmarks.

## Run the cmake in directory `CODE/acts_chep23/`

```bash

# CHATON
export COVFIE_BENCHMARK_FIELD="/home/data_sync/academique/These/covfie_sylvain/atlas.cvf"

# MSI
export COVFIE_BENCHMARK_FIELD="/media/sylvain/ext_ssd/SYNC/THESE_EXTERNAL/These/covfie_sylvain/atlas.cvf"

# BLOP
export COVFIE_BENCHMARK_FIELD="/home/blop/uploads/covfie_sylvain/atlas.cvf"


export CXX=clang++-15 && rm -fr build && cmake -B build -S cmake -DCMAKE_CXX_STANDARD=20 -DCMAKE_BUILD_TYPE=Release && cd build

make -j`nproc` # or only `make` for less errors on terminal on failed compilation

sudo -E ./lorentz_standalone.exe
sudo -E ./lorentz_kiwaku.exe
./acts_slice_standalone.exe
./acts_slice_kiwaku.exe
```

## Covfie_sylvain


As described in the `README.md` of my `covfie_sylvain` repo:

```bash
export CMAKE_PREF=""
export COVFIE_BENCHMARK_FIELD="/home/blop/uploads/covfie_sylvain/atlas.cvf"

export CXX=clang++-15 \
&& cmake -S covfie_sylvain -B build -DCOVFIE_BUILD_EXAMPLES=On -DCOVFIE_BUILD_TESTS=Off -DCOVFIE_BUILD_BENCHMARKS=On -DCOVFIE_PLATFORM_CPU=On \
                                    -DCMAKE_BUILD_TYPE=Release -DCMAKE_CXX_STANDARD=20 -DCMAKE_CUDA_STANDARD=17 -DCOVFIE_PLATFORM_CUDA=Off \
                                    -DCMAKE_PREFIX_PATH=$CMAKE_PREF

# Lorentz-euler:
cmake --build build -- -j $(nproc) && build/benchmarks/cpu/benchmark_cpu_custom

# ACTS slicing:
cmake --build build -- -j $(nproc) && build/examples/cpu/render_slice_cpu --input atlas.cvf --output my_image.bmp --z 0
```




Idée pour mesurer les différents temps du .at() :
- Temps complet
- Temps sans l'accès stride
- Temps sans l'accès linéaire
- Temps sans l'accès affine
- Et du coup je peux en déduire les temps pris




Performance for lorentz-euler Covfie, Blop, 2023-04-17 12h30:

```log
blop@blop-debian11:~/uploads/covfie_sylvain$ build/benchmarks/cpu/benchmark_cpu_custom
=================================
=================================
====- Lorentz-Euler COVFIE -=====
=================================
=================================

Will output in directory:  /home/blop/uploads/covfie_sylvain

File name:                 lorentz-euler_covfie_blop-debian11_2023-04-17_4096-65536_1024-2048_128-131072_InterpolateNN-LayoutStride-field.txt

 particles     steps      imom   total pa*st     elapsed s         mul
      4096      1024       128       4194304      1.575944          29
      4096      1024       192       4194304      1.553637          29
      4096      1024      6144       4194304      1.726434          29
      4096      1024     65536       4194304      4.620862          29
      4096      1024    131072       4194304      5.449882          29
      4096      2048       128       8388608      1.597258          15
      4096      2048       192       8388608      1.590215          15
      4096      2048      6144       8388608      1.947615          15
      4096      2048     65536       8388608      4.841546          15
      4096      2048    131072       8388608      5.647073          15
     65536      1024       128      67108864      1.739062           2
     65536      1024       192      67108864      1.714444           2
     65536      1024      6144      67108864      1.940223           2
     65536      1024     65536      67108864      5.108807           2
     65536      1024    131072      67108864      6.015323           2
     65536      2048       128     134217728      1.703939           1
     65536      2048       192     134217728      1.696807           1
     65536      2048      6144     134217728      2.090378           1
     65536      2048     65536     134217728      5.168547           1
     65536      2048    131072     134217728      6.027456           1
count = 20

File name:                 lorentz-euler_covfie_blop-debian11_2023-04-17_4096-65536_1024-2048_128-131072_constant-field.txt

 particles     steps      imom   total pa*st     elapsed s         mul
      4096      1024       128       4194304      0.856208          29
      4096      1024       192       4194304      0.856169          29
      4096      1024      6144       4194304      0.856188          29
      4096      1024     65536       4194304      0.859634          29
      4096      1024    131072       4194304      0.863069          29
      4096      2048       128       8388608      0.886024          15
      4096      2048       192       8388608      0.886009          15
      4096      2048      6144       8388608      0.886014          15
      4096      2048     65536       8388608      0.889485          15
      4096      2048    131072       8388608      0.893032          15
     65536      1024       128      67108864      0.944773           2
     65536      1024       192      67108864      0.944751           2
     65536      1024      6144      67108864      0.944753           2
     65536      1024     65536      67108864      0.948623           2
     65536      1024    131072      67108864      0.952401           2
     65536      2048       128     134217728      0.945103           1
     65536      2048       192     134217728      0.945109           1
     65536      2048      6144     134217728      0.945086           1
     65536      2048     65536     134217728      0.948815           1
     65536      2048    131072     134217728      0.952577           1
count = 20
```


Performance for lorentz-euler standalone, Blop, 2023-04-17 12h30:

```log
blop@blop-debian11:~/uploads/these-sylvain-joube/CODE/acts_chep23/build$ ./lorentz_standalone.exe 

===================================================
===================================================
======- STANDALONE Lorentz-Euler STANDALONE -======
===================================================
===================================================

Will output to file:  lorentz_standalone_blop-debian11_2023-04-16_4096-65536_1024-2048_128-131072_constant-field.txt
In directory:         /home/blop/uploads/these-sylvain-joube/CODE/acts_chep23/build

 particles     steps      imom   total pa*st     elapsed s         mul
      4096      1024       128       4194304      0.869233          29
      4096      1024       192       4194304      0.860901          29
      4096      1024      6144       4194304      0.860934          29
      4096      1024     65536       4194304      0.865577          29
      4096      1024    131072       4194304      0.870745          29
      4096      2048       128       8388608      0.890617          15
      4096      2048       192       8388608      0.890656          15
      4096      2048      6144       8388608      0.890550          15
      4096      2048     65536       8388608      0.895373          15
      4096      2048    131072       8388608      0.900708          15
     65536      1024       128      67108864      0.949960           2
     65536      1024       192      67108864      0.949879           2
     65536      1024      6144      67108864      0.949910           2
     65536      1024     65536      67108864      0.955147           2
     65536      1024    131072      67108864      0.960755           2
     65536      2048       128     134217728      0.949939           1
     65536      2048       192     134217728      0.949957           1
     65536      2048      6144     134217728      0.949982           1
     65536      2048     65536     134217728      0.955124           1
     65536      2048    131072     134217728      0.960834           1
```




## Analyse covfie 2023-04-22

### Complete reconstruction of the chain used to get the value at the desired position

```c++

struct FieldLin_aff {
    using backend_t =
    covfie::backend::affine<
      covfie::backend::linear<
        covfie::backend::strided<covfie::vector::ulong3, 
          covfie::backend::array<covfie::vector::float3>>>>;

    static covfie::field<backend_t> get_field()
    {
        return covfie::field<backend_t>(get_test_field());
    }
};


using data_field_t =
  covfie::field<
    covfie::backend::affine<
      covfie::backend::nearest_neighbour<
        covfie::backend::strided<covfie::vector::ulong3,
          covfie::backend::array<covfie::vector::float3>>>>>;

const data_field_t & get_test_field() { return std::make_unique<data_field_t>(std::ifstream ifs(file_name, std::ifstream::binary)).get(); }

field_ = 
  covfie::field<backend_t>(get_test_field());

const covfie::field_view<backend_t> & field_ = FieldLin_aff::get_field()
-> cast into field_view

COVFIE_DEVICE output_t at(Args... c) const 
{
    return m_storage.at(coordinate_t{c...});
}

COVFIE_DEVICE output_t at(coordinate_t c) const
{
    return m_storage.at(c);
}

storage_t m_storage;
using storage_t = typename backend_t::non_owning_data_t;
                = covfie::backend::affine::non_owning_data_t

With: backend_t = same as above (I guess)

non_owning_data_t

// ======= AFFINE =======
COVFIE_DEVICE typename covariant_output_t::vector_t
at(typename contravariant_input_t::vector_t c) const
{
    covfie::algebra::vector<
        contravariant_input_t::dimensions,
        typename contravariant_input_t::scalar_t>
        v;
    
    for (std::size_t i = 0; i < contravariant_output_t::dimensions; ++i)
    {
        v(i) = c[i];
    }

    covfie::algebra::vector<
        contravariant_input_t::dimensions,
        typename contravariant_input_t::scalar_t>
        nv = m_transform * v;
        
    typename contravariant_output_t::vector_t nc;

    for (std::size_t i = 0; i < contravariant_output_t::dimensions; ++i)
    {
        nc[i] = nv(i);
    }
    
    return m_backend.at(nc);
}

// ======= LINEAR =======




// ======= STRIDED =======


p = field_.at(0.f, 0.f, 0.f);
```



