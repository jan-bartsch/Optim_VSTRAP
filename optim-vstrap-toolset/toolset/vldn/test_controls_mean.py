#!/usr/bin/env python3

import argparse
from matplotlib import pyplot as plt
import tikzplotlib
import os
import csv
import math

fig_objective = plt.figure(figsize=(10, 10))

parser = argparse.ArgumentParser(prog="Visualizing optimization monitoring", description='Needs path to generated result files and decrease scale')
parser.add_argument('pathToResults', type=str, help='path to the results directory')
args = parser.parse_args()

discretization_vector_file = open(args.pathToResults + "discretization_vector.txt")

discretization_vector = discretization_vector_file.read().split("\n")
discretization_vector_float = []

for i in range(0,len(discretization_vector)-1):
	discretization_vector_float.insert(len(discretization_vector_float),abs(float(discretization_vector[i])))
	print(discretization_vector_float[i])
	
means_distance=[];
zeros=[];

with open(args.pathToResults +'Means.csv', newline='') as csvfile:
	reader = csv.reader(csvfile, delimiter=',', quotechar='|')
	for row in reader:
		#print(', '.join(row))
		means_distance.insert(len(means_distance),math.sqrt(float(row[0])*float(row[0])+float(row[1])*float(row[1])+float(row[2])*float(row[2])))
		print(math.sqrt(float(row[0])*float(row[0])+float(row[1])*float(row[1])+float(row[2])*float(row[2])))
		zeros.insert(len(zeros),0)

ax1 = plt.gca()
plt.title("Norm of matrix barycenter")
plt.plot(means_distance,'--o',label="Norm barycenter")
plt.plot(zeros,'k')
#ax1.set_yscale('log')
#ax1.set_xscale('log')
plt.xlabel("discretization")
plt.ylabel("norm")
ax1.legend()
#except:
#	print("No Difference file existing")

plt.show()
