#!/usr/bin/python

# Hello world python program

# Lancer le script : python3 ./2022-02-08_sparseccl.py

import matplotlib.pyplot as plt
import numpy as np
import statistics as stat
import sys
import math


VERSION_ATTENDUE = 1

# ============= Gestion de la taille
my_dpi = 96
# output_image_name = "no_name"
output_image_name = "ACTS_magnetic_field"
output_image_ver  = "v1"

computer_name = "blop" # aussi utilisé pour le nom du fichier d'entrée

# image_ratio = 1920 / 1080
image_ratio = 880 / 480

image_width = 1280 # 1280
image_height = 0
image_scale_factor = image_width / 640
line_width = image_scale_factor * 1.5

image_height = image_width / image_ratio #(image_width / 640) * 480
plt.figure(figsize=(image_width/my_dpi, image_height/my_dpi) , dpi=my_dpi)
# output_image_name = "lorentz_compare_" + computer_name + "_" + output_image_name + ".png"
output_image_name = "slice_" + computer_name + "_" + output_image_name + ".png"

MY_SIZE = (10 * image_scale_factor)
TITLE_SIZE = (12 * image_scale_factor)

#plt.rc('font', size=MY_SIZE)          # controls default text sizes
plt.rc('axes', titlesize=TITLE_SIZE)     # fontsize of the axes title
plt.rc('axes', labelsize=MY_SIZE)     # fontsize of the x and y labels
plt.rc('xtick', labelsize=MY_SIZE)    # fontsize of the tick labels
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


def make_1D_list(bench_list, field_name):
  res = []
  ind = 0
  for i in bench_list:
    res.append(i[field_name]) # field_name = "elapsed_time_us"
    ind += 1
  return res

def make_1D_list_every(bench_list, field_name, take_every = 0):
  res = []
  ind = 0
  for i in bench_list:
    if (ind % take_every == 0):
      res.append(int(i[field_name])) # field_name = "elapsed_time_us"
    else:
      res.append("")
    ind += 1
  return res

def make_1D_list_every_auto(bench_list, field_name):
  every = int(len(bench_list) / 6)
  print("len(bench_list) = " + str(len(bench_list)))
  print("every = " + str(every))
  if (every < 1):
    every = 1
  return make_1D_list_every(bench_list, field_name, every)

def make_diff_list(l1, l2):
  res  = []
  if (len(l1) != len(l2)):
    sys.exit("ERROR @make_diff_list. Number of points not the same in both lists.")

  for i in range(0, len(l1)):
    v1 = l1[i]["elapsed_time_us"]
    v2 = l2[i]["elapsed_time_us"]
    diff = 100 * v2 / v1
    # diff = (v2 - v1) / ((abs(v1) + abs(v2))/2) # Abs not necessary since v1 and v2 are positive time values 
    res.append(diff) # Percentage
  return res


def check_same_results(l1, l2):
  if (len(l1) != len(l2)):
    sys.exit("ERROR @check_same_results. Number of points not the same in both lists.")

  for i in range(0, len(l1)):
    v1 = l1[i]["check_value"]
    v2 = l2[i]["check_value"]
    if (abs(v1 - v2) > 5):
      sys.exit("ERROR @check_same_results. Value at position "
      + str(i) + " differs: " + str(v1) + " != " +str(v2) + " (diff > 3)")
  print("OK check_same_results, " + str(len(l1)) + " value(s) checked.")
  return


# TEMPORARILY DISABLE ERROR CHECKING



# acts_kiwaku_inline_list   = load_file("data/slice_kiwaku_inline_blop-debian11_step100.000000.txt")
# acts_kiwaku_noinline_list = load_file("data/slice_kiwaku_noinline_blop-debian11_step100.000000.txt")
# acts_standalone_list      = load_file("data/slice_standalone_blop-debian11_step100.000000.txt")


acts_kiwaku_inline_list   = load_file("data3/slice_kiwaku_inline_blop-debian11_step1000.txt")
acts_kiwaku_noinline_list = load_file("data3/slice_kiwaku_noinline_blop-debian11_step1000.txt")
acts_standalone_list      = load_file("data3/slice_standalone_opti_blop-debian11_step1000.txt")

check_same_results(acts_standalone_list, acts_standalone_list)
check_same_results(acts_standalone_list, acts_kiwaku_inline_list)
check_same_results(acts_standalone_list, acts_kiwaku_noinline_list)

ldiff0 = make_diff_list(acts_standalone_list, acts_standalone_list)
ldiff1 = make_diff_list(acts_standalone_list, acts_kiwaku_inline_list)
ldiff2 = make_diff_list(acts_standalone_list, acts_kiwaku_noinline_list)

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
ptotal_1D   = make_1D_list_every_auto(acts_standalone_list, "z_value")

# range(1, len(ldiff)+1) #make_1D_list_every(lorentz_covfie_list, "ptotal", 3)

plt.plot  (range(1, len(ldiff0)+1), ldiff0, color="grey", label="standalone", linestyle="dashdot", linewidth=line_width)
plt.plot  (range(1, len(ldiff1)+1), ldiff1, color="blue", label="kiwaku", linestyle="solid", linewidth=line_width)
# plt.plot  (range(1, len(ldiff2)+1), ldiff2, color="maroon", label="kiwaku not inline", linestyle="dotted", linewidth=line_width)

# plt.plot  (range(1, len(ldiff4)+1), ldiff4, color="red", label="standalone v3", linestyle="solid", linewidth=line_width)

# plt.ylim([0, 100])

plt.rcParams['grid.linestyle'] = "-"
plt.rcParams['grid.alpha'] = 0.15
plt.rcParams['grid.color'] = "black" ##cccccc
plt.grid(linewidth=line_width/20)

plt.xticks(range(1, len(ldiff1)+1), ptotal_1D)


plt.title(computer_name + " - ACTS slicing - Relative performance (%) between Kiwaku (inline) and:")

# # draw_curve("USM shared copy", "blue", y_list_shared_copy, y_median_shared_copy, "dotted")
# # draw_curve("USM host copy", "red", y_list_host_copy, y_median_host_copy, "dashdot")
# draw_curve("USM shared", "blue", y_list_shared_direct, y_median_shared_direct, "dotted")
# draw_curve("USM host", "red", y_list_host_direct, y_median_host_direct, "dashdot")

# plt.title("SparseCCL - flat arrays")
# #draw_curve("USM device", "green", y_list_device, y_median_device, "solid")
# draw_curve("shared flat", "blue", y_list_shared_flat, y_median_shared_flat, "solid")
# draw_curve("shared ptr", "green", y_list_shared_ptr, y_median_shared_ptr, "dotted")
# #draw_curve("USM host", "maroon", y_list_acc, y_median_acc, "dashed")
# #draw_curve("USM host", "maroon", y_list_host, y_median_host, "dashed")

# Faire l'affichage du tableau de valeurs (x100) et voir ce que ça donne
# par rapport aux précédentes valeurs 

# if FLAT_ONLY:
#     plt.figure(figsize=(800/my_dpi, 800/my_dpi), dpi=my_dpi)
# else:
#     plt.figure(figsize=(800/my_dpi, 800/my_dpi), dpi=my_dpi)


# plt.ylabel('Elapsed time (µs)')
plt.ylabel('Relative duration (%), lower is better')
plt.xlabel('Z-value')
#plt.ylim([-5, 100])
plt.legend()
# global_drawn_x_variables_number+1
# plt.xticks(range(1, 6), x_list_curve_drawn) # = x_list_shared et x_list_acc

plt.ylim([0, 110])

plt.savefig(output_image_name, format='png') #, dpi=my_dpi)

plt.show()
print ("Hello World!")
