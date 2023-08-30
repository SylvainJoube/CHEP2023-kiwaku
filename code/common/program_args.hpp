#pragma once

#include <vector>
#include <string_view>
#include <string>
#include <algorithm>
#include <iostream>
#include <sstream>
#include "utils/host_name.hpp"
#include "lorentz-euler/lorentz-euler_utlils.hpp"




namespace program_args
{

  bool bench_field_acts       = true;
  bool bench_field_constant   = true;
  bool use_system_clock       = true;
  bool use_nanobench          = false;
  // bool replace_computer_name  = true;
  std::string computer_name_str = "generic"; // will be replaced in parse(...), if necessary
  std::size_t iteration_count = 10;

  namespace lorentz
  {
    std::vector<std::vector<std::uint64_t>> get_parameter_ranges()
    {
      // Reduced set of parameters
      return
      {
          {512, 1024, 2048, 4096, 8192}, //, 16384, 32768, 65536},
          {1024}, // , 2048, 4096, 8192, 16384, 32768, 65536
          {128} // ,   192,  6144, 65536, 131072
      };

      // MEM_TEST
      // return
      // {
      //     {4096},
      //     {1024, 2048},
      //     {128}
      // };

      // Full set of parameters, as in covfie
      // return {
      //     {4096, 65536},
      //     {1024, 2048, 4096, 8192, 16384, 32768, 65536},
      //     {128,   192,   256,   384,   512,   768,   1024,
      //      1536,  2048,  3072,  4096,  6144,  8192,  12288,
      //      16384, 24576, 32768, 49152, 65536, 98304, 131072}};
    }

    std::string pretty_print_single_range(const std::vector<std::uint64_t>& param)
    {
      if (param.size() == 1) {
        return std::to_string(param[0]);
      } else {
        return std::to_string(param[0]) + "-" + std::to_string(param[param.size()-1]);
      }
    }

    std::string pretty_print_ranges()
    {
      auto params = get_parameter_ranges();

      std::string params_str = "";
      for (std::size_t i = 0; i < params.size(); ++i) {
        const std::vector<std::uint64_t>& param = params[i];
        params_str += "_" + pretty_print_single_range(param);
        // if (param.size() == 1) {
        //   params_str += "_" + std::to_string(param[0]);
        // } else {
        //   params_str += "_" + std::to_string(param[0]) + "-" + std::to_string(param[param.size()-1]);
        // }
      }
      return params_str;
    }

    std::string make_output_fname(std::string arch_name, std::string field_name)
    {
      std::string params_str = program_args::lorentz::pretty_print_ranges();
      std::string prefix;

      #if SMEM_ENABLE_LOG
        prefix = "with_mtest_lorentz";
      #else
        prefix = "lorentz";
      #endif
      
      // Always use optimized matrix
      // #if USE_OPTIMIZED_MATRIX
      //   prefix = prefix + "_opti";
      // #else
      //   prefix = prefix + "";
      // #endif

      // #if USE_NANOBENCH #if USE_SYSCLOCK
      //#endif
      if (::program_args::use_nanobench)    prefix = prefix + "_nanobench";
      if (::program_args::use_system_clock) prefix = prefix + "_sclock";
      
      prefix += "_it" +std::to_string(lorentz_iteration_result_t::iteration_base);

      return prefix + params_str + "_" + field_name + "_" + arch_name + "_" + computer_name_str + ".bench.txt";
    }
    
  }




  // program_args::bench_field_acts     
  // program_args::bench_field_constant 
  // program_args::use_system_clock     
  // program_args::use_nanobench        



  void print_bench_params(bool is_lorentz)
  {
    std::cout << "\nCurrent benchmark parameters:\n";
    std::cout << "     -field: ";
    if (bench_field_acts && bench_field_constant) std::cout << "\"all\" (both acts and constant)";
    else if (bench_field_acts)                    std::cout << "\"acts\"";
    else if (bench_field_constant)                std::cout << "\"constant\"";
    else                                          std::cout << "unknown (ERROR)";
    std::cout << "\n";

    // if (replace_computer_name)
    std::cout << "  -computer: \"" << computer_name_str << "\" (override system name)\n";
    // else
    //   std::cout << "  -computer: \"" << get_computer_name() << "\" (use system name)\n";

    if (use_system_clock) std::cout << "     -timer: \"sclock\" (use system clock)\n";
    if (use_nanobench)    std::cout << "     -timer: \"nanobench\" (use nanobench)\n";

    if (is_lorentz)
    {
      auto params = program_args::lorentz::get_parameter_ranges();

      std::cout << " -particles: \"" << program_args::lorentz::pretty_print_single_range(params[0]) << "\"\n";
      std::cout << "     -steps: \"" << program_args::lorentz::pretty_print_single_range(params[1]) << "\"\n";
      std::cout << "      -imom: \"" << program_args::lorentz::pretty_print_single_range(params[2]) << "\"\n";
    }
    std::cout << "\n";

    std::cout << "Should you want to change the `steps`, `particles` and `imom` bench values"
              << ", please directly change the code written on file `code/acts_chep23/lorentz_euler/lorentz_bench_common.hpp`"
              << ", function `get_parameter_ranges()` from namespace `lorentz`.\n\n";

  }

  
  bool check_parameter_match( std::string param
                            , std::vector<std::string> accepted_names
                            )
  {
    // Return true if parameter was found, false otherwise
    if (std::find(accepted_names.begin(), accepted_names.end(), param) == accepted_names.end())
      return false;
    return true;
  }

  // Returns true if param matches accepted_names
  bool check_parameter( std::string param
                      , std::string value
                      , std::vector<std::string> accepted_names
                      , std::vector<std::string> accepted_values
                      )
  {
    // Return true if parameter was found, false otherwise
    if ( ! check_parameter_match(param, accepted_names) )
      return false;
    
    // Parameter was found. Returns true if valid value, false otherwise
    if (std::find(accepted_values.begin(), accepted_values.end(), value) == accepted_values.end())
    {
      std::stringstream ss;
      ss << "ERROR: unexpected value given for parameter \"" + param + "\": \"" + value << "\"\n";
      ss << "       Accepted values are:";
      for (const auto& s : accepted_values) {
        ss << " \"" << s << "\" ";
      }
      std::cout << ss.str() << "\n\n";
      std::terminate();
      return false;
    }

    return true;
  }



  void parse(int argc, char* argv[], bool is_lorentz)
  {
    if (argc <= 1) 
    {
      print_bench_params(is_lorentz);
      return;
    }
    
    const std::vector<std::string> args(argv + 1, argv + argc);
    std::vector<std::string> accepted_args;
    // accepted_args.push_back("-algorithm");
    accepted_args.push_back("-field");
    accepted_args.push_back("-F");
    accepted_args.push_back("-computer");
    accepted_args.push_back("-C");
    accepted_args.push_back("-timer");
    accepted_args.push_back("-T");
    
    // Should you want to change the `steps`, `particles` and `imom` bench values, 
    // please directly change the code written on file `code/acts_chep23/lorentz_euler/lorentz_bench_common.hpp`, 
    // function `std::vector<std::vector<std::uint64_t>> get_parameter_ranges()` from namespace `lorentz`.

    // accepted_args.push_back("-particles");
    // accepted_args.push_back("-LP");
    // accepted_args.push_back("-steps");
    // accepted_args.push_back("-LS");
    // accepted_args.push_back("-imom");
    // accepted_args.push_back("-LM");

    std::size_t i = 0;
    while (i < args.size())
    {
      std::string s1 = args[i];
      bool found = std::find(accepted_args.begin(), accepted_args.end(), s1) != accepted_args.end();

      if (! found) {
        std::cout << "ERROR: unexpected parameter \"" + s1 + "\"\n";
        std::exit(-1);
      }

      if (i + 1 == args.size()) {
        std::cout << "ERROR: parameter \"" + s1 + "\" expects a value.\n";
        std::exit(-1);
      }

      std::string s2 = args[i + 1];

      if (check_parameter(s1, s2, {"-field", "-F"}, {"all", "acts", "constant"}))
      {
        bench_field_acts     = (s2 == "all") || (s2 == "acts");
        bench_field_constant = (s2 == "all") || (s2 == "constant");
      }
      
      if (check_parameter(s1, s2, {"-timer", "-T"}, {"sclock", "nanobench"}))
      {
        use_system_clock = (s2 == "sclock");
        use_nanobench    = (s2 == "nanobench");
      }

      if (check_parameter_match(s1, {"-computer", "-C"}))
      {
        if ((s2 == "real") || (s2 == "real_name"))
        {
          // replace_computer_name = false;
          computer_name_str = get_computer_name();
        } else {
          // replace_computer_name = true;
          computer_name_str = s2;
        }
      }


      // TODO:
      // - faire fonctionner le "--help"
      // - continuer les vérifications
      // - prendre en compte les paramètres dans le code  
      // - les paramètres "steps", "particles", "imom" ne sont pas modifiables
      //   via des arguments, ça impliquerait de changer la manière dont le plot
      //   affiche les données, et ça me prendrait trop de temps pour pas grand chose.
      // - vérifier que le plot fonctionne correctement avec les données générées  


      i += 2;
      // if ((s1 == "-field") || (s1 == "-F")) {
      //   if ((s2 != "acts") && (s2 != "constant"))
    }

    print_bench_params(is_lorentz);

  }
}