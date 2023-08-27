cmake_minimum_required(VERSION 3.16)

project(acts_chep23 LANGUAGES CXX)

# For VSCode's clangd compatibility
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
# set(CMAKE_CXX_STANDARD 20)

set(CMAKE_CXX_FLAGS "-Wall -Wextra")
set(CMAKE_CXX_FLAGS_DEBUG "-g")
set(CMAKE_CXX_FLAGS_RELEASE "-O3")

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release)
endif()


include(dependencies.cmake)

##==================================================================================================
## Compiler options
##==================================================================================================
add_library(kwk_compiler_options INTERFACE)

target_compile_features ( kwk_compiler_options INTERFACE  cxx_std_20 )
if (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
  target_compile_options( kwk_compiler_options INTERFACE /W3 /EHsc)
else()
  target_compile_options( kwk_compiler_options INTERFACE -Werror -Wall -Wextra)
endif()

target_link_libraries(kwk_compiler_options INTERFACE kiwaku::kiwaku)
##==================================================================================================


message(STATUS "Testing = ${kiwaku_SOURCE_DIR}")

# find_package(benchmark REQUIRED)


# ======= LORENTZ EULER KIWAKU =======
# add_executable(lorentz_kiwaku.exe ../lorentz_kiwaku.cpp)
# # L'utilisation de kwk_compiler_options permet d'éviter
# # d'avoir à réécrire tout à chaque target (ex lorentz_kiwaku.exe, acts_slice.exe, ...)
# target_link_libraries(lorentz_kiwaku.exe PUBLIC kwk_compiler_options) #ma_target_avec_kiwaku
# target_link_libraries(lorentz_kiwaku.exe PRIVATE benchmark::benchmark)


# ======= LORENTZ EULER STANDALONE =======
add_executable(lorentz_standalone.exe ../lorentz_euler/standalone/lorentz_standalone.cpp)
add_executable(lorentz_standalone_image.exe ../lorentz_euler_image/standalone/lorentz_standalone.cpp)
# target_link_libraries(lorentz_standalone.exe PRIVATE benchmark::benchmark)

# ======= LORENTZ EULER MEM PATTERN TEST =======
add_executable(mem_pattern_test.exe ../lorentz_euler/mem_pattern_test/mem_pattern_test.cpp)

# ======= LORENTZ EULER KIWAKU =======
add_executable(lorentz_kiwaku.exe ../lorentz_euler/kiwaku/lorentz_kiwaku.cpp ../acts_field/kiwaku/acts_struct_kiwaku.cpp)
target_link_libraries(lorentz_kiwaku.exe PUBLIC kwk_compiler_options)

# ======= SLICE STANDALONE =======
add_executable(acts_slice_standalone.exe ../slice/standalone/acts_slice_standalone.cpp)

# ======= SLICE KIWAKU =======
add_executable(acts_slice_kiwaku.exe ../slice/kiwaku/acts_slice_kiwaku.cpp ../acts_field/kiwaku/acts_struct_kiwaku.cpp)
target_link_libraries(acts_slice_kiwaku.exe PUBLIC kwk_compiler_options)


# Run cmake within the main directory with the command:
#   cmake -B build -S cmake -DCPM_kiwaku_SOURCE=/home/data_not_sync/logiciels/kiwaku -G Ninja -DCMAKE_CXX_COMPILER=clang++-15

# Compile the project with:
#   ninja -v lorentz_kiwaku.exe

# And run the project with:
#   ./lorentz_kiwaku.exe


# Compile when already cd in build:
# cmake ../cmake -DCPM_kiwaku_SOURCE=/home/data_not_sync/logiciels/kiwaku -G Ninja -DCMAKE_CXX_COMPILER=clang++-15
