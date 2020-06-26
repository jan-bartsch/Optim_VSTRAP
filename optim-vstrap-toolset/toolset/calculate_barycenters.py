#!/usr/bin/env python3

import argparse
import xml.etree.ElementTree as ET

from mesh import*

if __name__ == '__main__':
    mesh = Mesh()
    
    print('Starting main...')

    parser = argparse.ArgumentParser(prog="Mesh data interpolation script", description='Reads mesh and control file. Interpolates control defined in cells to mesh nodes.')
    parser.add_argument('mesh', type=str, help='path to the mesh file')
    parser.add_argument('barycenters', type=str, help='path to the barycenter file')

    args = parser.parse_args()
    print('[Calculate Barycenters]: Successfully parsed arguments')

    mesh.read_mesh_xml(args.mesh)
    mesh.write_barycenters_xml(args.barycenters)
    exit()
