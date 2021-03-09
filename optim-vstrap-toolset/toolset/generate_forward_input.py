#!/usr/bin/env python3

import sys
import argparse
import xml.etree.ElementTree as ET
import xml.dom.minidom as minidom
import csv
import numpy.matlib
import numpy as np
import math

parser = argparse.ArgumentParser(prog="Generate File for creation of adjoint particles", description='Needs Optim_input (xml)')
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
file_forward_input.write("\t <global_values> \n \t\t <time_step value=\"" + str(params["dt_VSTRAP"]) + "\"/> \n \t </global_values> \n")
file_forward_input.write("\t <abort_criterium> \n \t\t <max_itterations value=\"" + str(params["ntimesteps_gp_VSTRAP"]) + "\" /> \n \t </abort_criterium>\n")

optim_vMesh = pathsList["DOMAIN_MESH"]
optim_sMesh = pathsList["SURFACE_MESH"]
#optim_vMesh = optim_vMesh.replace("../../Optim_VSTRAP/data/","../")

file_forward_input.write("\t <executables> \n \t \t <executable name=\"mesh_initializer\" mode=\"CPU\"> \n \t\t\t <mesh name=\"vol_mesh\">\n")
file_forward_input.write("\t\t\t\t <load>"+str(optim_vMesh)+"</load> \n \t\t\t </mesh> \n")
if(float(params['inflow_included'])==1):
    file_forward_input.write("\t\t\t<mesh name=\"surf_mesh\">\n")
    file_forward_input.write("\t\t\t\t <load>"+str(optim_sMesh)+"</load> \n \t\t\t\t <face_data name=\"surface_charging\" type=\"double\"/>\n \t\t\t </mesh> \n")
file_forward_input.write("\t\t </executable>\n")

optim_pmax_gp = params['pmax_gp']

if(str(params['creation_forward_particles_method'])=='create_new'):
   print("Initialize forward particles using random values")
   file_forward_input.write("\t \t <executable name=\"particle_initializer\" mode=\"CPU\"> \n \t\t\t<group name=\"forward_particles\" empty=\"false\">\n")
   file_forward_input.write("\t\t\t\t <geometry x_min=\"-" +str(optim_pmax_gp) +" \" x_max=\"" +str(optim_pmax_gp) +" \" y_min=\"-" +str(optim_pmax_gp) +" \" y_max=\"" +str(optim_pmax_gp) +" \" z_min=\"-" +str(optim_pmax_gp) +" \" z_max=\"" +str(optim_pmax_gp) +" \"/> \n ")
   file_forward_input.write("\t\t\t\t <values number_density=\" "+ str(params["number_density_forward"]) +" \" weight=\""+ str(params["weight_forward"]) +" \" charge_number=\""+ str(params["charge_number_forward"]) +" \" mass=\"" + str(params["mass_forward"]) + "\" species=\"" + str(params["species_forward"]) + "\"/> \n ")
   file_forward_input.write("\t\t\t\t <temperature x_val=\"" + str(params["temperature_x_val"]) + "\" y_val=\"" + str(params["temperature_y_val"]) + "\" z_val=\"" + str(params["temperature_z_val"]) + "\"/> \n ")
   file_forward_input.write("\t\t\t\t <v_drift x_val=\""+ str(params["v_drift_x_val"]) +"\" y_val=\""+ str(params["v_drift_y_val"]) +"\" z_val=\""+ str(params["v_drift_z_val"]) +"\"/> \n \t\t\t </group> \n \t\t </executable> \n ")
elif(str(params['creation_forward_particles_method'])=='create_existing'):
    print("Initialize forward particles using existing values")
    file_forward_input.write("\t\t <executable name=\"particle_initializer\" mode=\"CPU\"> \n \t\t\t<group name=\"forward_particles\">\n")
    file_forward_input.write("\t\t\t <load>"+str(params['initial_condition_file'])+"</load> \n \t\t\t </group> \n \t\t </executable> \n ")
elif(str(params['creation_forward_particles_method'])=='only_inflow'):
    file_forward_input.write("\t\t <executable name=\"particle_initializer\" mode=\"CPU\"> \n \t\t\t<group name=\"forward_particles\" empty=\"true\"/>\n")
    file_forward_input.write("\t\t </executable> \n ")

#########
## pwi ##
#########

file_forward_input.write("\t\t <executable name=\"pwi\" mode=\"CPU\">\n")
if(float(params['inflow_included'])==1):
	file_forward_input.write("\t\t\t<boundary_type name=\"mesh\"/>\n")
	file_forward_input.write("\t\t\t<mesh name=\"surf_mesh\"/>\n")
	file_forward_input.write("\t\t\t<outlet_surface tag= \"4\"/>\n")
	file_forward_input.write("\t\t\t<batch name=\"forward_particles\">\n")
	file_forward_input.write("\t\t\t\t<species name =\""+str(params["species_forward"])+"\"/>\n")
	file_forward_input.write("\t\t\t</batch>\n")
else:
	file_forward_input.write("\t\t\t<boundary_type name=\"cuboid\"/>\n ")
	file_forward_input.write("\t\t\t <geometry x_min=\"-" +str(optim_pmax_gp) +" \" x_max=\"" +str(optim_pmax_gp) +" \" y_min=\"-" +str(optim_pmax_gp) +" \" y_max=\"" +str(optim_pmax_gp) +" \" z_min=\"-" +str(optim_pmax_gp) +" \" z_max=\"" +str(optim_pmax_gp) +" \"/> \n ")
	file_forward_input.write("\t\t\t <particle_group name=\"forward_particles\"/> \n")
file_forward_input.write("\t\t </executable> \n")

file_forward_input.write("\t\t <executable name=\"bgf\" mode=\"CPU\"> \n \t\t\t<volume_mesh name=\"vol_mesh\"/>\n")
file_forward_input.write("\t\t\t <load>./"+ str(pathsList["BGF_CONTROL"]) +"</load>\n  ")
file_forward_input.write("\t\t\t <particle_batch> \n \t\t\t\t <field name=\"interpolated_control_field\"/> \n \t\t\t\t <particle_group name=\"forward_particles\"/> \n \t\t\t </particle_batch>\n \t\t </executable>\n")

file_forward_input.write("\t\t <executable name=\"pusher\" mode=\"CPU\" type=\"forward\">\n")
file_forward_input.write("\t\t\t <particle_group name=\"forward_particles\"/> \n \t\t </executable> \n")

file_forward_input.write("\t\t  <executable name=\"dsmc\" mode=\"CPU\"> \n \t\t\t <particle_group name=\"forward_particles\"/> \n \t\t\t <load>"+ str(pathsList["DSMC_interaction"]) +"</load> \n \t\t </executable>\n")

file_forward_input.write("\t\t <executable name=\"plasma_state\" mode=\"CPU\"> \n \t\t\t <batch file_name=\"batch_1\" format=\"csv\" output_interval=\""+str(params['plasma_state_output_interval'])+"\"> \n \t\t\t <particle_group name=\"forward_particles\"/> \n \t\t\t </batch> \n")
file_forward_input.write("\t\t\t<file path = \"./results/\" name=\"plasma_state\" format=\"csv\" output_interval=\"1\"/> \n \t\t </executable>\n")


if(float(params['mesh_2d_writer_included'])==1):
    print("Include mesh_2d_writer")
    print(str(pathsList['PATH_TO_SHARED_FILES']))
    file_forward_input.write("\t\t <executable name=\"mesh_2d_data_writer\" mode=\"CPU\">\n")
    file_forward_input.write("\t\t\t <file name=\"" + str(params['mesh_2d_name']) + "\" format=\"vtu\" path=\"" + str(pathsList['PATH_TO_SHARED_FILES']) + str(pathsList['mesh_2d_path']) + "\" output_interval=\"" + str(params['mesh_2d_outputinterval']) +"\" />\n")
    file_forward_input.write("\t\t\t <particle_group name=\"forward_particles\"/>\n")
    file_forward_input.write("\t\t\t <values height=\"1.0\" min_1=\"-0.5\" min_2=\"-0.5\" dl=\"0.25\" n_1=\"4\" n_2=\"4\" plane=\"xy\"/>\n")
    file_forward_input.write("\t\t </executable>\n")

if(float(params['mesh_3d_writer_included'])==1):
    print("Include mesh_3d_writer")
    print(str(pathsList['PATH_TO_SHARED_FILES']))
    file_forward_input.write("\t\t <executable name=\"mesh_3d_data_writer\" mode=\"CPU\">\n")
    file_forward_input.write("\t\t\t <file name=\"" + str(params['mesh_3d_name']) + "\" format=\"vtu\" path=\"" + str(pathsList['PATH_TO_SHARED_FILES']) + str(pathsList['mesh_3d_path']) + "\" output_interval=\"" + str(params['mesh_3d_outputinterval']) +"\" />\n")
    file_forward_input.write("\t\t\t <particle_group name=\"forward_particles\"/>\n")
    file_forward_input.write("\t\t </executable>\n")

if(float(params['inflow_included'])==1):
    print("Inflow present")
    file_forward_input.write("\t\t <executable name=\"inflow\" mode=\"CPU\">\n \t\t\t <batch name=\"static\" type=\"static\"> \n")
    file_forward_input.write("\t\t\t\t <particle_group name=\"forward_particles\"/>\n")
    file_forward_input.write("\t\t\t\t <values number_density=\" "+ str(params["number_density_forward_inflow"]) +" \" weight=\""+ str(params["weight_forward_inflow"]) +"\" charge_number=\""+ str(params["charge_number_forward_inflow"]) +"\" mass=\"" + str(params["mass_forward_inflow"]) + "\" species=\"" + str(params["species_forward_inflow"]) + "\"/>\n")
    file_forward_input.write("\t\t\t\t <temperature x_val=\"" + str(params["temperature_x_val_inflow"]) + "\" y_val=\"" + str(params["temperature_y_val_inflow"]) + "\" z_val=\"" + str(params["temperature_z_val_inflow"]) + "\"/> \n ")
    file_forward_input.write("\t\t\t\t <v_drift x_val=\"" + str(params["v_drift_x_val_inflow"]) + "\" y_val=\"" + str(params["v_drift_y_val_inflow"]) + "\" z_val=\"" + str(params["v_drift_z_val_inflow"]) + "\"/> \n ")
    file_forward_input.write("\t\t\t\t <physical_surface tag=\"6\"/>\n \t\t\t </batch>\n \t\t\t <mesh name=\"surf_mesh\"/> \n \t\t </executable>\n")

file_forward_input.write("\t\t <executable name=\"fmm\" mode=\"CPU\">\n \t\t\t <smearing_radius value=\"0.0\"/>\n \t\t\t <method name=\"fmm\"/>\n \t\t\t <particle_group name=\"forward_particles\"/>\n \t\t </executable>\n")

file_forward_input.write("\t </executables>\n")



file_forward_input.write("\t <schedule> \n \t\t <init> \n")
file_forward_input.write("\t\t\t <executable name=\"mesh_initializer\"/> \n")
file_forward_input.write("\t\t\t <executable name=\"particle_initializer\"/> \n")
if(float(params['inflow_included'])==1):
    file_forward_input.write("\t\t\t <executable name=\"inflow\"/> \n")
file_forward_input.write("\t\t\t <executable name=\"plasma_state\"/> \n")
file_forward_input.write("\t\t\t <executable name=\"pusher\"/> \n")
file_forward_input.write("\t\t\t <executable name=\"pwi\"/> \n")
file_forward_input.write("\t\t\t <executable name=\"dsmc\"/> \n")
if(float(params['fmm'])==1):
	file_forward_input.write("\t\t\t <executable name=\"fmm\" />\n")
if(float(params['mesh_2d_writer_included'])==1):
    file_forward_input.write("\t\t\t <executable name=\"mesh_2d_data_writer\" />\n")
if(float(params['mesh_3d_writer_included'])==1):
    file_forward_input.write("\t\t\t <executable name=\"mesh_3d_data_writer\" />\n")
file_forward_input.write("\t\t\t <executable name=\"bgf\"/> \n \t\t</init> \n")

file_forward_input.write("\t\t <exec> \n")
if(float(params['inflow_included'])==1):
    file_forward_input.write("\t\t\t <executable name=\"inflow\"/> \n")
if(float(params['fmm'])==1):
	file_forward_input.write("\t\t\t <executable name=\"fmm\" />\n")
file_forward_input.write("\t\t\t <executable name=\"dsmc\"/> \n")
file_forward_input.write("\t\t\t <executable name=\"pwi\"/> \n")
file_forward_input.write("\t\t\t <executable name=\"bgf\"/> \n")
file_forward_input.write("\t\t\t <executable name=\"pusher\"/> \n")
if(float(params['mesh_2d_writer_included'])==1):
    file_forward_input.write("\t\t\t <executable name=\"mesh_2d_data_writer\" />\n")
if(float(params['mesh_3d_writer_included'])==1):
    file_forward_input.write("\t\t\t <executable name=\"mesh_3d_data_writer\" />\n")
file_forward_input.write("\t\t\t <executable name=\"plasma_state\"/> \n \t\t </exec> \n")
file_forward_input.write("\t</schedule>\n")
file_forward_input.write("</simulation>")
