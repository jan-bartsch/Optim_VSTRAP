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

#print(params)
#####
# backward input
##### 

print("Creating file " + str(pathsList["PATH_TO_SHARED_FILES_ABSOLUTE"]) + str(pathsList["INPUT_BACKWARD"]))
file_backward_input = open(str(pathsList["PATH_TO_SHARED_FILES_ABSOLUTE"]) + str(pathsList["INPUT_BACKWARD"]), 'w+')

file_backward_input.write("<simulation>\n")
file_backward_input.write("\t <global_values> \n \t\t <time_step value=\"" + str(params["dt_gp"]) + "\"/> \n \t </global_values> \n")
file_backward_input.write("\t <abort_criterium> \n \t\t <max_itterations value=\"" + str(params["ntimesteps_gp"]) + "\" /> \n \t </abort_criterium>\n")

optim_vMesh = pathsList["DOMAIN_MESH"]
#optim_vMesh = optim_vMesh.replace("../../Optim_VSTRAP/data/","../")

file_backward_input.write("\t <executables> \n \t \t <executable name=\"mesh_initializer\" mode=\"CPU\"> \n \t\t\t <mesh name=\"vol_mesh\">\n")
file_backward_input.write("\t\t\t\t <load>"+str(optim_vMesh)+"</load> \n \t\t\t </mesh> \n \t\t </executable>\n")

file_backward_input.write("\t\t <executable name=\"particle_initializer\" mode=\"CPU\"> \n \t\t\t<group name=\"adjoint_particles\" empty=\"true\"/>\n \t\t </executable>\n")

optim_pmax_gp = params['pmax_gp']

file_backward_input.write("\t\t <executable name=\"adjoint_particle_creator\" mode=\"CPU\"> \n \t\t\t")
file_backward_input.write("\t\t\t <geometry x_min=\"-" +str(optim_pmax_gp) +" \" x_max=\"" +str(optim_pmax_gp) +" \" y_min=\"-" +str(optim_pmax_gp) +" \" y_max=\"" +str(optim_pmax_gp) +" \" z_min=\"-"+str(optim_pmax_gp) +" \" z_max=\"" +str(optim_pmax_gp) +" \"/> \n")
file_backward_input.write("\t\t\t <particle_group name=\"adjoint_particles\" type=\"ions\" number_density=\""+str(params['adjoint_number_density'])
                          +"\" weight=\""+str(params['adjoint_weight'])+"\" charge_number=\""+str(params['adjoint_charge_number'])
                          +"\" mass=\""+ str(params['adjoint_mass']) +"\" + species=\""+str(params['adjoint_species'])+"\" /> \n")
file_backward_input.write("\t\t\t <particle_group name=\"adjoint_particles_electrons\" type=\"electrons\" number_density=\""+str(params['adjoint_number_density'])
                          +"\" weight=\""+str(params['adjoint_weight'])+"\" charge_number=\"-1\" mass=\"9.109e-31\" + species=\"e-\" /> \n")
file_backward_input.write("\t\t\t <load>"+str(pathsList["CREATION_ADJOINT_PARTCLES"])+"</load> \n \t\t </executable> \n")

file_backward_input.write("\t\t <executable name=\"pwi\" mode=\"CPU\"> \n \t\t\t<boundary_type name=\"cuboid\"/>\n")
file_backward_input.write("\t\t\t <geometry x_min=\"-" +str(optim_pmax_gp) +" \" x_max=\"" +str(optim_pmax_gp) +" \" y_min=\"-" +str(optim_pmax_gp) +" \" y_max=\"" +str(optim_pmax_gp) +" \" z_min=\"-" +str(optim_pmax_gp) +" \" z_max=\"" +str(optim_pmax_gp) +" \"/> \n ")
file_backward_input.write("\t\t\t <particle_group name=\"adjoint_particles\"/> \n \t\t </executable> \n")

file_backward_input.write("\t\t <executable name=\"bgf\" mode=\"CPU\"> \n \t\t\t<volume_mesh name=\"vol_mesh\"/>\n")
file_backward_input.write("\t\t\t <load>./"+ str(pathsList["BGF_CONTROL"]) +"</load>\n  ")
file_backward_input.write("\t\t\t <particle_batch> \n \t\t\t\t <field name=\"interpolated_control_field\"/> \n \t\t\t\t <particle_group name=\"adjoint_particles\"/> \n \t\t\t </particle_batch>\n \t\t </executable>\n")

file_backward_input.write("\t\t <executable name=\"pusher\" mode=\"CPU\" type=\"adjoint\">\n")
file_backward_input.write("\t\t\t <particle_group name=\"adjoint_particles\"/> \n \t\t </executable> \n")

file_backward_input.write("\t\t<executable name=\"mcc\" mode=\"CPU\"> \n \t\t\t <particle_group name=\"adjoint_particles\"/> \n \t\t\t <interaction name=\"ela\"> \n \t\t\t </interaction> \n \t\t\t")
file_backward_input.write("<background> \n \t\t\t\t <species name=\"40Ar\" mass=\"6.6335209e-26\" mole_fraction=\"1.0\"> \n \t\t\t\t <number_density fixed=\"true\" value=\"1e20\"/> \n \t\t\t\t <temperature fixed=\"true\" x=\"1000\" y=\"1000\" z=\"1000\"/> \n \t\t\t\t </species> \n \t\t\t </background> \n")
file_backward_input.write("\t\t\t <load>"+str(pathsList["MCC_interaction"]) + "</load> \n \t\t </executable> \n")

file_backward_input.write("\t\t <executable name=\"plasma_state\" mode=\"CPU\"> \n \t\t\t <batch file_name=\"batch_1\" format=\"csv\" output_interval=\"1\"> \n \t\t\t <particle_group name=\"adjoint_particles\"/> \n \t\t\t </batch> \n")
file_backward_input.write("\t\t\t<file path = \"./results/\" name=\"plasma_state\" format=\"csv\" output_interval=\"1\"/> \n \t\t </executable>\n")
file_backward_input.write("\t </executables>\n")

file_backward_input.write("\t <schedule> \n \t\t <init> \n")
file_backward_input.write("\t\t\t <executable name=\"mesh_initializer\"/> \n")
file_backward_input.write("\t\t\t <executable name=\"particle_initializer\"/> \n")
file_backward_input.write("\t\t\t <executable name=\"adjoint_particle_creator\"/> \n")
file_backward_input.write("\t\t\t <executable name=\"plasma_state\"/> \n")
file_backward_input.write("\t\t\t <executable name=\"pusher\"/> \n")
file_backward_input.write("\t\t\t <executable name=\"pwi\"/> \n")
file_backward_input.write("\t\t\t <executable name=\"mcc\"/> \n")
file_backward_input.write("\t\t\t <executable name=\"bgf\"/> \n \t\t</init> \n")
file_backward_input.write("\t\t <exec> \n")
file_backward_input.write("\t\t\t <executable name=\"mcc\"/> \n")
file_backward_input.write("\t\t\t <executable name=\"adjoint_particle_creator\"/> \n")
file_backward_input.write("\t\t\t <executable name=\"bgf\"/> \n")
file_backward_input.write("\t\t\t <executable name=\"pusher\"/> \n")
file_backward_input.write("\t\t\t <executable name=\"pwi\"/> \n")
file_backward_input.write("\t\t\t <executable name=\"plasma_state\"/> \n \t\t </exec> \n")
file_backward_input.write("\t </schedule>\n")
file_backward_input.write("</simulation>")


