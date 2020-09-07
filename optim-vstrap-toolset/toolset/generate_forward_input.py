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
file_forward_input.write("\t\t\t\t <load>"+str(optim_vMesh)+"</load> \n \t\t\t </mesh> \n \t\t </executable>\n")

optim_pmax_gp = params['pmax_gp']

file_forward_input.write("\t \t <executable name=\"particle_initializer\" mode=\"CPU\"> \n \t\t\t<group name=\"forward_particles\" empty=\"false\">\n")
file_forward_input.write("\t\t\t\t <geometry x_min=\"-" +str(optim_pmax_gp) +" \" x_max=\"" +str(optim_pmax_gp) +" \" y_min=\"-" +str(optim_pmax_gp) +" \" y_max=\"" +str(optim_pmax_gp) +" \" z_min=\"-" +str(optim_pmax_gp) +" \" z_max=\"" +str(optim_pmax_gp) +" \"/> \n ")
file_forward_input.write("\t\t\t\t <values number_density=\" "+ str(params["number_density_forward"]) +" \" weight=\""+ str(params["weight_forward"]) +" \" charge_number=\""+ str(params["charge_number_forward"]) +" \" mass=\"" + str(params["mass_forward"]) + "\" species=\"" + str(params["species_forward"]) + "\"/> \n ")
file_forward_input.write("\t\t\t\t <temperature x_val=\"" + str(params["temperature_x_val"]) + "\" y_val=\"" + str(params["temperature_y_val"]) + "\" z_val=\"" + str(params["temperature_z_val"]) + "\"/> \n ")
file_forward_input.write("\t\t\t\t <v_drift x_val=\""+ str(params["v_drift_x_val"]) +"\" y_val=\""+ str(params["v_drift_y_val"]) +"\" z_val=\""+ str(params["v_drift_z_val"]) +"\"/> \n \t\t\t </group> \n \t\t </executable> \n ")

file_forward_input.write("\t\t <executable name=\"pwi\" mode=\"CPU\"> \n \t\t\t<boundary_type name=\"cuboid\"/>\n")
file_forward_input.write("\t\t\t <geometry x_min=\"-" +str(optim_pmax_gp) +" \" x_max=\"" +str(optim_pmax_gp) +" \" y_min=\"-" +str(optim_pmax_gp) +" \" y_max=\"" +str(optim_pmax_gp) +" \" z_min=\"-" +str(optim_pmax_gp) +" \" z_max=\"" +str(optim_pmax_gp) +" \"/> \n ")
file_forward_input.write("\t\t\t <particle_group name=\"forward_particles\"/> \n \t\t </executable> \n")

file_forward_input.write("\t\t <executable name=\"bgf\" mode=\"CPU\"> \n \t\t\t<volume_mesh name=\"vol_mesh\"/>\n")
file_forward_input.write("\t\t\t <load>"+ str(pathsList["bgf_control"]) +"</load>\n  ")
file_forward_input.write("\t\t\t <particle_batch> \n \t\t\t\t <field name=\"interpolated_control_field\"/> \n \t\t\t\t <particle_group name=\"forward_particles\"/> \n \t\t\t </particle_batch>\n \t\t </executable>\n")

file_forward_input.write("\t\t <executable name=\"pusher\" mode=\"CPU\" type=\"forward\">\n")
file_forward_input.write("\t\t\t <particle_group name=\"forward_particles\"/> \n \t\t </executable> \n")

file_forward_input.write("\t\t  <executable name=\"dsmc\" mode=\"CPU\"> \n \t\t\t <particle_group name=\"forward_particles\"/> \n \t\t\t <load>"+ str(pathsList["DSMC_interaction"]) +"</load> \n \t\t </executable>\n")

file_forward_input.write("\t\t <executable name=\"plasma_state\" mode=\"CPU\"> \n \t\t\t <batch file_name=\"batch_1\" format=\"csv\" output_interval=\"1\"> \n \t\t\t <particle_group name=\"forward_particles\"/> \n \t\t\t </batch> \n")
file_forward_input.write("\t\t\t<file path = \"./results/\" name=\"plasma_state\" format=\"csv\" output_interval=\"1\"/> \n \t\t </executable>\n")
file_forward_input.write("\t </executables>\n")

file_forward_input.write("\t <schedule> \n \t\t <init> \n")
file_forward_input.write("\t\t\t <executable name=\"mesh_initializer\"/> \n")
file_forward_input.write("\t\t\t <executable name=\"particle_initializer\"/> \n")
file_forward_input.write("\t\t\t <executable name=\"plasma_state\"/> \n")
file_forward_input.write("\t\t\t <executable name=\"pusher\"/> \n")
file_forward_input.write("\t\t\t <executable name=\"pwi\"/> \n")
file_forward_input.write("\t\t\t <executable name=\"dsmc\"/> \n")
file_forward_input.write("\t\t\t <executable name=\"bgf\"/> \n \t\t</init> \n")
file_forward_input.write("\t\t <exec> \n")
file_forward_input.write("\t\t\t <executable name=\"bgf\"/> \n")
file_forward_input.write("\t\t\t <executable name=\"dsmc\"/> \n")
file_forward_input.write("\t\t\t <executable name=\"pusher\"/> \n")
file_forward_input.write("\t\t\t <executable name=\"pwi\"/> \n")
file_forward_input.write("\t\t\t <executable name=\"plasma_state\"/> \n \t\t </exec> \n")
file_forward_input.write("\t</schedule>\n")
file_forward_input.write("</simulation>")








