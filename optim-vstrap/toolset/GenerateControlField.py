#!/usr/bin/env python3

import argparse
import xml.etree.ElementTree as ET

from mesh import*

if __name__ == '__main__':
    mesh = Mesh()
    
    print('Starting main...')

    parser = argparse.ArgumentParser(prog="Mesh data interpolation script", description='Reads mesh and control file. Interpolates control defined in cells to mesh nodes.')
    parser.add_argument('mesh', type=str, help='path to the mesh file')
    parser.add_argument('control', type=str, help='path to the control file')
    parser.add_argument('new_control', type=str, help='path to the created control file')
    parser.add_argument('-i', '--input', type=str, help='input format', default='xml')
    parser.add_argument('-o', '--output', type=str, help='output format', default='xml')

    args = parser.parse_args()
    print('[Interpolate cell2Node]: Successfully parsed arguments')

    try:
        if args.input == "xml":
            mesh.read_mesh_xml(args.mesh)
            print('[Interpolate cell2Node]: Successfully read mesh xml')
            mesh.read_control_xml(args.control)
            print('[Interpolate cell2Node]: Successfully read control xml')
        elif args.input == "csv":
            mesh.read_mesh_csv(args.mesh)
            mesh.read_control_csv(args.control)
            print('[Interpolate cell2Node]: Successfully read mesh/control csv')
        else:
            raise Exception("Undefined input format", args.input)

        mesh.interpolate_cell2node()

        if args.output == "xml":
            mesh.write_control_xml(args.new_control)
            print('[Interpolate cell2Node]: Successfully wrote control xml')
        elif args.output == "csv":
            mesh.write_control_csv(args.new_control)
            print('[Interpolate cell2Node]: Successfully wrote control csv')
        else:
            raise Exception("Undefined output format", args.output)
    except Exception as e:
        print(e)
        exit()
