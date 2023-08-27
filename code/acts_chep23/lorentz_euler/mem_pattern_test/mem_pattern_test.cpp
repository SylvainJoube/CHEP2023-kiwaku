#include <iostream>
#include <filesystem>
#include "../../../common/utils/utils.hpp"
#include "../../../common/lorentz-euler/lorentz-euler_utlils.hpp"
#include "../lorentz_bench_common.hpp"
#include "../../acts_field/standalone/field_standalone.hpp"
#include "../../acts_field/field_common.hpp"


void pattern_test()
{
  os_table_t<os_acts_field> os_table{};

  float s = 0;
  pt3D<float> p;

  std::size_t iter_nb = 1000000;

  bench::chrono_t chrono;
  chrono.Init();

  for (std::size_t i = 0; i < iter_nb; ++i) {
    p = os_table.at(0, 0, 0); s += p[0] + p[1] + p[2]; 
    p = os_table.at(0.0418574, 0.0941747, 0.075915); s += p[0] + p[1] + p[2]; 
    p = os_table.at(0.0839029, 0.188266, 0.15183); s += p[0] + p[1] + p[2]; 
    p = os_table.at(0.126136, 0.282273, 0.227745); s += p[0] + p[1] + p[2]; 
    p = os_table.at(0.168558, 0.376196, 0.303659); s += p[0] + p[1] + p[2]; 
    p = os_table.at(0.211167, 0.470035, 0.379574); s += p[0] + p[1] + p[2]; 
    p = os_table.at(0.253963, 0.563788, 0.455488); s += p[0] + p[1] + p[2]; 
    p = os_table.at(0.296947, 0.657456, 0.531402); s += p[0] + p[1] + p[2]; 
    p = os_table.at(0.340118, 0.751038, 0.607316); s += p[0] + p[1] + p[2]; 
    p = os_table.at(0.383475, 0.844534, 0.68323); s += p[0] + p[1] + p[2]; 
    p = os_table.at(0.42702, 0.937944, 0.759144); s += p[0] + p[1] + p[2]; 
    p = os_table.at(0.470751, 1.03127, 0.835057); s += p[0] + p[1] + p[2]; 
    p = os_table.at(0.514669, 1.1245, 0.910971); s += p[0] + p[1] + p[2]; 
    p = os_table.at(0.558773, 1.21765, 0.986884); s += p[0] + p[1] + p[2]; 
    p = os_table.at(0.603063, 1.31071, 1.0628); s += p[0] + p[1] + p[2]; 
    p = os_table.at(0.647539, 1.40368, 1.13871); s += p[0] + p[1] + p[2]; 
    p = os_table.at(0.6922, 1.49656, 1.21462); s += p[0] + p[1] + p[2]; 
    p = os_table.at(0.737048, 1.58936, 1.29054); s += p[0] + p[1] + p[2]; 
    p = os_table.at(0.78208, 1.68206, 1.36645); s += p[0] + p[1] + p[2]; 
    p = os_table.at(0.827298, 1.77468, 1.44236); s += p[0] + p[1] + p[2]; 
    p = os_table.at(0.872701, 1.8672, 1.51827); s += p[0] + p[1] + p[2]; 
    p = os_table.at(0.918289, 1.95963, 1.59419); s += p[0] + p[1] + p[2]; 
    p = os_table.at(0.964061, 2.05198, 1.6701); s += p[0] + p[1] + p[2]; 
    p = os_table.at(1.01002, 2.14423, 1.74601); s += p[0] + p[1] + p[2]; 
    p = os_table.at(1.05616, 2.23639, 1.82192); s += p[0] + p[1] + p[2]; 
    p = os_table.at(1.10248, 2.32845, 1.89783); s += p[0] + p[1] + p[2]; 
    p = os_table.at(1.14899, 2.42043, 1.97375); s += p[0] + p[1] + p[2]; 
    p = os_table.at(1.19569, 2.51231, 2.04966); s += p[0] + p[1] + p[2]; 
    p = os_table.at(1.24256, 2.6041, 2.12557); s += p[0] + p[1] + p[2]; 
    p = os_table.at(1.28962, 2.69579, 2.20148); s += p[0] + p[1] + p[2]; 
    p = os_table.at(1.33687, 2.7874, 2.27739); s += p[0] + p[1] + p[2]; 
    p = os_table.at(1.38429, 2.8789, 2.3533); s += p[0] + p[1] + p[2]; 
    p = os_table.at(1.4319, 2.97032, 2.42921); s += p[0] + p[1] + p[2]; 
    p = os_table.at(1.47969, 3.06163, 2.50512); s += p[0] + p[1] + p[2]; 
    p = os_table.at(1.52767, 3.15286, 2.58103); s += p[0] + p[1] + p[2]; 
    p = os_table.at(1.57582, 3.24398, 2.65694); s += p[0] + p[1] + p[2]; 
    p = os_table.at(1.62416, 3.33501, 2.73285); s += p[0] + p[1] + p[2]; 
    p = os_table.at(1.67268, 3.42595, 2.80876); s += p[0] + p[1] + p[2]; 
    p = os_table.at(1.72138, 3.51678, 2.88467); s += p[0] + p[1] + p[2]; 
    p = os_table.at(1.77027, 3.60752, 2.96058); s += p[0] + p[1] + p[2]; 
    p = os_table.at(1.81933, 3.69817, 3.03649); s += p[0] + p[1] + p[2]; 
    p = os_table.at(1.86858, 3.78871, 3.1124); s += p[0] + p[1] + p[2]; 
    p = os_table.at(1.918, 3.87916, 3.18831); s += p[0] + p[1] + p[2]; 
    p = os_table.at(1.96761, 3.9695, 3.26422); s += p[0] + p[1] + p[2]; 
    p = os_table.at(2.0174, 4.05975, 3.34013); s += p[0] + p[1] + p[2]; 
    p = os_table.at(2.06737, 4.1499, 3.41604); s += p[0] + p[1] + p[2]; 
    p = os_table.at(2.11752, 4.23995, 3.49195); s += p[0] + p[1] + p[2]; 
    p = os_table.at(2.16784, 4.3299, 3.56786); s += p[0] + p[1] + p[2]; 
    p = os_table.at(2.21835, 4.41975, 3.64377); s += p[0] + p[1] + p[2]; 
    p = os_table.at(2.26904, 4.5095, 3.71967); s += p[0] + p[1] + p[2]; 
    p = os_table.at(2.31991, 4.59915, 3.79558); s += p[0] + p[1] + p[2]; 
    p = os_table.at(2.37095, 4.68869, 3.87149); s += p[0] + p[1] + p[2]; 
    p = os_table.at(2.42218, 4.77814, 3.9474); s += p[0] + p[1] + p[2]; 
    p = os_table.at(2.47358, 4.86748, 4.02331); s += p[0] + p[1] + p[2]; 
    p = os_table.at(2.52516, 4.95672, 4.09921); s += p[0] + p[1] + p[2]; 
    p = os_table.at(2.57692, 5.04585, 4.17512); s += p[0] + p[1] + p[2]; 
    p = os_table.at(2.62886, 5.13488, 4.25103); s += p[0] + p[1] + p[2];
  }
  chrono.ElapsedTime();
  std::cout << s << " in " << chrono.Str() << "\n";
}

void pattern_test2()
{
  os_table_t<os_acts_field> os_table{};

  float s = 0;
  pt3D<float> p;

  std::size_t iter_nb = 1000000;

  bench::chrono_t chrono;
  chrono.Init();

  for (std::size_t i = 0; i < iter_nb; ++i) {
    p = os_table.at(0.f, 0.f, 0.f); s += p[0] + p[1] + p[2]; 
    p = os_table.at(0.0418574f, 0.0941747f, 0.075915f); s += p[0] + p[1] + p[2]; 
    p = os_table.at(0.0839029f, 0.188266f, 0.15183f); s += p[0] + p[1] + p[2]; 
    p = os_table.at(0.126136f, 0.282273f, 0.227745f); s += p[0] + p[1] + p[2]; 
    p = os_table.at(0.168558f, 0.376196f, 0.303659f); s += p[0] + p[1] + p[2]; 
    p = os_table.at(0.211167f, 0.470035f, 0.379574f); s += p[0] + p[1] + p[2]; 
    p = os_table.at(0.253963f, 0.563788f, 0.455488f); s += p[0] + p[1] + p[2]; 
    p = os_table.at(0.296947f, 0.657456f, 0.531402f); s += p[0] + p[1] + p[2]; 
    p = os_table.at(0.340118f, 0.751038f, 0.607316f); s += p[0] + p[1] + p[2]; 
    p = os_table.at(0.383475f, 0.844534f, 0.68323f); s += p[0] + p[1] + p[2]; 
    p = os_table.at(0.42702f, 0.937944f, 0.759144f); s += p[0] + p[1] + p[2]; 
    p = os_table.at(0.470751f, 1.03127f, 0.835057f); s += p[0] + p[1] + p[2]; 
    p = os_table.at(0.514669f, 1.1245f, 0.910971f); s += p[0] + p[1] + p[2]; 
    p = os_table.at(0.558773f, 1.21765f, 0.986884f); s += p[0] + p[1] + p[2]; 
    p = os_table.at(0.603063f, 1.31071f, 1.0628f); s += p[0] + p[1] + p[2]; 
    p = os_table.at(0.647539f, 1.40368f, 1.13871f); s += p[0] + p[1] + p[2]; 
    p = os_table.at(0.6922f, 1.49656f, 1.21462f); s += p[0] + p[1] + p[2]; 
    p = os_table.at(0.737048f, 1.58936f, 1.29054f); s += p[0] + p[1] + p[2]; 
    p = os_table.at(0.78208f, 1.68206f, 1.36645f); s += p[0] + p[1] + p[2]; 
    p = os_table.at(0.827298f, 1.77468f, 1.44236f); s += p[0] + p[1] + p[2]; 
    p = os_table.at(0.872701f, 1.8672f, 1.51827f); s += p[0] + p[1] + p[2]; 
    p = os_table.at(0.918289f, 1.95963f, 1.59419f); s += p[0] + p[1] + p[2]; 
    p = os_table.at(0.964061f, 2.05198f, 1.6701f); s += p[0] + p[1] + p[2]; 
    p = os_table.at(1.01002f, 2.14423f, 1.74601f); s += p[0] + p[1] + p[2]; 
    p = os_table.at(1.05616f, 2.23639f, 1.82192f); s += p[0] + p[1] + p[2]; 
    p = os_table.at(1.10248f, 2.32845f, 1.89783f); s += p[0] + p[1] + p[2]; 
    p = os_table.at(1.14899f, 2.42043f, 1.97375f); s += p[0] + p[1] + p[2]; 
    p = os_table.at(1.19569f, 2.51231f, 2.04966f); s += p[0] + p[1] + p[2]; 
    p = os_table.at(1.24256f, 2.6041f, 2.12557f); s += p[0] + p[1] + p[2]; 
    p = os_table.at(1.28962f, 2.69579f, 2.20148f); s += p[0] + p[1] + p[2]; 
    p = os_table.at(1.33687f, 2.7874f, 2.27739f); s += p[0] + p[1] + p[2]; 
    p = os_table.at(1.38429f, 2.8789f, 2.3533f); s += p[0] + p[1] + p[2]; 
    p = os_table.at(1.4319f, 2.97032f, 2.42921f); s += p[0] + p[1] + p[2]; 
    p = os_table.at(1.47969f, 3.06163f, 2.50512f); s += p[0] + p[1] + p[2]; 
    p = os_table.at(1.52767f, 3.15286f, 2.58103f); s += p[0] + p[1] + p[2]; 
    p = os_table.at(1.57582f, 3.24398f, 2.65694f); s += p[0] + p[1] + p[2]; 
    p = os_table.at(1.62416f, 3.33501f, 2.73285f); s += p[0] + p[1] + p[2]; 
    p = os_table.at(1.67268f, 3.42595f, 2.80876f); s += p[0] + p[1] + p[2]; 
    p = os_table.at(1.72138f, 3.51678f, 2.88467f); s += p[0] + p[1] + p[2]; 
    p = os_table.at(1.77027f, 3.60752f, 2.96058f); s += p[0] + p[1] + p[2]; 
    p = os_table.at(1.81933f, 3.69817f, 3.03649f); s += p[0] + p[1] + p[2]; 
    p = os_table.at(1.86858f, 3.78871f, 3.1124f); s += p[0] + p[1] + p[2]; 
    p = os_table.at(1.918f, 3.87916f, 3.18831f); s += p[0] + p[1] + p[2]; 
    p = os_table.at(1.96761f, 3.9695f, 3.26422f); s += p[0] + p[1] + p[2]; 
    p = os_table.at(2.0174f, 4.05975f, 3.34013f); s += p[0] + p[1] + p[2]; 
    p = os_table.at(2.06737f, 4.1499f, 3.41604f); s += p[0] + p[1] + p[2]; 
    p = os_table.at(2.11752f, 4.23995f, 3.49195f); s += p[0] + p[1] + p[2]; 
    p = os_table.at(2.16784f, 4.3299f, 3.56786f); s += p[0] + p[1] + p[2]; 
    p = os_table.at(2.21835f, 4.41975f, 3.64377f); s += p[0] + p[1] + p[2]; 
    p = os_table.at(2.26904f, 4.5095f, 3.71967f); s += p[0] + p[1] + p[2]; 
    p = os_table.at(2.31991f, 4.59915f, 3.79558f); s += p[0] + p[1] + p[2]; 
    p = os_table.at(2.37095f, 4.68869f, 3.87149f); s += p[0] + p[1] + p[2]; 
    p = os_table.at(2.42218f, 4.77814f, 3.9474f); s += p[0] + p[1] + p[2]; 
    p = os_table.at(2.47358f, 4.86748f, 4.02331f); s += p[0] + p[1] + p[2]; 
    p = os_table.at(2.52516f, 4.95672f, 4.09921f); s += p[0] + p[1] + p[2]; 
    p = os_table.at(2.57692f, 5.04585f, 4.17512f); s += p[0] + p[1] + p[2]; 
    p = os_table.at(2.62886f, 5.13488f, 4.25103f); s += p[0] + p[1] + p[2];
  }
  chrono.ElapsedTime();
  std::cout << "pattern_test2: (" << s << ") in " << chrono.Str() << "\n";
}


void pattern_test_strided()
{
  os_table_t<os_acts_field> os_table{};

  float s = 0;
  pt3D<float> p;

  std::size_t iter_nb = 10000000;

  bench::chrono_t chrono;
  chrono.Init();

  for (std::size_t i = 0; i < iter_nb; ++i) {
    p = os_table.at_strided(100UL,100UL,150UL); s += p[0] + p[1] + p[2]; 
    p = os_table.at_strided(100UL,100UL,151UL); s += p[0] + p[1] + p[2]; 
    p = os_table.at_strided(100UL,101UL,150UL); s += p[0] + p[1] + p[2]; 
    p = os_table.at_strided(100UL,101UL,151UL); s += p[0] + p[1] + p[2]; 
    p = os_table.at_strided(101UL,100UL,150UL); s += p[0] + p[1] + p[2]; 
    p = os_table.at_strided(101UL,100UL,151UL); s += p[0] + p[1] + p[2]; 
    p = os_table.at_strided(101UL,101UL,150UL); s += p[0] + p[1] + p[2]; 
    p = os_table.at_strided(101UL,101UL,151UL); s += p[0] + p[1] + p[2]; 
    p = os_table.at_strided(100UL,100UL,150UL); s += p[0] + p[1] + p[2]; 
    p = os_table.at_strided(100UL,100UL,151UL); s += p[0] + p[1] + p[2]; 
    p = os_table.at_strided(100UL,101UL,150UL); s += p[0] + p[1] + p[2]; 
    p = os_table.at_strided(100UL,101UL,151UL); s += p[0] + p[1] + p[2]; 
    p = os_table.at_strided(101UL,100UL,150UL); s += p[0] + p[1] + p[2]; 
    p = os_table.at_strided(101UL,100UL,151UL); s += p[0] + p[1] + p[2]; 
    p = os_table.at_strided(101UL,101UL,150UL); s += p[0] + p[1] + p[2]; 
    p = os_table.at_strided(101UL,101UL,151UL); s += p[0] + p[1] + p[2]; 
    p = os_table.at_strided(100UL,100UL,150UL); s += p[0] + p[1] + p[2]; 
    p = os_table.at_strided(100UL,100UL,151UL); s += p[0] + p[1] + p[2]; 
    p = os_table.at_strided(100UL,101UL,150UL); s += p[0] + p[1] + p[2]; 
    p = os_table.at_strided(100UL,101UL,151UL); s += p[0] + p[1] + p[2]; 
    p = os_table.at_strided(101UL,100UL,150UL); s += p[0] + p[1] + p[2]; 
    p = os_table.at_strided(101UL,100UL,151UL); s += p[0] + p[1] + p[2]; 
    p = os_table.at_strided(101UL,101UL,150UL); s += p[0] + p[1] + p[2]; 
    p = os_table.at_strided(101UL,101UL,151UL); s += p[0] + p[1] + p[2]; 
    p = os_table.at_strided(100UL,100UL,150UL); s += p[0] + p[1] + p[2]; 
    p = os_table.at_strided(100UL,100UL,151UL); s += p[0] + p[1] + p[2]; 
    p = os_table.at_strided(100UL,101UL,150UL); s += p[0] + p[1] + p[2]; 
    p = os_table.at_strided(100UL,101UL,151UL); s += p[0] + p[1] + p[2]; 
    p = os_table.at_strided(101UL,100UL,150UL); s += p[0] + p[1] + p[2]; 
    p = os_table.at_strided(101UL,100UL,151UL); s += p[0] + p[1] + p[2]; 
    p = os_table.at_strided(101UL,101UL,150UL); s += p[0] + p[1] + p[2]; 
    p = os_table.at_strided(101UL,101UL,151UL); s += p[0] + p[1] + p[2]; 
    p = os_table.at_strided(100UL,100UL,150UL); s += p[0] + p[1] + p[2]; 
    p = os_table.at_strided(100UL,100UL,151UL); s += p[0] + p[1] + p[2]; 
    p = os_table.at_strided(100UL,101UL,150UL); s += p[0] + p[1] + p[2]; 
    p = os_table.at_strided(100UL,101UL,151UL); s += p[0] + p[1] + p[2]; 
    p = os_table.at_strided(101UL,100UL,150UL); s += p[0] + p[1] + p[2]; 
    p = os_table.at_strided(101UL,100UL,151UL); s += p[0] + p[1] + p[2]; 
    p = os_table.at_strided(101UL,101UL,150UL); s += p[0] + p[1] + p[2]; 
    p = os_table.at_strided(101UL,101UL,151UL); s += p[0] + p[1] + p[2]; 
    p = os_table.at_strided(100UL,100UL,150UL); s += p[0] + p[1] + p[2]; 
    p = os_table.at_strided(100UL,100UL,151UL); s += p[0] + p[1] + p[2]; 
    p = os_table.at_strided(100UL,101UL,150UL); s += p[0] + p[1] + p[2]; 
    p = os_table.at_strided(100UL,101UL,151UL); s += p[0] + p[1] + p[2]; 
    p = os_table.at_strided(101UL,100UL,150UL); s += p[0] + p[1] + p[2]; 
    p = os_table.at_strided(101UL,100UL,151UL); s += p[0] + p[1] + p[2]; 
    p = os_table.at_strided(101UL,101UL,150UL); s += p[0] + p[1] + p[2]; 
    p = os_table.at_strided(101UL,101UL,151UL); s += p[0] + p[1] + p[2]; 
    p = os_table.at_strided(100UL,100UL,150UL); s += p[0] + p[1] + p[2]; 
    p = os_table.at_strided(100UL,100UL,151UL); s += p[0] + p[1] + p[2]; 
    p = os_table.at_strided(100UL,101UL,150UL); s += p[0] + p[1] + p[2]; 
    p = os_table.at_strided(100UL,101UL,151UL); s += p[0] + p[1] + p[2]; 
    p = os_table.at_strided(101UL,100UL,150UL); s += p[0] + p[1] + p[2]; 
    p = os_table.at_strided(101UL,100UL,151UL); s += p[0] + p[1] + p[2]; 
    p = os_table.at_strided(101UL,101UL,150UL); s += p[0] + p[1] + p[2]; 
    p = os_table.at_strided(101UL,101UL,151UL); s += p[0] + p[1] + p[2]; 
    p = os_table.at_strided(100UL,100UL,150UL); s += p[0] + p[1] + p[2]; 
    p = os_table.at_strided(101UL,100UL,150UL); s += p[0] + p[1] + p[2]; 
    p = os_table.at_strided(100UL,99UL,149UL); s += p[0] + p[1] + p[2]; 
    p = os_table.at_strided(100UL,99UL,150UL); s += p[0] + p[1] + p[2]; 
    p = os_table.at_strided(100UL,100UL,149UL); s += p[0] + p[1] + p[2]; 
    p = os_table.at_strided(100UL,100UL,150UL); s += p[0] + p[1] + p[2]; 
    p = os_table.at_strided(101UL,99UL,149UL); s += p[0] + p[1] + p[2]; 
    p = os_table.at_strided(101UL,99UL,150UL); s += p[0] + p[1] + p[2]; 
    p = os_table.at_strided(101UL,100UL,149UL); s += p[0] + p[1] + p[2]; 
    p = os_table.at_strided(101UL,100UL,150UL); s += p[0] + p[1] + p[2]; 
    p = os_table.at_strided(100UL,99UL,149UL); s += p[0] + p[1] + p[2]; 
    p = os_table.at_strided(100UL,99UL,150UL); s += p[0] + p[1] + p[2]; 
    p = os_table.at_strided(100UL,100UL,149UL); s += p[0] + p[1] + p[2]; 
    p = os_table.at_strided(100UL,100UL,150UL); s += p[0] + p[1] + p[2]; 
    p = os_table.at_strided(101UL,99UL,149UL); s += p[0] + p[1] + p[2]; 
    p = os_table.at_strided(101UL,99UL,150UL); s += p[0] + p[1] + p[2]; 
    p = os_table.at_strided(101UL,100UL,149UL); s += p[0] + p[1] + p[2]; 
    p = os_table.at_strided(101UL,100UL,150UL); s += p[0] + p[1] + p[2]; 
    p = os_table.at_strided(100UL,99UL,149UL); s += p[0] + p[1] + p[2]; 
    p = os_table.at_strided(100UL,99UL,150UL); s += p[0] + p[1] + p[2]; 
    p = os_table.at_strided(100UL,100UL,149UL); s += p[0] + p[1] + p[2]; 
    p = os_table.at_strided(100UL,100UL,150UL); s += p[0] + p[1] + p[2]; 
    p = os_table.at_strided(101UL,99UL,149UL); s += p[0] + p[1] + p[2]; 
    p = os_table.at_strided(101UL,99UL,150UL); s += p[0] + p[1] + p[2]; 
    p = os_table.at_strided(101UL,100UL,149UL); s += p[0] + p[1] + p[2]; 
    p = os_table.at_strided(101UL,100UL,150UL); s += p[0] + p[1] + p[2]; 
    p = os_table.at_strided(100UL,99UL,149UL); s += p[0] + p[1] + p[2]; 
    p = os_table.at_strided(100UL,99UL,150UL); s += p[0] + p[1] + p[2]; 
    p = os_table.at_strided(100UL,100UL,149UL); s += p[0] + p[1] + p[2]; 
    p = os_table.at_strided(100UL,100UL,150UL); s += p[0] + p[1] + p[2]; 
    p = os_table.at_strided(101UL,99UL,149UL); s += p[0] + p[1] + p[2]; 
    p = os_table.at_strided(101UL,99UL,150UL); s += p[0] + p[1] + p[2]; 
    p = os_table.at_strided(101UL,100UL,149UL); s += p[0] + p[1] + p[2]; 
    p = os_table.at_strided(101UL,100UL,150UL); s += p[0] + p[1] + p[2]; 
    p = os_table.at_strided(100UL,99UL,149UL); s += p[0] + p[1] + p[2]; 
    p = os_table.at_strided(100UL,99UL,150UL); s += p[0] + p[1] + p[2]; 
    p = os_table.at_strided(100UL,100UL,149UL); s += p[0] + p[1] + p[2]; 
    p = os_table.at_strided(100UL,100UL,150UL); s += p[0] + p[1] + p[2]; 
    p = os_table.at_strided(101UL,99UL,149UL); s += p[0] + p[1] + p[2]; 
    p = os_table.at_strided(101UL,99UL,150UL); s += p[0] + p[1] + p[2]; 
    p = os_table.at_strided(101UL,100UL,149UL); s += p[0] + p[1] + p[2]; 
    p = os_table.at_strided(101UL,100UL,150UL); s += p[0] + p[1] + p[2]; 
  }
  chrono.ElapsedTime();
  std::cout << "pattern_test_strided: (" << s << ") in " << chrono.Str() << "\n";
}














int main()
{
  // std::cout << "Miaou.\n";
  pattern_test2();
  pattern_test_strided();
  return 0;
}