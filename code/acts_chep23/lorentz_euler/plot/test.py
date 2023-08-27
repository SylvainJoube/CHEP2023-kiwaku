import matplotlib.pyplot as plt
import numpy as np
import statistics as stat
import sys
import math
import plot_utils as pu

import random
  

values = []
random.seed(5)

for i in range(0, 1000):
  values.append(random.randint(10, 20))

values.append(-54462)

values.append(54462)
values.append(54462)
values.append(54462)


v = stat.quantiles(values, n=10)



print("median = " + str(stat.median(values)))
print("v = ")
print(v)

# stat.quantiles()