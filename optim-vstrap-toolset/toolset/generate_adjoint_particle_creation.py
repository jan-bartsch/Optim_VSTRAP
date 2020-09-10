#!/usr/bin/env python3

import argparse
import xml.etree.ElementTree as ET
import csv
import numpy.matlib
import numpy as np
import math
import xml.dom.minidom as minidom

parser = argparse.ArgumentParser(prog="Generate File for creation of adjoint particles", description='Needs target folder for creation file and Optim_input (xml)')
parser.add_argument('OptimInput', type=str, help='filepath of optim input')
parser.add_argument('target_folder', type=str, help='target folder for creation file (relative path)')
args = parser.parse_args()

OptimIn = minidom.parse(args.OptimInput);
parameters = OptimIn.getElementsByTagName('globalParameters')[0];
paths = OptimIn.getElementsByTagName('paths')[0];
params = {}
pathsList = {}

for p in parameters.getElementsByTagName('parameter'):
	params[p.getAttribute("name")] = p.getAttribute("value")

for p in paths.getElementsByTagName('path'):
	pathsList[p.getAttribute("name")] = p.getAttribute("value")

file = open(args.target_folder + str(pathsList["CREATION_ADJOINT_PARTCLES"]), 'w+');

ntimesteps = int(params["ntimesteps_gp"])
mu_x = float(params["adjoint_mu_x"])
mu_y = float(params["adjoint_mu_y"])
mu_z = float(params["adjoint_mu_z"])
s_x = float(params["adjoint_s_x"])
s_y = float(params["adjoint_s_y"])
s_z = float(params["adjoint_s_z"])

v_x = float(params["adjoint_vx"])
v_y = float(params["adjoint_vx"])
v_z = float(params["adjoint_vx"])

most_probable_speed = float(params["expected_speed"])
expected_speed = float(params["most_probable_speed"])

sigma_v = math.sqrt(math.pi/8.0)*expected_speed
#sigma_v = math.sqrt(1.0/2.0)*most_probable_-speed


v_s_x = sigma_v;
v_s_y = sigma_v;
v_s_z = sigma_v;


file.write("<parameraters>\n")

for timestep in range(0,ntimesteps):
	file.write("\t<set iteration=\"" + str(timestep) + "\">\n")
	file.write("\t\t<particle_values number_density=\""+str(params["adjoint_number_density"])+" \" weight=\""+str(params["adjoint_weight"])+"\" charge_number=\""+str(params["adjoint_charge_number"])+"\" mass=\""+str(params["adjoint_mass"])+"\" species=\""+str(params["adjoint_species"])+"\"/>\n")
	file.write("\t\t<position>\n \t\t\t<mu x_val = \"" + str(mu_x) + "\" y_val = \"" + str(mu_y) + "\" z_val = \"" + str(mu_z) + "\" />\n")
	file.write("\t\t\t<sigma x_val = \"" + str(s_x) +"\" y_val = \"" + str(s_y)+ "\" z_val = \"" + str(s_z) +"\"/> \n \t\t</position>\n")
	file.write("\t\t<velocity> \n \t\t\t<mu x_val = \"" + str(v_x) + " \" y_val = \"" + str(v_y) + "\" z_val = \"" + str(v_z) + "\" />\n")
	file.write("\t\t\t<sigma x_val = \"" + str(v_s_x) +"\" y_val = \"" + str(v_s_y)+ "\" z_val = \"" + str(v_s_z) +"\"/> \n \t\t</velocity>\n")
	file.write("\t</set>\n")
file.write("</parameraters>")
