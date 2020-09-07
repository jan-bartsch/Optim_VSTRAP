#!/usr/bin/env python3

import sys
import argparse
import xml.etree.ElementTree as ET
import xml.dom.minidom as minidom
import csv
import numpy.matlib
import numpy as np
import math

parser = argparse.ArgumentParser(prog="Generate File for creation of adjoint particles", description='Needs target folder for creation file and Optim_input (xml)')
parser.add_argument('OptimInput', type=str, help='filepath of optim input')
args = parser.parse_args()


OptimIn = minidom.parse(args.OptimInput);
parameters = OptimIn.getElementsByTagName('globalParameters')[0];
paths = OptimIn.getElementsByTagName('paths')[0];
subroutines = OptimIn.getElementsByTagName('subroutines')[0];
params = {}
pathsList = {}
subs = {}

for p in parameters.getElementsByTagName('parameter'):
	params[p.getAttribute("name")] = p.getAttribute("value")

for p in paths.getElementsByTagName('path'):
	pathsList[p.getAttribute("name")] = p.getAttribute("value")

#####
# forward input
#####

print("Creating file " + str(pathsList["PATH_TO_SHARED_FILES_ABSOLUTE"]) + str(pathsList["INPUT_FORWARD"]))
file_forward_input = open(str(pathsList["PATH_TO_SHARED_FILES_ABSOLUTE"]) + str(pathsList["INPUT_FORWARD"]), 'w+')

file_forward_input.write("<simulation>\n")
file_forward_input.write("\t <global_values> \n \t\t <time_step value=\"" + str(params["dt_gp"]) + "\"/> \n \t </global_values> \n")
file_forward_input.write("\t <abort_criterium> \n \t\t <max_itterations value=\"" + str(params["ntimesteps_gp"]) + "\" /> \n \t </abort_criterium>\n")

optim_vMesh = pathsList["DOMAIN_MESH"]
optim_vMesh = optim_vMesh.replace("../../Optim_VSTRAP/data/","../")

file_forward_input.write("\t <executables> \n \t \t <executable name=\"mesh_initializer\" mode=\"CPU\"> \n \t\t\t <mesh name=\"vol_mesh\">\n")
file_forward_input.write("\t\t\t\t <load>"+str(optim_vMesh)+"</load> \n \t\t\t </mesh>")

optim_pmax_gp = pathsList["pmax_gp"]

file_forward_input.write("\t <executables> \n \t \t <executable name=\"particle_initializer\" mode=\"CPU\"> \n \t\t\t<group name=\"forward_particles\" empty=\"false\">\n")
file_forward_input.write("\t\t\t\t <geometry x_min=\"-" +str(optim_pmax_gp) +" \" x_max\"" +str(optim_pmax_gp) +" \" y_min=\"-" +str(optim_pmax_gp) +" \" y_max=\"" +str(optim_pmax_gp) +" \" z_min=\"-" +str(optim_pmax_gp) +" \" z_max=\"" +str(optim_pmax_gp) +" \"/> \n ")
