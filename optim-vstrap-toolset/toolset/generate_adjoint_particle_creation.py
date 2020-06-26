#!/usr/bin/env python3

import argparse
import xml.etree.ElementTree as ET
import csv
import numpy.matlib
import numpy as np
import math

parser = argparse.ArgumentParser(prog="Generate File for creation of adjoint particles", description='Needs target folder for creation fle')
parser.add_argument('target_folder', type=str, help='target folder for creation file (relative path)')
args = parser.parse_args()

#file = open("creation_adjoint_particles.xml", 'w+');
file = open(args.target_folder + "/creation_adjoint_particles.xml", 'w+');

ntimesteps = 40;
mu_x = 0.0; 
mu_y = 0.0;
mu_z = 0.0;
s_x = 0.20;
s_y = 0.20;
s_z = 0.20;

v_x = 0.0;
v_y = 0.0;
v_z = 0.0;


v_s_x = 3e+2;
v_s_y = 3e+2;
v_s_z = 3e+2;


file.write("<parameraters>\n")

for timestep in range(0,ntimesteps):
	file.write("\t<set iteration=\"" + str(timestep) + "\">\n")
	file.write("\t\t<particle_values number_density=\"4e+12\" weight=\"5e+8\" charge_number=\"+1\" mass=\"6.63e-26\" species=\"40Ar+\"/>\n")
	file.write("\t\t<position>\n \t\t\t<mu x_val = \"" + str(mu_x) + "\" y_val = \"" + str(mu_y) + "\" z_val = \"" + str(mu_z) + "\" />\n")
	file.write("\t\t\t<sigma x_val = \"" + str(s_x) +"\" y_val = \"" + str(s_y)+ "\" z_val = \"" + str(s_z) +"\"/> \n \t\t</position>\n")
	file.write("\t\t<velocity> \n \t\t\t<mu x_val = \"" + str(v_x) + " \" y_val = \"" + str(v_y) + "\" z_val = \"" + str(v_z) + "\" />\n")
	file.write("\t\t\t<sigma x_val = \"" + str(v_s_x) +"\" y_val = \"" + str(v_s_y)+ "\" z_val = \"" + str(v_s_z) +"\"/> \n \t\t</velocity>\n")
	file.write("\t</set>\n")
file.write("</parameraters>")
