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
valide_vector_file = open(args.pathToResults + "Valide.txt")

discretization_vector = discretization_vector_file.read().split("\n")
discretization_vector_float = []

valide_vector = valide_vector_file.read().split("\n")
valide_vector_float = []


for i in range(0,len(discretization_vector)-1):
	discretization_vector_float.insert(len(discretization_vector_float),abs(float(discretization_vector[i])))
	#print(discretization_vector_float[i])
	valide_vector_float.insert(len(valide_vector_float),float(valide_vector[i]))
	
print(valide_vector_float)
means_distance=[];
zeros=[];
counter = 0;
with open(args.pathToResults +'Means.csv', newline='') as csvfile:
	reader = csv.reader(csvfile, delimiter=',', quotechar='|')
	for row in reader:
		#print(', '.join(row))
		print(counter)
		means_distance.insert(len(means_distance),math.sqrt(float(row[0])*float(row[0])+float(row[1])*float(row[1])+float(row[2])*float(row[2]))*valide_vector_float[counter])
		print(math.sqrt(float(row[0])*float(row[0])+float(row[1])*float(row[1])+float(row[2])*float(row[2])))
		zeros.insert(len(zeros),0)
		counter = counter +1

ax1 = plt.gca()
plt.title("Norm of cross product (control x perfect control) multiplied by orientation")
plt.plot(discretization_vector_float,means_distance,'--o',label="Norm")
plt.plot(discretization_vector_float,zeros,'k')
#ax1.set_yscale('log')
ax1.set_xscale('log')
plt.xlabel("discretization of mass (weight of particle)")
plt.ylabel("norm")
ax1.legend()
#except:
#	print("No Difference file existing")

plt.show()
