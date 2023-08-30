#pragma once

#define SCALAR_TYPE float
#define SMEM_ENABLE_LOG false

#define USE_OPTIMIZED_MATRIX true // should always be true

// Bench using nanobench
// replaced by program_args::use_nanobench
// #define USE_NANOBENCH false

// Bench using std::system_clock
// replaced by program_args::use_system_clock
// #define USE_SYSCLOCK true