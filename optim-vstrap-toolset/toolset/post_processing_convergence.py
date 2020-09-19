#!/usr/bin/env python3

import argparse
from matplotlib import pyplot as plt
import tikzplotlib

fig_objective = plt.figure(figsize=(10, 10))

parser = argparse.ArgumentParser(prog="Visualizing optimization monitoring", description='Needs path to generated src files')
parser.add_argument('pathToBuildSRC', type=str, help='path to the build directory')
args = parser.parse_args()

####
# objective
####

try:
	objective_track_file = open(args.pathToBuildSRC + "objectiveTrack.txt")

	objective_track = objective_track_file.read().split("\n")
	objective_track_float = [];


	for i in range(0,len(objective_track)-1):
		objective_track_float.insert(len(objective_track_float),float(objective_track[i])/abs(float(objective_track[0])))


	ax1 = plt.subplot(221)
	plt.plot(objective_track_float)
	plt.title("Relative value of objective")
	ax1.set_yscale('linear')
except:
	print("No objectiveTrack file existing")


####
# gradient
####
try:
	norm_gradient_file = open(args.pathToBuildSRC + "normGradientTrack.txt")

	norm_gradient = norm_gradient_file.read().split("\n")
	norm_gradient_float = []

	#print(norm_gradient)

	for i in range(0,len(norm_gradient)-1):
		norm_gradient_float.insert(len(norm_gradient_float),float(norm_gradient[i])/float(norm_gradient[0]))

	ax2 = plt.subplot(222)
	plt.plot(norm_gradient_float)
	plt.title("Norm relative gradient")
	ax2.set_yscale('linear')
except:
	print("No normGradientTrack file existing")

####
# control
####
try:
	norm_control_file = open(args.pathToBuildSRC + "normControlTrack.txt")

	norm_control = norm_control_file.read().split("\n")
	norm_control_float = []

	for i in range(0,len(norm_control)-1):
		norm_control_float.insert(len(norm_control_float),float(norm_control[i]))

	ax3 = plt.subplot(223)
	plt.plot(norm_control_float)
	plt.title("Norm control")
	ax3.set_yscale('linear')
except:
	print("No normControlTrack file existing")



####
# wasserstein distance
####
try:
	wd_file = open(args.pathToBuildSRC + "stepsizeTrack.txt")


	wd = wd_file.read().split("\n")
	wd_float = []

	for i in range(0,len(wd)-1):
		wd_float.insert(len(wd_float),float(wd[i]))

	ax4 = plt.subplot(224)
	plt.plot(wd_float)
	plt.title("stepsizeTrack")
	ax4.set_yscale('log')
except:
	print("No stepsizeTrack file existing")

tikzplotlib.save(args.pathToBuildSRC+"post_processing_converging.tex")
plt.savefig(args.pathToBuildSRC+"optimization_monitoring.png")
plt.show()
