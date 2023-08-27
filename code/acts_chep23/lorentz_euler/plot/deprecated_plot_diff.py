#!/usr/bin/python

# Hello world python program

# Lancer le script : python3 ./2022-02-08_sparseccl.py

import matplotlib.pyplot as plt
import numpy as np
import statistics as stat
import sys
import math
import plot_utils as pu


VERSION_ATTENDUE = 1

use_acts_field = True
plot_diff = True # Plot values relative to covfie, or plot absolute values
computer_name = "chaton" # aussi utilisé pour le nom du fichier d'entrée

# ============= Gestion de la taille
my_dpi = 96
# output_image_name = "no_name"
field_type_name = "" # InterpolateNN_LayoutStride

if use_acts_field:
  field_type_name = "acts-field"
else:
  field_type_name = "constant-field"

output_image_name = field_type_name # InterpolateNN_LayoutStride
output_image_ver  = "v1"


image_width = 1280
image_height = 0
image_scale_factor = image_width / 640
line_width = image_scale_factor * 1.5

image_height = (image_width / 640) * 480
plt.figure(figsize=(image_width/my_dpi, image_height/my_dpi) , dpi=my_dpi)
# output_image_name = "lorentz_compare_" + computer_name + "_" + output_image_name + ".png"
output_image_name = "lorentz_compare_" + computer_name + "_" + output_image_name + ".png"

MY_SIZE = (10 * image_scale_factor)
MY_SIZE_SMALL = (6 * image_scale_factor)
TITLE_SIZE = (12 * image_scale_factor)

#plt.rc('font', size=MY_SIZE)          # controls default text sizes
plt.rc('axes', titlesize=TITLE_SIZE)     # fontsize of the axes title
plt.rc('axes', labelsize=MY_SIZE)     # fontsize of the x and y labels
plt.rc('xtick', labelsize=MY_SIZE_SMALL)    # fontsize of the tick labels
plt.rc('ytick', labelsize=MY_SIZE)    # fontsize of the tick labels
plt.rc('legend', fontsize=MY_SIZE)    # legend fontsize
#plt.rc('figure', titlesize=MY_SIZE)   # fontsize of the figure title
# plt.rcParams.update({'font.size': MY_SIZE})

# fin gestion de la taille =============

# Ici, une courbe n'est qu'une suite toute basique de points

# lorentz_external_v2_fname = "build/lorentz-euler_v2.txt"

# Charge le fichier de bench "path" et retourne la liste de ce qui a été lu.
def load_file(path):
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
      # v1:
      # particles | steps | imom | repeat_count | elapsed_time | check_string
      
      # v2:
      # particles | steps | imom | repeat_count | check_string
      # elapsed_time1 | ... | elapsed_timeN

      header = {} # dictionnaire vide
      header["particles"]     = int(words[0])
      header["steps"]         = int(words[1])
      header["imom"]          = int(words[2])
      header["repeat_count"]  = int(words[3])
      header["elapsed_time"]  = (float(words[4])) # math.log
      header["check_string"]  = words[5]
      header["ptotal"]        = header["particles"] * header["steps"]
      bench_list.append(header)
  
      # Lecture de la prochaine ligne
      line = fp.readline()

  return bench_list




# Constant field: blop-debian11
# lorentz_covfie_list     = load_file("data/lorentz-euler_covfie_blop-debian11_2023-04-17_4096-65536_1024-2048_128-131072_constant-field.txt")
# lorentz_standalone_list = load_file("data/lorentz_standalone_blop-debian11_2023-04-16_4096-65536_1024-2048_128-131072_constant-field.txt")
# lorentz_kiwaku_list     = load_file("data/lorentz_kiwaku_blop-debian11_2023-04-16_4096-65536_1024-2048_128-131072_constant-field.txt")

if use_acts_field:
  lorentz_covfie_list     = load_file("data4/covfie_lorentz_nanob_chaton_4096-4096_1024-16384_128-128_InterpolateLin-LayoutStride-field.txt")
  lorentz_standalone_list = load_file("data4/lorentz_opti_nanob_standalone_chaton_4096-4096_1024-16384_128-128_acts-field.txt")
  lorentz_kiwaku_list     = load_file("data4/lorentz_opti_nanob_kiwaku_chaton_4096-4096_1024-16384_128-128_acts-field.txt")






# TEMPORARILY DISABLE ERROR CHECKING
# check_same_results(lorentz_standalone_list, lorentz_kiwaku_list)

# external_1D = make_1D_list(lorentz_external_list, "elapsed_time")
# covfie_1D   = make_1D_list(lorentz_covfie_list, "elapsed_time")

ptotal_1D   = pu.make_1D_list_every(lorentz_covfie_list, "particles", "steps", 1)

# plt.plot(range(1, len(external_1D)+1), external_1D, color="blue", label="external", linestyle="solid", linewidth=line_width)
# plt.plot(range(1, len(covfie_1D)+1), covfie_1D, color="red", label="covfie", linestyle="dashed", linewidth=line_width)

if plot_diff:
  ldiff0 = pu.make_diff_list(lorentz_covfie_list, lorentz_standalone_list)
  ldiff1 = pu.make_diff_list(lorentz_covfie_list, lorentz_covfie_list)
  ldiff2 = pu.make_diff_list(lorentz_covfie_list, lorentz_kiwaku_list)
else:
  ldiff0 = pu.make_absolute_list(lorentz_standalone_list)
  ldiff1 = pu.make_absolute_list(lorentz_covfie_list)
  ldiff2 = pu.make_absolute_list(lorentz_kiwaku_list)

plt.plot  (range(1, len(ldiff1)+1), ldiff1, color="green", label="Covfie", linestyle="dashed", linewidth=line_width)
plt.plot  (range(1, len(ldiff0)+1), ldiff0, color="grey", label="Standalone (opti matrix)", linestyle="dashdot", linewidth=line_width)
plt.plot  (range(1, len(ldiff2)+1), ldiff2, color="blue", label="Kiwaku (opti matrix)", linestyle="dotted", linewidth=line_width)

plt.rcParams['grid.linestyle'] = "-"
plt.rcParams['grid.alpha'] = 0.15
plt.rcParams['grid.color'] = "black" ##cccccc
plt.grid(linewidth=line_width/20)

plt.xticks(range(1, len(ldiff0)+1), ptotal_1D)

plt.title(computer_name + " - " + field_type_name + " - relative duration (%), lower is better")

# plt.ylabel('Elapsed time (µs)')
plt.ylabel('Duration compared to covfie (%), lower is better')
plt.xlabel('(particles) - (steps) - (total count in millions)')
# Ascending number of particles (and imom), in millions (10^6)
#plt.ylim([-5, 100])
plt.legend()
# global_drawn_x_variables_number+1
# plt.xticks(range(1, 6), x_list_curve_drawn) # = x_list_shared et x_list_acc

# plt.ylim([0, 16])

plt.savefig(output_image_name, format='png') #, dpi=my_dpi)

plt.show()
print ("Hello World!")
