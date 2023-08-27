/*
 * This file is part of covfie, a part of the ACTS project
 *
 * Copyright (c) 2022 CERN
 *
 * This Source Code Form is subject to the terms of the Mozilla Public License,
 * v. 2.0. If a copy of the MPL was not distributed with this file, You can
 * obtain one at http://mozilla.org/MPL/2.0/.
 */

#pragma once

#include <iostream>
#include <random>
#include "../../common/utils/utils.hpp"
#include "../../common/lorentz-euler/lorentz-euler_utlils.hpp"
#include "../acts_field/field_common.hpp"
#include "../acts_field/data_structures_common.hpp"
#include "lorentz_bench_common.hpp"
#include "bitmap_standalone.hpp"


#if USE_NANOBENCH
  #define ANKERL_NANOBENCH_IMPLEMENT
  #include "../../common/nanobench.h"
#endif



class Euler
{
};

class RungeKutta4
{
};

class Wide
{
};

class Deep
{
};

namespace covfie::benchmark
{
  struct counters
  {
    std::size_t access_per_it;
    std::size_t bytes_per_it;
    std::size_t flops_per_it;
  };

  template <std::size_t N>
  struct lorentz_agent
  {
    float pos[N];
    float mom[N];
  };

  template <std::size_t N>
  struct propagation_agent
  {
    float pos[N];
  };

  template <typename T, std::size_t N>
  struct random_agent
  {
    T pos[N];
  };
}

template <typename Propagator, typename Order>
struct Lorentz
{

  struct parameters
  {
    std::size_t particles, steps, imom;
  };

  static constexpr std::array<std::string_view, 3> parameter_names = {
      "N", "S", "I"};
  
  struct iteration_result_t
  {
    std::string chk_str = "";
    double elapsed_time_s = 0;
    double cycles = 0;
  };

  using particle_vector = std::vector<covfie::benchmark::lorentz_agent<3>>;

  static inline particle_vector
  particles_creation(std::size_t particles, std::size_t imom)
  {
    std::mt19937 e(42); // rd()
    std::uniform_real_distribution<> phi_dist(0.f, 2.f * 3.1415927f);
    std::uniform_real_distribution<> costheta_dist(-1.f, 1.f);
    std::uniform_real_distribution<> imom_dist(0.1f, 1.f);

    particle_vector objs(particles);

    for (std::size_t i = 0; i < particles; ++i) {
      float theta = std::acos(costheta_dist(e));
      float phi = phi_dist(e);

      objs[i].pos[0] = 0.f;
      objs[i].pos[1] = 0.f;
      objs[i].pos[2] = 0.f;

      std::size_t choosen_imom = imom; // imom_dist(e) * imom;

      objs[i].mom[0] =
          static_cast<float>(choosen_imom) * std::sin(theta) * std::cos(phi);
      objs[i].mom[1] =
          static_cast<float>(choosen_imom) * std::sin(theta) * std::sin(phi);
      objs[i].mom[2] = static_cast<float>(choosen_imom) * std::cos(theta);
    }
    return objs;
  }

  template <typename Data_T>
  static inline void 
  particles_steps_computation
  (
      particle_vector& objs
    , std::size_t particles
    , std::size_t steps
    , os_table_t<Data_T>& table_
  )
  {
    float ss = 0.001f;

    if constexpr (std::is_same_v<Order, Deep>) {
      for (std::size_t i = 0; i < particles; ++i) {
        for (std::size_t s = 0; s < steps; ++s) {
          covfie::benchmark::lorentz_agent<3> &o = objs[i];
          float lf[3];

          if constexpr (std::is_same_v<Propagator, Euler>) {
            pt3D<float> b = table_.at(o.pos[0], o.pos[1], o.pos[2]);
            lf[0] = o.mom[1] * b[2] - o.mom[2] * b[1];
            lf[1] = o.mom[2] * b[0] - o.mom[0] * b[2];
            lf[2] = o.mom[0] * b[1] - o.mom[1] * b[0];
            // ++access_count;
          }

          o.pos[0] += o.mom[0] * ss;
          o.pos[1] += o.mom[1] * ss;
          o.pos[2] += o.mom[2] * ss;

          if (__builtin_expect(
                  o.pos[0] < -9999.f || o.pos[0] > 9999.f ||
                      o.pos[1] < -9999.f || o.pos[1] > 9999.f ||
                      o.pos[2] < -14999.f || o.pos[2] > 14999.f,
                  0))
          {
            if      (o.pos[0] < -9999.f) { o.pos[0] += 19998.f; }
            else if (o.pos[0] > 9999.f)  { o.pos[0] -= 19998.f; }

            if      (o.pos[1] < -9999.f) { o.pos[1] += 19998.f; }
            else if (o.pos[1] > 9999.f)  { o.pos[1] -= 19998.f; }

            if      (o.pos[2] < -14999.f) { o.pos[2] += 29998.f; }
            else if (o.pos[2] > 14999.f)  { o.pos[2] -= 29998.f; }
          }

          o.mom[0] += lf[0] * ss;
          o.mom[1] += lf[1] * ss;
          o.mom[2] += lf[2] * ss;

        }
      }
    } else {
      for (std::size_t s = 0; s < steps; ++s) {
        for (std::size_t i = 0; i < particles; ++i) {
          covfie::benchmark::lorentz_agent<3> &o = objs[i];
          float lf[3];

          if constexpr (std::is_same_v<Propagator, Euler>) {
            pt3D<float> b = table_.at(o.pos[0], o.pos[1], o.pos[2]);
            lf[0] = o.mom[1] * b[2] - o.mom[2] * b[1];
            lf[1] = o.mom[2] * b[0] - o.mom[0] * b[2];
            lf[2] = o.mom[0] * b[1] - o.mom[1] * b[0];
            // ++access_count;
          }

          o.pos[0] += o.mom[0] * ss;
          o.pos[1] += o.mom[1] * ss;
          o.pos[2] += o.mom[2] * ss;

          if (__builtin_expect(
                  o.pos[0] < -9999.f || o.pos[0] > 9999.f ||
                      o.pos[1] < -9999.f || o.pos[1] > 9999.f ||
                      o.pos[2] < -14999.f || o.pos[2] > 14999.f,
                  0))
          {
            if      (o.pos[0] < -9999.f) { o.pos[0] += 19998.f; }
            else if (o.pos[0] > 9999.f)  { o.pos[0] -= 19998.f; }

            if      (o.pos[1] < -9999.f) { o.pos[1] += 19998.f; }
            else if (o.pos[1] > 9999.f)  { o.pos[1] -= 19998.f; }

            if      (o.pos[2] < -14999.f) { o.pos[2] += 29998.f; }
            else if (o.pos[2] > 14999.f)  { o.pos[2] -= 29998.f; }
          }

          o.mom[0] += lf[0] * ss;
          o.mom[1] += lf[1] * ss;
          o.mom[2] += lf[2] * ss;
        }
      }
    }
  }


  template <typename Data_T>
  static inline void 
  particles_steps_computation_graphic
  (
      particle_vector& objs
    , std::size_t particles
    , std::size_t steps
    , os_table_t<Data_T>& table_
    , std::string img_out_fname
  )
  {
    float ss = 0.001f;

    std::size_t image_width  = 1024;
    std::size_t image_height = 1024;
    char* img = new char[image_width * image_height];
    for (std::size_t i = 0; i < image_width * image_height; ++i) {
      img[i] = 0;
    }

    // img (x,y) -> pos (px,py) in field:
    // px = (x / image_width) * 20000.f - 10000.f
    // pos (px,py) in field -> img (x,y) 
    // x = image_width * (px + 10000.f)/20000.f
    // pt3D<float> p = a.at(fx * 20000.f - 10000.f, fy * 20000.f - 10000.f, z_value);

    if constexpr (std::is_same_v<Order, Deep>) {
      for (std::size_t i = 0; i < particles; ++i) {
        for (std::size_t s = 0; s < steps; ++s) {
          covfie::benchmark::lorentz_agent<3> &o = objs[i];
          float lf[3];

          if constexpr (std::is_same_v<Propagator, Euler>) {
            pt3D<float> b = table_.at(o.pos[0], o.pos[1], o.pos[2]);
            lf[0] = o.mom[1] * b[2] - o.mom[2] * b[1];
            lf[1] = o.mom[2] * b[0] - o.mom[0] * b[2];
            lf[2] = o.mom[0] * b[1] - o.mom[1] * b[0];
            // ++access_count;
          }

          std::size_t ix = static_cast<float>(image_width)  * (o.pos[0] + 10000.f) / 20000.f;
          std::size_t iy = static_cast<float>(image_height) * (o.pos[1] + 10000.f) / 20000.f;

          img[image_height * ix + iy] = static_cast<char>(255);

          o.pos[0] += o.mom[0] * ss;
          o.pos[1] += o.mom[1] * ss;
          o.pos[2] += o.mom[2] * ss;

          if (__builtin_expect(
                  o.pos[0] < -9999.f || o.pos[0] > 9999.f ||
                      o.pos[1] < -9999.f || o.pos[1] > 9999.f ||
                      o.pos[2] < -14999.f || o.pos[2] > 14999.f,
                  0))
          {
            if      (o.pos[0] < -9999.f) { o.pos[0] += 19998.f; }
            else if (o.pos[0] > 9999.f)  { o.pos[0] -= 19998.f; }

            if      (o.pos[1] < -9999.f) { o.pos[1] += 19998.f; }
            else if (o.pos[1] > 9999.f)  { o.pos[1] -= 19998.f; }

            if      (o.pos[2] < -14999.f) { o.pos[2] += 29998.f; }
            else if (o.pos[2] > 14999.f)  { o.pos[2] -= 29998.f; }
          }
          

          o.mom[0] += lf[0] * ss;
          o.mom[1] += lf[1] * ss;
          o.mom[2] += lf[2] * ss;

        }
      }
    } else {
      for (std::size_t s = 0; s < steps; ++s) {
        for (std::size_t i = 0; i < particles; ++i) {
          covfie::benchmark::lorentz_agent<3> &o = objs[i];
          float lf[3];

          if constexpr (std::is_same_v<Propagator, Euler>) {
            pt3D<float> b = table_.at(o.pos[0], o.pos[1], o.pos[2]);
            lf[0] = o.mom[1] * b[2] - o.mom[2] * b[1];
            lf[1] = o.mom[2] * b[0] - o.mom[0] * b[2];
            lf[2] = o.mom[0] * b[1] - o.mom[1] * b[0];
            // ++access_count;
          }

          o.pos[0] += o.mom[0] * ss;
          o.pos[1] += o.mom[1] * ss;
          o.pos[2] += o.mom[2] * ss;

          if (__builtin_expect(
                  o.pos[0] < -9999.f || o.pos[0] > 9999.f ||
                      o.pos[1] < -9999.f || o.pos[1] > 9999.f ||
                      o.pos[2] < -14999.f || o.pos[2] > 14999.f,
                  0))
          {
            if      (o.pos[0] < -9999.f) { o.pos[0] += 19998.f; }
            else if (o.pos[0] > 9999.f)  { o.pos[0] -= 19998.f; }

            if      (o.pos[1] < -9999.f) { o.pos[1] += 19998.f; }
            else if (o.pos[1] > 9999.f)  { o.pos[1] -= 19998.f; }

            if      (o.pos[2] < -14999.f) { o.pos[2] += 29998.f; }
            else if (o.pos[2] > 14999.f)  { o.pos[2] -= 29998.f; }
          }

          o.mom[0] += lf[0] * ss;
          o.mom[1] += lf[1] * ss;
          o.mom[2] += lf[2] * ss;
        }
      }
    }

    render_bitmap
    (
        img,//.get(),
        image_width,
        image_height,
        img_out_fname
    );
      
    delete[] img;
  }





  
  static std::string particles_result_checking(particle_vector& objs)
  {
    std::string chk_str;
    float total_pos[3];
    float total_mom[3];
    for (std::size_t i = 0; i < 3; ++i) {
      total_pos[i] = 0;
      total_mom[i] = 0;
    }
    
    // float total_p1 = 0, total_p2 = 0, total_p3 = 0;
    // float total_mom1 = 0, total_mom2 = 0, total_mom3 = 0;

    // for (std::size_t i = 0; i < particles; ++i) {
    for (const auto &pa : objs) {
      for (std::size_t i = 0; i < 3; ++i) {
        total_pos[i] += pa.pos[i];
        total_mom[i] += pa.mom[i];
      }
    }

    chk_str = "CHK";
    for (std::size_t i = 0; i < 3; ++i) {
      chk_str += "_" + std::to_string(total_mom[i]);
    }
    chk_str += "/";
    for (std::size_t i = 0; i < 3; ++i) {
      if (i != 0) chk_str += "_";
      chk_str += std::to_string(total_pos[i]);
    }
    // chk_str = "CHK_" + std::to_string(total_mom1) + "_" + std::to_string(total_mom2) + "_" + std::to_string(total_mom3) + "/" + std::to_string(total_p1) + "_" + std::to_string(total_p2) + "_" + std::to_string(total_p3);

    if (G_PRINT_LORENTZ_CHK) { std::cout << chk_str; }
    return chk_str;
  }


  template <typename Data_T>
  static iteration_result_t iteration
  (
      std::size_t particles
    , std::size_t steps
    , std::size_t imom
    , os_table_t<Data_T> &table_
    , std::string img_out_fname
  )
  {
    iteration_result_t itres;
    bench::chrono_t chrono;

    particle_vector objs = particles_creation(particles, imom);

    #if USE_NANOBENCH
      ankerl::nanobench::Bench bench;
      std::size_t run_count = 0;
      bench = ankerl::nanobench::Bench().epochs(1).run("Lorentz iteration",
        [&]{
          particles_steps_computation(objs, particles, steps, table_);
          ++run_count;
        }
      );
      if (run_count != 1) {
        print::err("Lorentz::iteration: nanobench run_count(" + std::to_string(run_count) + ") != 1");
      }
      std::vector<ankerl::nanobench::Result> vres = bench.results();

      // double cyc_op_med           =   vres.begin()->median(ankerl::nanobench::Result::Measure::cpucycles);
      double cyc_op_mean          =   vres.begin()->average(ankerl::nanobench::Result::Measure::cpucycles); // Nombre cycles total
      // double cyc_op_max           =   vres.begin()->maximum(ankerl::nanobench::Result::Measure::cpucycles);
      // double cyc_op_min           =   vres.begin()->minimum(ankerl::nanobench::Result::Measure::cpucycles);
      // double cyc_op_err           =   vres.begin()->medianAbsolutePercentError(ankerl::nanobench::Result::Measure::cpucycles) ;

      // std::cout << print::pad_right(cyc_op_mean, 14);
      // std::cout << print::pad_right(cyc_op_med , 14);
      // std::cout << print::pad_right(cyc_op_min , 14);
      // std::cout << print::pad_right(cyc_op_max , 14);
      // std::cout << print::pad_right(cyc_op_err , 14);
      itres.cycles = cyc_op_mean;
      // std::cout << "cyc_op_mean = " << cyc_op_mean << "  run_count = " << run_count << std::endl;
    #endif
    // Uses the same objs, I do not think that is an issue.
    #if USE_SYSCLOCK
      chrono.Init();
      particles_steps_computation_graphic(objs, particles, steps, table_, img_out_fname);
      itres.elapsed_time_s = chrono.ElapsedTime();
    #endif

    itres.chk_str = particles_result_checking(objs);
    return itres;
  }
};

struct lorentz_bench_t
{

  template <typename FIELD_T, std::size_t REPEAT_FACTOR>
  static void
  execute_custom(std::size_t particles, std::size_t steps, std::size_t imom, std::string img_out_fname) // , FIELD_T* field
  {
    // os_constant_field os_field{std::array<float, 3>{0.f, 0.f, 2.f}};
    os_table_t<FIELD_T> os_table{};

// #if SMEM_ENABLE_LOG
//     // MEM TEST
//     std::size_t iterations = 1;
// #else
//     double multiply = REPEAT_FACTOR * 6000000. / (particles * steps);
//     std::size_t iterations = static_cast<std::size_t>(multiply) + lorentz_iteration_result_t::iteration_base;
// #endif

    std::size_t iterations = 1;


    // std::cout << print::pad_right("cyc_op_mean", 14);
    // std::cout << print::pad_right("cyc_op_med" , 14);
    // std::cout << print::pad_right("cyc_op_min" , 14);
    // std::cout << print::pad_right("cyc_op_max" , 14);
    // std::cout << print::pad_right("cyc_op_err" , 14);
    // std::cout << std::endl;

    lorentz_iteration_result_t res_total;

    #if USE_NANOBENCH
      res_total.cycles.resize(iterations);
    #endif

    #if USE_SYSCLOCK
      res_total.times_s.resize(iterations);
    #endif

    for (uint i = 0; i < iterations; ++i)
    {
      Lorentz<Euler, Deep>::iteration_result_t res;
      res = Lorentz<Euler, Deep>::iteration<FIELD_T>(particles, steps, imom, os_table, img_out_fname);
      
      if (res_total.chk_str != "" && res_total.chk_str != res.chk_str)
      {
        print::err("lorentz_bench_t::execute_custom: res_total.chk_str != r.chk_str\n"
                    + res_total.chk_str + " !=\n"
                    + res.chk_str + " !=\n"
                  );
      }
      res_total.chk_str = res.chk_str;

      #if USE_NANOBENCH
        res_total.cycles[i] = res.cycles;
      #endif

      #if USE_SYSCLOCK
        res_total.times_s[i] = res.elapsed_time_s;
      #endif

      // res_total.elapsed_time_s += r.elapsed_time_s;
    }

    // lorentz_iteration_result_t b;
    res_total.particles = particles;
    res_total.steps = steps;
    res_total.imom = imom;
    res_total.repeat_iterations = iterations;
    //res_total.elapsed_time_s = res_total.elapsed_time_s; //chrono.ElapsedTime();
    res_total.chk_str = res_total.chk_str;
    write_f << res_total;
    res_total.print_results();

    // Structure du fichier (texte) :
    // Autant de fois qu'il y a d'évènements (nouvelle ligne) :
    // v1:
    //   particles | steps | imom | repeat_count | elapsed_time_us | check_string

    // v2:
    //   particles | steps | imom | repeat_count | check_string
    //   elapsed_time_us_1 | ... | elapsed_time_us_N
    //   cycles_1 | ... | cycles_N
    //   


    // v1
    // lorentz_iteration_result_t b;
    // b.particles = particles;
    // b.steps = steps;
    // b.imom = imom;
    // b.repeat_iterations = iterations;
    // b.elapsed_time_s = res_total.elapsed_time_s; //chrono.ElapsedTime();
    // b.chk_str = res_total.chk_str;
    // write_f << b;
    // b.print_results();
  }
};