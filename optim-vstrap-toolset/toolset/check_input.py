#!/usr/bin/env python3

import argparse
import xml.etree.ElementTree as ET
import xml.dom.minidom as minidom

def check_input():

	all_clear = True

	parser = argparse.ArgumentParser(prog="Check if xml-arguments fit together", description='Needs data directory (with input files) and filepath of optim input')
	parser.add_argument('inputDir', type=str, help='path to input files')
	parser.add_argument('OptimInput', type=str, help='filepath of optim input')
	args = parser.parse_args()

	OptimIn = minidom.parse(args.OptimInput);
	forwardIn = minidom.parse(args.inputDir + "input_forward.xml");
	backwardIn = minidom.parse(args.inputDir + "input_backward.xml");

	parameters = OptimIn.getElementsByTagName('globalParameters')[0];
	paths = OptimIn.getElementsByTagName('paths')[0];
	subroutines = OptimIn.getElementsByTagName('subroutines')[0];
	f_exec = forwardIn.getElementsByTagName('executables')[0]
	b_exec = backwardIn.getElementsByTagName('executables')[0]


	params = {}
	pathsList = {}
	subs = {}
	f_executables = {}
	b_executables = {}

	for p in parameters.getElementsByTagName('parameter'):
		params[p.getAttribute("name")] = p.getAttribute("value")

	for p in paths.getElementsByTagName('path'):
		pathsList[p.getAttribute("name")] = p.getAttribute("value")

	for e in f_exec.getElementsByTagName('executable'):
		print(e.getAttribute("name"))
		f_executables[e.getAttribute("name")] = e

	for e in b_exec.getElementsByTagName('executable'):
		#print(e.getAttribute("name"))
		b_executables[e.getAttribute("name")] = e


	####
	# dimensionOfControl_gp, pcell_gp
	####

	if (float(params["dimensionOfControl_gp"])>float(params["pcell_gp"])):
		print("[Check_Input] dimensionOfControl_gp is greater than pcell_gp, this will lead to an error in the calculation of the gradient")
		print("dimensionOfControl_gp: " + params["dimensionOfControl_gp"])
		print("pcell_gp: " + params["pcell_gp"])
		all_clear = False;
	else:
		print("[Check_Input] dimensionOfControl_gp < pcell_gp")

	####
	# Time step
	####
	forward_timeStep = float(forwardIn.getElementsByTagName('global_values')[0].getElementsByTagName('time_step')[0].getAttribute("value"))
	backward_timeStep = float(backwardIn.getElementsByTagName('global_values')[0].getElementsByTagName('time_step')[0].getAttribute("value"))
	optim_timeStep = float(params["dt_gp"])

	if (optim_timeStep != forward_timeStep or forward_timeStep != backward_timeStep or backward_timeStep != optim_timeStep):
		print("[Check_Input] Timestep-size is not consistent")
		all_clear = False;
	else:
		print("[Check_Input] Timestep-size is consistent")

	####
	# Iterations in time
	####
	forward_iterations = float(forwardIn.getElementsByTagName('abort_criterium')[0].getElementsByTagName('max_itterations')[0].getAttribute("value"))
	backward_iterations = float(backwardIn.getElementsByTagName('abort_criterium')[0].getElementsByTagName('max_itterations')[0].getAttribute("value"))
	optim_iterations = float(params["ntimesteps_gp"])

	if (optim_iterations != forward_iterations or forward_iterations != backward_iterations or backward_iterations != optim_iterations):
		print("[Check_Input] Iterations number is not consistent")
		all_clear = False;
	else:
		print("[Check_Input] Iterations number is consistent")


	####
	# Volume mesh
	####
	forward_vMesh = f_executables["mesh_initializer"].getElementsByTagName('mesh')[0].getElementsByTagName('load')[0].firstChild.nodeValue
	forward_vMesh = forward_vMesh.replace('../', '/')
	backward_vMesh = b_executables["mesh_initializer"].getElementsByTagName('mesh')[0].getElementsByTagName('load')[0].firstChild.nodeValue
	backward_vMesh = backward_vMesh.replace('../', '/')
	optim_vMesh = pathsList["DOMAIN_MESH"]

	if (forward_vMesh == backward_vMesh):
		if (forward_vMesh in optim_vMesh):
			print("[Check_Input] Mesh equal everywhere")
		else:
			print("Mesh not equal")
			print("Input files was " + forward_vMesh + "; Optim_input was " + optim_vMesh)
			all_clear = False
	else:
		print("[Check_Input] Mesh in input files not equal")
		all_clear = False

	####
	# control field
	####
	forward_control = f_executables["bgf"].getElementsByTagName('load')[0].firstChild.nodeValue
	backward_control = b_executables["bgf"].getElementsByTagName('load')[0].firstChild.nodeValue

	if (forward_control == backward_control):
		print("[Check_Input] Control equal")
	else:
		print("[Check_Input] Controls are not equal")
		all_clear = False
		print(forward_control)
		print(backward_control)

	####
	# pwi
	####
	forward_pwi = f_executables["pwi"].getElementsByTagName('boundary_type')[0].getAttribute("name")
	backward_pwi = b_executables["pwi"].getElementsByTagName('boundary_type')[0].getAttribute("name")


	if (forward_pwi == backward_pwi):
		print("[Check_Input] PWI boundary_type equal")
	else:
		print("[Check_Input] PWI boundary_type are not equal:")
		all_clear = False
		print(forward_pwi)
		print(backward_pwi)

	####
	# geometry
	####
	forward_geo = f_executables["particle_initializer"].getElementsByTagName('geometry')[0]
	backward_geo = b_executables["adjoint_particle_creator"].getElementsByTagName('geometry')[0]
	optim_geo = float(params["pmax_gp"])

	if (forward_geo.getAttribute("x_min")==backward_geo.getAttribute("x_min") and forward_geo.getAttribute("y_min")==backward_geo.getAttribute("y_min") and forward_geo.getAttribute("z_min")==backward_geo.getAttribute("z_min")
	and forward_geo.getAttribute("x_max")==backward_geo.getAttribute("x_max") and forward_geo.getAttribute("y_max")==backward_geo.getAttribute("y_max") and forward_geo.getAttribute("z_max")==backward_geo.getAttribute("z_max") ):
		print("[Check input] Geometry equal")
		if (optim_geo == float(forward_geo.getAttribute("x_max")) ):
			print("[Check input] Geometry optim equal")
		else:
			print("[Check input] Geometry optim not equal")
			all_clear = False
	else:
		print("[Check input] Geometry not equal")
		all_clear = False;



	if(all_clear):
		print("\n[Check_Input] **All clear**")
		return 0
	else:
		print("[Check_Input] Error, some input Parameters are not consistent")
		raise Exception('Input Parameters are not consistent')
		return 1

if __name__ == '__main__':
	check_input()
	exit()
