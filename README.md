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














