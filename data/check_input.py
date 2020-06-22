#!/usr/bin/env python3

import argparse
import xml.etree.ElementTree as ET
import xml.dom.minidom as minidom


parser = argparse.ArgumentParser(prog="Check if xml-arguments fit together", description='Needs data directory (with input files)')
parser.add_argument('inputDir', type=str, help='path to input files')
args = parser.parse_args()

OptimIn = minidom.parse(args.inputDir + "Optim_input.xml");
parameters = OptimIn.getElementsByTagName('globalParameters')[0];
paths = OptimIn.getElementsByTagName('paths')[0];
subroutines = OptimIn.getElementsByTagName('subroutines')[0];

params = [];
i = 0;

for p in parameters.getElementsByTagName('parameter'):
	p_data = p.getAttribute("name")
	print(i)
	params.append([p.getAttribute("name"),p.getAttribute("value")])
	i = i+1

forwardIn = minidom.parse(args.inputDir + "input_forward.xml");
backwardIn = minidom.parse(args.inputDir + "input_backward.xml");

forward_timeStep = forwardIn.getElementsByTagName('global_values')[0].getElementsByTagName('time_step')[0].getAttribute("value")
backward_timeStep = backwardIn.getElementsByTagName('global_values')[0].getElementsByTagName('time_step')[0].getAttribute("value")

print(params)

print(Optim_timestep)

#for p in parameters.getElementsByTagName('parameter'):
#	p_data = p.getAttribute("name")
#	print(p_data)
