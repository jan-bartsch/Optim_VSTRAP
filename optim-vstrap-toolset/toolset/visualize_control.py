#!/usr/bin/env python3

import argparse
import xml.etree.ElementTree as ET

from control_field_class import*

if __name__ == '__main__':
	control = Control_field()
	print('Starting visualizing control...')

	parser = argparse.ArgumentParser(prog="Mesh data interpolation script", description='Reads mesh and control file. Visualizes Control.')
	parser.add_argument('control', type=str, help='path to the control file')
	parser.add_argument('mesh', type=str, help='path to the mesh file')
	parser.add_argument('scaling', type=float, help='scaling of control')
	parser.add_argument('pathToBuildSRC', type=str, help='path to the build directory')

	args = parser.parse_args()
	print('[Interpolate cell2Node]: Successfully parsed arguments')

	try:
		control.create_Lists(args.control,args.mesh,args.scaling)
		control.plot_Control_field(control.nodesMesh,control.endPoints,args.scaling,args.pathToBuildSRC)
	except Exception as e:
		print(e)
		exit()
