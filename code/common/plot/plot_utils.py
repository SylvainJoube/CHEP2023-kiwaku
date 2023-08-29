
import matplotlib.pyplot as plt
import numpy as np
import statistics as stat
import sys
import math



def make_1D_list(bench_list, field_name):
  res = []
  ind = 0
  for i in bench_list:
    res.append(i[field_name]) # field_name = "elapsed_time"
    ind += 1
  return res



def make_1D_list_every(bench_list, field_name, take_every = 0):
  res = []
  ind = 0
  for i in bench_list:
    if (ind % take_every == 0):
      res.append(int(i[field_name])) # field_name = "elapsed_time_us"
      # res.append(int(i[field_name] / 1000000)) # field_name = "elapsed_time"
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



def make_1D_list_every_3lines(bench_list, field_name1, field_name2, take_every = 0):
  res = []
  ind = 0
  for i in bench_list:
    if (ind % take_every == 0):
      v1 = i[field_name1]
      v2 = i[field_name2]
      tot = v1 * v2
      res.append(str(v1) + "\n" + str(v2) + "\n" + str(int(tot/1000000)) + "M") # field_name = "elapsed_time"
      # res.append(int(i[field_name] / 1000000)) # field_name = "elapsed_time"
    else:
      res.append("")
    ind += 1
  return res



def make_1D_list_every_1line(bench_list, field_name1, take_every = 0):
  res = []
  ind = 0
  for i in bench_list:
    if (ind % take_every == 0):
      v1 = i[field_name1]
      res.append(str(v1))
    else:
      res.append("")
    ind += 1
  return res



def make_diff_list(l1, l2, keyword = None):
  res  = []
  if (len(l1) != len(l2)):
    sys.exit("ERROR @make_diff_list. Number of points not the same in both lists.")

  if keyword == None:
    keyword = "elapsed_time"

  for i in range(0, len(l1)):
    v1 = l1[i][keyword]
    v2 = l2[i][keyword]
    diff = (100 * v2 / v1) # / l1[i]["ptotal"]
    # diff = 100 + (v2 - v1) / ((abs(v1) + abs(v2))/2) # Abs not necessary since v1 and v2 are positive time values 
    res.append(diff) # Percentage
  return res


def make_absolute_list(l1, keyword = None):
  if keyword == None:
    keyword = "elapsed_time"
  res  = []
  for i in range(0, len(l1)):
    res.append(l1[i][keyword])
  return res

# def make_absolute_list(l1):
#   return make_absolute_list(l1, "elapsed_time")




def check_same_results(l1, l2, field_name, tolerance = 5):
  if (len(l1) != len(l2)):
    sys.exit("ERROR @check_same_results. Number of points not the same in both lists.")

  for i in range(0, len(l1)):
    v1 = l1[i][field_name] # "check_string"
    v2 = l2[i][field_name]
    if (tolerance == 0):
      if (v1 != v2):
        sys.exit("ERROR @check_same_results. Value at position "
        + str(i) + " differs: \n    " + str(v1) + " != \n    " + str(v2) + " (tolerance = 0)")
    else:
      if (abs(v1 - v2) > tolerance):
        sys.exit("ERROR @check_same_results. Value at position "
        + str(i) + " differs: abs(v1(" + str(v1) + ") - v2(" +str(v2) + ") > tolerance(" + str(tolerance) + ")")
  print("OK check_same_results, " + str(len(l1)) + " value(s) checked.")
  return

# def check_same_results(l1, l2):
#   if (len(l1) != len(l2)):
#     sys.exit("ERROR @check_same_results. Number of points not the same in both lists.")

#   for i in range(0, len(l1)):
#     v1 = l1[i]["check_value"]
#     v2 = l2[i]["check_value"]
#     if (abs(v1 - v2) > 5):
#       sys.exit("ERROR @check_same_results. Value at position "
#       + str(i) + " differs: " + str(v1) + " != " +str(v2) + " (diff > 3)")
#   print("OK check_same_results, " + str(len(l1)) + " value(s) checked.")
#   return

def draw_violin_plot(color, y_list):
  c = color
  bp_ = plt.violinplot(y_list, showextrema=False)
  for pc in bp_['bodies']:
      pc.set_facecolor(c)
      pc.set_edgecolor(c) # #D43F3A black
      pc.set_alpha(1)



def filter_outliers(list):

  if len(list) <= 2:
    return list

  n_quantiles = 10 ## must be >= 4
  
  q = stat.quantiles(list, n=n_quantiles)

  index = 0
  for i in range(len(list)):
    elem = list[index]
    if (elem < q[0]) or (elem > q[n_quantiles - 2]):
      list.pop(index)
    else:
      index += 1

  return list


def remove_newline(list):
  if (len(list) == 1) and (list[0] == "\n"):
    del list[0]
    # print("remove_newline newsize = " + str(len(list)))
  else:
    if (len(list) != 0):
      list[len(list)-1] = list[len(list)-1].rstrip("\n")
  return list

def list_str_to_int(list, divide_by = None):
  # print("list_str_to_int len = " + str(len(list)) + " 1st value = "+ str(list[0]))
  if len(list) == 0:
    list.append(0)
  else:
    if divide_by == None:
      list = [int(i) for i in list]
    else:
      list = [int(i)/divide_by for i in list]
  return list


def make_violin_plot_list(bench_list, keyword, compare_to_list = None):
  l2_res = []

  for header in bench_list:
    l2_res.append(header[keyword].copy())
  
  if compare_to_list != None:
    l2_compare = []

    for header in compare_to_list:
      l2_compare.append(header[keyword]) # <- ! not a copy !
    
    for il in range(0, len(l2_res)):
      l1_res = l2_res[il]
      l1_compare = l2_compare[il]
      compare_value = stat.median(l1_compare)

      for i in range(0, len(l1_res)):
        old = l1_res[i]
        l1_res[i] = 100 * l1_res[i] / compare_value # l1_compare[i]
      l2_res[il] = l1_res

  return l2_res