#!/usr/bin/python

# Hello world python program

# Lancer le script : python3 ./2022-02-08_sparseccl.py

import argparse
import matplotlib.pyplot as plt
import numpy as np
import statistics as stat
import sys
import math

sys.path.append('../common/plot/')
import plot_utils as pu

# Plots diff between Lorentz standalone and Kiwaku, omitting Covfie

parser = argparse.ArgumentParser()

parser.add_argument("-bar", help="Do the bar option")
parser.add_argument("-foo", help="Foo the program")
parser.add_argument("-mode", help="advanced or default mode")

args = parser.parse_args()

print(f"Args: {args}\nCommand Line: {sys.argv}\nfoo: {args.foo}")
print(f"Dict format: {vars(args)}")


VERSION_ATTENDUE = 2

use_acts_field = True

plot_diff = True # Plot values relative to standalone, or plot absolute values

plot_cycles_instead_of_time = False 

per_particle_values = True # Divide the values by the number of particles and steps 

input_path = "bench_output/"

const_algo_name_lorentz = "lorentz_opti"
const_algo_name_slice   = "slice"

file_standalone = ""
file_kiwaku     = ""
clock_config    = "sclock" # system clock
algorithm_name  = "lorentz_opti"
iteration_count = "it10"
computer_name   = "noobuntublue" # generic # aussi utilisé pour le nom du fichier d'entrée 
particles_n     = "512-8192"
steps_n         = "1024"
imom_n          = "128"
field_name      = "acts-field" # constant-field

file_prefix     = algorithm_name + "_" + clock_config + "_" + iteration_count
file_suffix     = computer_name + "_" + particles_n + "_" + steps_n + "_" + imom_n + "_" + field_name + ".txt"

file_standalone = file_prefix + "_standalone_" + file_suffix
file_kiwaku     = file_prefix + "_kiwaku_" + file_suffix


def is_lorentz_euler_algorithm():
  return algorithm_name == const_algo_name_lorentz

def is_slice_algorithm():
  return algorithm_name == const_algo_name_slice

if args.mode == "default":
  print("--- mode = default, will set all parameters to their default values.")
else:
  print("--- mode = custom, please set all the required parameters.")
  # TODO

if is_lorentz_euler_algorithm():
  VERSION_ATTENDUE = 2

if is_slice_algorithm():
  VERSION_ATTENDUE = 1


str_min_iteration_count = "10" # added to plot title only




# ============= Gestion de la taille
image_dpi = 96

image_ratio = 11/6 # =880/480 


image_width = 1280
image_height = image_width / image_ratio #(image_width / 640) * 480

image_scale_factor = image_width / 640
line_width = image_scale_factor * 1.5

plt.figure(figsize=(image_width/image_dpi, image_height/image_dpi) , dpi=image_dpi)


MY_SIZE       = 10 * image_scale_factor
MY_SIZE_SMALL = 6  * image_scale_factor
TITLE_SIZE    = 12 * image_scale_factor

#plt.rc('font', size=MY_SIZE)          # controls default text sizes
plt.rc('axes', titlesize=TITLE_SIZE)     # fontsize of the axes title
plt.rc('axes', labelsize=MY_SIZE)     # fontsize of the x and y labels
plt.rc('xtick', labelsize=MY_SIZE)    # fontsize of the tick labels
plt.rc('ytick', labelsize=MY_SIZE)    # fontsize of the tick labels
plt.rc('legend', fontsize=MY_SIZE)    # legend fontsize
#plt.rc('figure', titlesize=MY_SIZE)   # fontsize of the figure title
# plt.rcParams.update({'font.size': MY_SIZE})

# Charge le fichier de bench "path" et retourne la liste de ce qui a été lu.
def lorentz_load_file(path):
  global VERSION_ATTENDUE
  bench_list = []

  with open(path) as fp:
    version = fp.readline() # version du fichier actuel (doit être 106 et non plus 105)
    print("Version du fichier : {}".format(version))

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
      

      # print(header["times_l"])

      bench_list.append(header)
  
      # Reading next benchmark header
      line = fp.readline()

  return bench_list


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



# if use_acts_field:
#   standalone_list = load_file("data/d10_thinkpad/lorentz_opti_sclock_it10_standalone_noobuntublue_512-8192_1024_128_acts-field.txt")
#   kiwaku_list     = load_file("data/d10_thinkpad/lorentz_opti_sclock_it10_kiwaku_noobuntublue_512-8192_1024_128_acts-field.txt")


if is_lorentz_euler_algorithm():
  standalone_list = lorentz_load_file(file_standalone)
  kiwaku_list     = lorentz_load_file(file_kiwaku)

if is_slice_algorithm():
  standalone_list = slice_load_file(file_standalone)
  kiwaku_list     = slice_load_file(file_kiwaku)


# TEMPORARILY DISABLE ERROR CHECKING
# pu.check_same_results(standalone_list, kiwaku_list)
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

# pu.draw_violin_plot("grey", pu.make_violin_plot_list(standalone_list, violin_keyword, compare_to_list))
# pu.draw_violin_plot("blue", pu.make_violin_plot_list(kiwaku_list, violin_keyword, compare_to_list))


if plot_diff:
  ldiff0 = pu.make_diff_list(compare_to_list, standalone_list, plot_keyword)
  ldiff2 = pu.make_diff_list(compare_to_list, kiwaku_list, plot_keyword)
else:
  ldiff0 = pu.make_absolute_list(standalone_list, plot_keyword)
  ldiff2 = pu.make_absolute_list(kiwaku_list, plot_keyword)

#  (opti matrix)
plt.plot  (range(1, len(ldiff0)+1), ldiff0, color="grey", label="Standalone", linestyle="dashdot", linewidth=line_width)
plt.plot  (range(1, len(ldiff2)+1), ldiff2, color="blue", label="Kiwaku", linestyle="solid", linewidth=line_width)


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
  
  out_fname = "2023-05-01_" + computer_name + "_" + field_name + "_" + unit_name_file + "_" + pdiff_str + ".png"

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

plt.ylim([0, 113])

plt.savefig(out_fname, format='png') #, dpi=image_dpi)

plt.show()
print ("Hello World!")
