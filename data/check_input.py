#!/usr/bin/env python3

import argparse
import xml.etree.ElementTree as ET
import xml.dom.minidom as minidom

all_clear = True


parser = argparse.ArgumentParser(prog="Check if xml-arguments fit together", description='Needs data directory (with input files)')
parser.add_argument('inputDir', type=str, help='path to input files')
args = parser.parse_args()

OptimIn = minidom.parse(args.inputDir + "Optim_input.xml");
parameters = OptimIn.getElementsByTagName('globalParameters')[0];
paths = OptimIn.getElementsByTagName('paths')[0];
subroutines = OptimIn.getElementsByTagName('subroutines')[0];

params = {};

for p in parameters.getElementsByTagName('parameter'):
	p_data = p.getAttribute("name")
	params[p.getAttribute("name")] = p.getAttribute("value")

forwardIn = minidom.parse(args.inputDir + "input_forward.xml");
backwardIn = minidom.parse(args.inputDir + "input_backward.xml");

forward_timeStep = float(forwardIn.getElementsByTagName('global_values')[0].getElementsByTagName('time_step')[0].getAttribute("value"))
backward_timeStep = float(backwardIn.getElementsByTagName('global_values')[0].getElementsByTagName('time_step')[0].getAttribute("value"))
optim_timeStep = float(params["dt_gp"])

if (optim_timeStep != forward_timeStep or forward_timeStep != backward_timeStep or backward_timeStep != optim_timeStep):
	all_clear = False;

if(all_clear):
	print("All clear")
else:
	print("Error")

#for p in parameters.getElementsByTagName('parameter'):
#	p_data = p.getAttribute("name")
#	print(p_data)
