#!/usr/bin/env python3

import argparse
from matplotlib import pyplot as plt
import tikzplotlib

fig_objective = plt.figure(figsize=(10, 10))

parser = argparse.ArgumentParser(prog="Visualizing optimization monitoring", description='Needs path to generated result files and decrease scale')
parser.add_argument('pathToResults', type=str, help='path to the results directory')
parser.add_argument('scale', type=float, help='diminishing scale')
args = parser.parse_args()

####
# objective
####

#try:
difference_file = open(args.pathToResults + "Difference.txt")

difference = difference_file.read().split("\n")
difference_float = [];

landau_decrease_linear = [];
landau_decrease_quadratic = [];

scale = args.scale;


for i in range(0,len(difference)-1):
	difference_float.insert(len(difference_float),abs(float(difference[i])))
	landau_decrease_linear.insert(len(landau_decrease_linear),pow(scale,i)*difference_float[0])
	landau_decrease_quadratic.insert(len(landau_decrease_quadratic),pow(pow(scale,i),2)*difference_float[0])


ax1 = plt.subplot(221)
plt.plot(difference_float, label="difference")
plt.plot(landau_decrease_linear, label="linear")
plt.plot(landau_decrease_quadratic, label="quadratic")
plt.title("Difference")
ax1.set_yscale('log')
ax1.legend()
#except:
#	print("No Difference file existing")

plt.show()
