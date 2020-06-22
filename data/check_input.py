#!/usr/bin/env python3

import argparse
import xml.etree.ElementTree as ET
import xml.dom.minidom as minidom


parser = argparse.ArgumentParser(prog="Check if xml-arguments fit together", description='Needs data directory (with input files)')
parser.add_argument('inputDir', type=str, help='path to input files')
args = parser.parse_args()

OptimIn = minidom.parse(args.inputDir + "Optim_input.xml");
parameters = OptimIn.getElementsByTagName('globalParameters')[0];


for p in parameters.getElementsByTagName('parameter'):
	p_data = p.getAttribute("name")
	print(p_data)
