#!/usr/bin/python

# Hello world python program

# Lancer le script : python3 ./2022-02-08_sparseccl.py

import argparse
import matplotlib.pyplot as plt
import numpy as np
import statistics as stat
import sys
import math
import os

sys.path.append('../common/plot/')
import plot_utils as pu

# Plots diff between Lorentz standalone and Kiwaku, omitting Covfie

parser = argparse.ArgumentParser()

parser.add_argument("-algorithm", "-A", help="Chooses which algorithm to plot: \"lorentz_opti\" or \"slice\"")
parser.add_argument("-field", "-F", help="Chooses which field to plot: \"acts\" or \"constant\"")
parser.add_argument("-computer", "-CPT", help="Sets the computer name used for the benchmark output file (default: \"generic\")")
parser.add_argument("-timer", "-T", help="Sets the time measurment method used for the benchmark output file: \"sclock\" for system clock timer or \"nanobench\" for the nanobench measurments.")
parser.add_argument("-compare", "-CMP", help="Plot strategy. \"relative\": plots by comparing the Kiwaku curve with the standalone. \"absolute\": plots absolute time/cycles values.")

parser.add_argument("-particles", "-LP", help="For Lorentz-Euler algorithm only. Sets the range for particles, as written on the output benchmark file. Format: \"min-max\" or \"value\" if (min==max). Example: \"512-8192\" or \"512\"")
parser.add_argument("-steps", "-LS", help="For Lorentz-Euler algorithm only. Sets the range for steps (number of move iterations for each particle), as written on the output benchmark file. Format: \"min-max\" or \"value\" if (min==max). Example: \"512-8192\" or \"512\"")
parser.add_argument("-imom", "-LM", help="For Lorentz-Euler algorithm only. Sets the range for imom (initial cinetic momentum for each particle), as written on the output benchmark file. Format: \"min-max\" or \"value\" if (min==max). Example: \"512-8192\" or \"512\"")

args = parser.parse_args()

# print(f"Args: {args}\nCommand Line: {sys.argv}\nfoo: {args.foo}")
# print(f"Dict format: {vars(args)}")


VERSION_ATTENDUE = 2

check_value_tolerance = 5

use_acts_field = True

plot_diff = True # Plot values relative to standalone, or plot absolute values

per_particle_values = True # Divide the values by the number of particles and steps 

input_directory  = "output_bench/"
output_directory = "output_plot/"

const_algo_name_lorentz = "lorentz"
const_algo_name_slice   = "slice"

# cn = const name
cn_field_acts     = "acts"
cn_field_constant = "constant"


file_standalone = ""
file_kiwaku     = ""
timer_config    = "sclock" # system clock
algorithm_name  = "lorentz"
iteration_count = "it10"
computer_name   = "generic" # generic # aussi utilisé pour le nom du fichier d'entrée 
field_name      = "acts-field" # constant-field

if args.computer != None:
    computer_name = args.computer
    print("Benchmark computer name set to \"" + computer_name + "\"")


def check_parameter_validity(param, accepted_params):
  if param in accepted_params:
    return True
  else:
    return False


# if args.mode != None:

if args.algorithm not in [const_algo_name_lorentz, const_algo_name_slice]:
  print("ERROR: please provide a valid algorithm name with \"-algorithm lorentz_opti\" or \"-algorithm slice\"")
  exit()
# if args.algorithm == "lorentz":
#   algorithm_name = "lorentz_opti"
# else:
algorithm_name = args.algorithm


if (args.field != None):
  if args.field not in ["constant", "acts"]:
    print("ERROR: please provide a valid field name with \"-field acts\" or \"-field constant\"")
    exit()
  field_name = args.field + "-field"
  print("Benchmark field set to: " + field_name)

if (args.timer != None):
  if args.timer not in ["sclock", "nanobench"]:
    print("ERROR: please provide a valid timer name with \"-timer sclock\" or \"-timer nanobench\"")
    exit()
  timer_config = args.timer
  print("Benchmark timer set to: " + timer_config)

if (args.compare != None):
  if args.compare not in ["relative", "absolute"]:
    print("ERROR: please provide a valid comparison method with \"-compare relative\" or \"-compare absolute\"")
    exit()
  plot_diff = (args.compare == "relative")
  print("Benchmark comparison method set to: " + args.compare)


def is_lorentz_euler_algorithm():
  global algorithm_name, const_algo_name_lorentz
  return algorithm_name == const_algo_name_lorentz

def is_slice_algorithm():
  global algorithm_name, const_algo_name_slice
  return algorithm_name == const_algo_name_slice

# if args.mode == "default":
#   print("--- mode = default, will set all parameters to their default values.")
# else:
#   print("--- mode = custom, please set all the required parameters.")
#   # TODO



str_min_iteration_count = "10" # added to plot title only



# ============= Gestion de la taille
image_dpi = 96

image_ratio = 11/6 # =880/480 

image_width = 1280
image_height = image_width / image_ratio #(image_width / 640) * 480

image_scale_factor = image_width / 640
line_width = image_scale_factor * 1.5

plt.figure(figsize=(image_width/image_dpi, image_height/image_dpi) , dpi=image_dpi)

FONT_SIZE   = 10 * image_scale_factor
TITLE_SIZE  = 12 * image_scale_factor

#plt.rc('font', size=FONT_SIZE)          # controls default text sizes
plt.rc('axes', titlesize=TITLE_SIZE)   # fontsize of the axes title
plt.rc('axes', labelsize=FONT_SIZE)      # fontsize of the x and y labels
plt.rc('xtick', labelsize=FONT_SIZE)     # fontsize of the tick labels
plt.rc('ytick', labelsize=FONT_SIZE)     # fontsize of the tick labels
plt.rc('legend', fontsize=FONT_SIZE)     # legend fontsize
#plt.rc('figure', titlesize=FONT_SIZE)   # fontsize of the figure title
# plt.rcParams.update({'font.size': FONT_SIZE})



if is_lorentz_euler_algorithm():
  VERSION_ATTENDUE = 2

  particles_n     = "512-8192"
  steps_n         = "1024"
  imom_n          = "128"

  if args.particles != None:
    particles_n = args.particles
    print("Lorentz-Euler: particles range set to \"" + particles_n + "\"")

  if args.steps != None:
    steps_n = args.steps
    print("Lorentz-Euler: steps range set to \"" + steps_n + "\"")

  if args.imom != None:
    imom_n = args.imom
    print("Lorentz-Euler: imom range set to \"" + imom_n + "\"")

  file_prefix     = algorithm_name + "_" + timer_config + "_" + iteration_count
  file_prefix    += "_" + particles_n + "_" + steps_n + "_" + imom_n + "_" + field_name
  file_suffix     = computer_name

  file_standalone = input_directory + file_prefix + "_standalone_" + file_suffix + ".bench.txt"
  file_kiwaku     = input_directory + file_prefix + "_kiwaku_" + file_suffix + ".bench.txt"

  file_out_png = output_directory + file_prefix + "_" + file_suffix
  if plot_diff:
    file_out_png += "_relative"
  else:
    file_out_png += "_absolute"
  
  file_out_png += ".bench.png" 

  # def print_parameters():
  print("\nLorentz-Euler: current paramaters:")
  print(" -algorithm: " + algorithm_name)
  print("     -field: " + field_name)
  print("  -computer: " + computer_name)
  print("     -timer: " + timer_config)
  print(" -particles: " + particles_n)
  print("     -steps: " + steps_n)
  print("      -imom: " + imom_n)
  print()
  print("Associated file names:")
  print("          Kiwaku: " + file_kiwaku)
  print("      Standalone: " + file_standalone)
  print("    Out plot img: " + file_out_png)
  print()

  plot_cycles_instead_of_time = (timer_config == "nanobench")


    # parser.add_argument("-algorithm", "-A", help="Chooses which algorithm to plot: \"lorentz_opti\" or \"slice\"")
    # parser.add_argument("-field", "-F", help="Chooses which field to plot: \"acts\" or \"constant\"")
    # parser.add_argument("-computer", "-C", help="Sets the computer name used for the benchmark outpit file (default: \"generic\")")
    # parser.add_argument("-mode", help="advanced or default mode")

    # parser.add_argument("-particles", "-LP", help="For Lorentz-Euler algorithm only. Sets the range for particles, as written on the output benchmark file. Format: \"min-max\" or \"value\" if (min==max). Example: \"512-8192\" or \"512\"")
    # parser.add_argument("-steps", "-LS", help="For Lorentz-Euler algorithm only. Sets the range for steps (number of move iterations for each particle), as written on the output benchmark file. Format: \"min-max\" or \"value\" if (min==max). Example: \"512-8192\" or \"512\"")
    # parser.add_argument("-imom", "-LM", help="For Lorentz-Euler algorithm only. Sets the range for imom (initial cinetic momentum for each particle), as written on the output benchmark file. Format: \"min-max\" or \"value\" if (min==max). Example: \"512-8192\" or \"512\"")



  if not os.path.exists(file_standalone):
    print("ERROR Lorentz-Euler: standalone version file not found:")
    print("    \"" + file_standalone + "\"")
    print("  " + "Maybe the parameters \"particle\",  \"steps\" or \"imom\" do not match the ones used in your C++ benchmark?")
    exit()

  if not os.path.exists(file_kiwaku):
    print("ERROR Lorentz-Euler: kiwaku version file not found:")
    print("    \"" + file_kiwaku + "\"")
    print("  " + "Maybe the parameters \"particle\", \"steps\" or \"imom\" do not match the ones used in your C++ benchmark?")
    exit()


  def lorentz_load_file(path):
    global VERSION_ATTENDUE
    bench_list = []

    with open(path) as fp:
      version = fp.readline() # version du fichier actuel (doit être 106 et non plus 105)
      # print("File version : {}".format(version))

      if (int(version) != VERSION_ATTENDUE):
        sys.exit("ERREUR, VERSION DU FICHIER NON COMPATIBLE : " + str(int(version)) + ".  VERSION ATTENDUE = " + str(VERSION_ATTENDUE))

      line = fp.readline()

      while line:
        words = pu.remove_newline(line.split(" "))
        
        # v2: (first line for benchmark info, second line for timers)
        # particles | steps | imom | repeat_count | check_string
        # elapsed_time1 | ... | elapsed_timeN

        header = {} # empty dictionnary
        header["particles"]     = int(words[0])
        header["steps"]         = int(words[1])
        header["imom"]          = int(words[2])
        header["repeat_count"]  = int(words[3])
        header["check_string"]  = words[4]
        header["ptotal"]        = header["particles"] * header["steps"]

        divide_factor = 1
        if per_particle_values:
          if plot_cycles_instead_of_time:
            divide_factor = header["ptotal"]
          else:
            divide_factor = header["ptotal"] / 1000
        else:
          if plot_cycles_instead_of_time:
            divide_factor = 1e9
          else:
            divide_factor = 1e6

        times_int   = pu.filter_outliers(pu.list_str_to_int(pu.remove_newline(fp.readline().split(" ")), divide_factor))
        cycles_int  = pu.filter_outliers(pu.list_str_to_int(pu.remove_newline(fp.readline().split(" ")), divide_factor))

        header["times_l"]   = times_int
        header["cycles_l"]  = cycles_int

        header["elapsed_time"]  = stat.median(times_int)
        header["cycle"]         = stat.median(cycles_int)

        if timer_config == "nanobench":
          zeroes_everywhere = True
          for e in cycles_int:
            if e != 0:
              zeroes_everywhere = False
              break
          if zeroes_everywhere:
            print("INPUT ERROR: (every cpu_cycle) == 0, when using Nanobench, you need to run the benchmark with admin rights: sudo -E ./lorentz-euler_....exe")
            print("             for file: " + path)
            exit(-1)
        

        # print(header["times_l"])

        bench_list.append(header)
    
        # Reading next benchmark header
        line = fp.readline()

    return bench_list

  standalone_list = lorentz_load_file(file_standalone)
  kiwaku_list     = lorentz_load_file(file_kiwaku)
  # TEMPORARILY DISABLE ERROR CHECKING
  # TODO: understand why such a difference
  # pu.check_same_results(standalone_list, kiwaku_list, "check_string", 0)

  # TEMPORARILY DISABLE ERROR CHECKING
  # pu.check_same_results(standalone_list, kiwaku_list, "check_value", check_value_tolerance)
  # Error with kiwaku_list, so it seems.
  # pu.check_same_results(standalone_list, lorentz_covfie_list)

  # external_1D = make_1D_list(lorentz_external_list, "elapsed_time")

  # ptotal_1D   = pu.make_1D_list_every_1line(standalone_list, "particles", "steps", 1)
  ptotal_1D   = pu.make_1D_list_every_1line(standalone_list, "particles", 1)


  compare_to_list = None
  if plot_diff:
    compare_to_list = standalone_list

  if plot_cycles_instead_of_time:
    violin_keyword = "cycles_l"
    plot_keyword = "cycle"
  else:
    violin_keyword = "times_l"
    plot_keyword = "elapsed_time"

  # if plot_diff:
  pu.draw_violin_plot("grey", pu.make_violin_plot_list(standalone_list, violin_keyword, compare_to_list))
  pu.draw_violin_plot("blue", pu.make_violin_plot_list(kiwaku_list, violin_keyword, compare_to_list))


  if plot_diff:
    ldiff0 = pu.make_diff_list(compare_to_list, standalone_list, plot_keyword)
    ldiff2 = pu.make_diff_list(compare_to_list, kiwaku_list, plot_keyword)
  else:
    ldiff0 = pu.make_absolute_list(standalone_list, plot_keyword)
    ldiff2 = pu.make_absolute_list(kiwaku_list, plot_keyword)

  #  (opti matrix)
  plt.plot  (range(1, len(ldiff0)+1), ldiff0, color="grey", label="Standalone", linestyle="dashdot", linewidth=line_width)
  plt.plot  (range(1, len(ldiff2)+1), ldiff2, color="blue", label="Kiwaku", linestyle="solid", linewidth=line_width)

  max_y_value = max([max(ldiff0), max(ldiff2)])


  plt.rcParams['grid.linestyle'] = "-"
  plt.rcParams['grid.alpha'] = 0.15
  plt.rcParams['grid.color'] = "black" ##cccccc
  plt.grid(linewidth=line_width/20)

  plt.xticks(range(1, len(ldiff0)+1), ptotal_1D)

  unit_name = ""
  if plot_diff:
    unit_name = "relative "
  else:
    unit_name = ""

  if plot_cycles_instead_of_time:
    unit_name += "cpu cycles"
  else:
    unit_name += "time"

  pdiff_str = ""
  if plot_diff:
    pdiff_str = "diff"
  else:
    pdiff_str = "absolute"

  if is_lorentz_euler_algorithm():

    plt.title("Lorentz-Euler - " + computer_name + " - " + field_name + " - " + unit_name + " - iter/pt: " + str_min_iteration_count)

    unit_name_file = unit_name.replace(" ", "_")

    if per_particle_values:
      unit_name_file += "_per-particle"
    
    # out_fname = "2023-05-01_" + computer_name + "_" + field_name + "_" + unit_name_file + "_" + pdiff_str + ".bench.png"

    # plt.ylabel('Elapsed time (µs)')
    if plot_diff:
      plt.ylabel('Relative performance (%), lower is better')
    else:
      if per_particle_values:
        if plot_cycles_instead_of_time:
          plt.ylabel('CPU cycles per (particle*steps)')
        else:
          plt.ylabel('Elapsed time (nanoseconds) per (particle*steps)')
      else:
        if plot_cycles_instead_of_time:
          plt.ylabel('CPU cycles in billions (1e9)')
        else:
          plt.ylabel('Elapsed time in seconds')

    plt.xlabel('Number of particles')



  # Ascending number of particles (and imom), in millions (10^6)
  #plt.ylim([-5, 100])
  plt.legend()
  # global_drawn_x_variables_number+1
  # plt.xticks(range(1, 6), x_list_curve_drawn) # = x_list_shared et x_list_acc

  # TODO
  plt.ylim([0, max_y_value * 1.15])

  print
  plt.savefig(file_out_png, format='png') #, dpi=image_dpi)

  plt.show()




if is_slice_algorithm():
  VERSION_ATTENDUE = 1

  def slice_load_file(path):
    global VERSION_ATTENDUE
    bench_list = []

    with open(path) as fp:
      version = fp.readline() # version du fichier actuel (doit être 106 et non plus 105)
      print("Version du fichier : {}".format(version))

      if (int(version) != VERSION_ATTENDUE):
        sys.exit("ERREUR, VERSION DU FICHIER NON COMPATIBLE : " + str(int(version)) + ".  VERSION ATTENDUE = " + str(VERSION_ATTENDUE))

      line = fp.readline()

      while line:
        words = line.split(" ")
        words[len(words)-1] = words[len(words)-1].rstrip("\n")
        # words.remove('\n')
        # print(words)

        # Autant de fois qu'il y a d'évènements (nouvelle ligne) :
        # z_value | elapsed_time_us | check_value
        header = {} # dictionnaire vide
        header["z_value"]         = int   (words[0])
        header["elapsed_time_us"] = float (words[1]) # math.log
        header["check_value"]     = int   (words[2])
        bench_list.append(header)
        # print(header)
    
        # Lecture de la prochaine ligne
        line = fp.readline()

    return bench_list

  # standalone_list = slice_load_file(file_standalone)
  # kiwaku_list     = slice_load_file(file_kiwaku)
  # pu.check_same_results(standalone_list, kiwaku_list, "check_value", check_value_tolerance)

  # TODO: slice a constant field? (maybe not very interesting?)

  acts_kiwaku_inline_list   = slice_load_file("slice/plot/data3/slice_kiwaku_inline_blop-debian11_step1000.txt")
  acts_kiwaku_noinline_list = slice_load_file("slice/plot/data3/slice_kiwaku_noinline_blop-debian11_step1000.txt")
  acts_standalone_list      = slice_load_file("slice/plot/data3/slice_standalone_opti_blop-debian11_step1000.txt")


  # ERROR CHECKING (with tolerance)
  pu.check_same_results(acts_standalone_list, acts_standalone_list, "check_value", check_value_tolerance)
  pu.check_same_results(acts_standalone_list, acts_kiwaku_inline_list, "check_value", check_value_tolerance)
  pu.check_same_results(acts_standalone_list, acts_kiwaku_noinline_list, "check_value", check_value_tolerance)

  ldiff0 = pu.make_diff_list(acts_standalone_list, acts_standalone_list, "elapsed_time_us")
  ldiff1 = pu.make_diff_list(acts_standalone_list, acts_kiwaku_inline_list, "elapsed_time_us")
  ldiff2 = pu.make_diff_list(acts_standalone_list, acts_kiwaku_noinline_list, "elapsed_time_us")

  def make_1D_ticks(ticks_number):
    res = []
    ind = 0
    every = int(ticks_number / 6)
    for i in range(1, ticks_number + 1):
      if (ind % every == 0):
        res.append(str(i))
      else:
        res.append("")
      ind += 1
    return res

  # ptotal_1D   = make_1D_ticks(len(acts_covfie_list))
  ptotal_1D   = pu.make_1D_list_every_auto(acts_standalone_list, "z_value")

  # range(1, len(ldiff)+1) #make_1D_list_every(lorentz_covfie_list, "ptotal", 3)

  plt.plot  (range(1, len(ldiff0)+1), ldiff0, color="grey", label="standalone", linestyle="dashdot", linewidth=line_width)
  plt.plot  (range(1, len(ldiff1)+1), ldiff1, color="blue", label="kiwaku", linestyle="solid", linewidth=line_width)
  plt.plot  (range(1, len(ldiff2)+1), ldiff2, color="maroon", label="kiwaku not inline", linestyle="dotted", linewidth=line_width)

  # plt.plot  (range(1, len(ldiff4)+1), ldiff4, color="red", label="standalone v3", linestyle="solid", linewidth=line_width)

  # plt.ylim([0, 100])

  plt.rcParams['grid.linestyle'] = "-"
  plt.rcParams['grid.alpha'] = 0.15
  plt.rcParams['grid.color'] = "black" ##cccccc
  plt.grid(linewidth=line_width/20)

  plt.xticks(range(1, len(ldiff1)+1), ptotal_1D)


  plt.title(computer_name + " - ACTS slicing - Relative performance (%) between Kiwaku (inline) and:")


  # plt.ylabel('Elapsed time (µs)')
  plt.ylabel('Relative duration (%), lower is better')
  plt.xlabel('Z-value')
  plt.legend()

  plt.ylim([0, 110]) # TODO: check max value in each curve and add 10

  output_image_name = "slice_" + computer_name + "_" + "ACTS_magnetic_field" + ".bench.png"

  plt.savefig(output_image_name, format='png') #, dpi=my_dpi)

  plt.show()


print ("Hello World!")
