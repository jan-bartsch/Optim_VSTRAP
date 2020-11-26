#!/usr/bin/env python3

import argparse
from matplotlib import pyplot as plt
import tikzplotlib
import os

fig_objective = plt.figure(figsize=(10, 10))

parser = argparse.ArgumentParser(prog="Visualizing optimization monitoring", description='Needs path to generated result files and decrease scale')
parser.add_argument('pathToResults', type=str, help='path to the results directory')
parser.add_argument('scale', type=float, help='diminishing scale')
args = parser.parse_args()

####
# objective
####

#try:
H1_difference_file = open(args.pathToResults + "H1-difference.txt")
H2_difference_file = open(args.pathToResults + "H2-difference.txt")
L2_difference_file = open(args.pathToResults + "L2-difference.txt")

weight_vector_file = open(args.pathToResults + "discretization_vector.txt")

H1_difference = H1_difference_file.read().split("\n")
H1_difference_float = []

H2_difference = H2_difference_file.read().split("\n")
H2_difference_float = []

L2_difference = L2_difference_file.read().split("\n")
L2_difference_float = []

weight_vector = weight_vector_file.read().split("\n")
weight_vector_float = []

weight_vector_float1 = []

landau_decrease_linear = []
landau_decrease_quadratic = []

scale = args.scale;


for i in range(0,len(H1_difference)-1):
	H1_difference_float.insert(len(H1_difference_float),abs(float(H1_difference[i]))/abs(float(H1_difference[0])))
	H2_difference_float.insert(len(H2_difference_float),abs(float(H2_difference[i]))/abs(float(H2_difference[0])))
	L2_difference_float.insert(len(L2_difference_float),abs(float(L2_difference[i]))/abs(float(L2_difference[0])))
	landau_decrease_linear.insert(len(landau_decrease_linear),pow(scale,i)*H1_difference_float[0])
	landau_decrease_quadratic.insert(len(landau_decrease_quadratic),pow(pow(scale,i),2)*H1_difference_float[0])

	weight_vector_float1.insert(len(weight_vector_float1),(float(weight_vector[i])-float(weight_vector[i+1]))/(float(weight_vector[0])))
	weight_vector_float.insert(len(weight_vector_float),(float(weight_vector[i]))/(float(weight_vector[0])))

print(weight_vector_float)
print(weight_vector_float1)

ax1 = plt.gca()
plt.plot(H1_difference_float, label="difference H1")
plt.plot(H2_difference_float, label="difference H2")
#plt.plot(L2_difference_float, label="difference L2")
plt.plot(weight_vector_float, label="fraction weights")
plt.plot(weight_vector_float1, label="difference weights")
linear_label = "linear (" + str(scale)+")";
#plt.plot(landau_decrease_linear, label=linear_label)
#plt.plot(landau_decrease_quadratic, label="quadratic")
plt.title("Difference between controls (relative)")
plt.xlabel("iterations")
ax1.set_yscale('log')
#ax1.set_xscale('log')
ax1.legend()
#except:
#	print("No Difference file existing")

plt.show()
