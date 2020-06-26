import xml.etree.ElementTree as ET
import csv
import numpy.matlib
import numpy as np
import math

class Node:
    def __init__(self, id = 0, coord = (0.0, 0.0, 0.0)):
        self.id = id
        self.x_coord = coord[0]
        self.y_coord = coord[1]
        self.z_coord = coord[2]
        self.value = np.zeros(3)

    def get_position(self):
        return np.asarray([self.x_coord, self.y_coord, self.z_coord])

class Cell:
    def __init__(self):
        self.id = 0
        self.nodes_ids = []
        self.value = np.zeros(3)
        self.volume = 0.0
        self.type = 0
        self.barycenter = np.zeros(3)

    def set_nodes(self, nodes):
        for node in nodes:
            self.nodes_ids.append(node.id)

        self.volume = self.calc_volume(nodes)
        self.barycenter = self.calc_barycenter(nodes)

    def calc_volume(self, nodes):
        if self.type == 2:
            # surface triangle
            pass
        elif self.type == 4:
            return self._calc_volume_tetrahedron(nodes)
        elif self.type == 5:
            return self._calc_volume_hexahedron(nodes)
        else:
            raise Exception(__name__, self.calc_volume.__name__, "Undefined volume calculation for type {}.".format(self.type))

    def calc_barycenter(self, nodes):
        if self.type == 2:
            # barycenter triangle
            pass
        elif self.type == 4:
            # barycenter tetrahedron
            pass
        elif self.type == 5:
            return self._calc_barycenter_hexahedron(nodes)
        else:
            raise Exception(__name__, self.calc_volume.__name__, "Undefined volume calculation for type {}.".format(self.type))

    def _calc_volume_tetrahedron(self, nodes):
        point1 = nodes[0].get_position()
        point2 = nodes[1].get_position()
        point3 = nodes[2].get_position()
        point4 = nodes[3].get_position()

        diff1 = point1 - point4
        diff2 = point2 - point4
        diff3 = point3 - point4

        return abs(diff1.dot(np.cross(diff2, diff3)))/6.0

    def _calc_volume_hexahedron(self, nodes):
        point1 = nodes[0].get_position()
        point2 = nodes[1].get_position()
        point3 = nodes[2].get_position()
        point4 = nodes[3].get_position()
        point5 = nodes[4].get_position()

        diff1 = np.linalg.norm(point2 - point1)
        diff2 = np.linalg.norm(point4 - point1)
        diff3 = np.linalg.norm(point5 - point1)

        return diff1*diff2*diff3

    def _calc_barycenter_hexahedron(self, nodes):
        #print(nodes)
        point1 = nodes[0].get_position()
        point2 = nodes[1].get_position()
        point3 = nodes[2].get_position()
        point4 = nodes[3].get_position()
        point5 = nodes[4].get_position()
        point6 = nodes[5].get_position()
        point7 = nodes[6].get_position()
        point8 = nodes[7].get_position()

        barycenter = 1/8*(point1+point2+point3+point4+point5+point6+point7+point8)
        #print(barycenter)

        return barycenter

class Mesh:
    def __init__(self):
        self.cells = {}
        self.nodes = {}
        self.volume = 0.0

    def __str__(self):
        str = ""
        str += "CELLS:\n"
        str += "NODES:"

        return str

    def clear(self):
        self.cells = {}
        self.nodes = {}
        self.volume = 0.0

    def read_mesh_xml(self, file_name):
        tree = ET.parse(file_name)
        root = tree.getroot()

        self.clear()
        self.__check_mesh_file(root)
        self.__create_nodes(root)
        self.__create_cells(root)
        self.__calc_volume()

    def interpolate_cell2node(self):
        node2cell = {}

        for id, node in self.nodes.items():
            node2cell[id] = []

        for id, cell in self.cells.items():
            for node_id in cell.nodes_ids:
                node2cell[node_id].append(id)

        for node_id, cell_ids in node2cell.items():
            volume = 0.0

            for cell_id in cell_ids:
                cell = self.cells[cell_id]

                if cell.volume != None:
                    self.nodes[node_id].value += cell.value * cell.volume
                    volume += cell.volume

            self.nodes[node_id].value /= volume

    def read_control_csv(self, file_name):
        with open(file_name, newline='') as csvfile:
            reader = csv.reader(csvfile, delimiter=',')
            first_line = True

            for row in reader:
                if first_line:
                    first_line = False
                else:
                    cell_id = int(row[0])
                    control = (float(row[1]), float(row[2]), float(row[3]))

                    self.cells[cell_id].value = np.array(control)

    def read_control_xml(self, file_name):
        tree = ET.parse(file_name)
        root = tree.getroot()

        values = root.findall('value')

        for value in values:
            cell_id = int(value.get('cell_id'))
            control = []

            for str in value.text.split(','):
                control.append(float(str))

            self.cells[cell_id].value = np.array(control)

    def write_control_csv(self, file_name):
        with open(file_name, 'w+') as file:
            file.write("#node_id,#x,#y,#z\n")

            for id, node in self.nodes.items():
                file.write(str(id) + ',' + str(node.value[0]) + ',' + str(node.value[1]) + ',' + str(node.value[2]) + '\n')

    def write_control_xml(self, file_name):
        with open(file_name, 'w+') as file:
            file.write("<field name=\"interpolated_control_field\" type=\"electric_field\" dimensions=\"3\" number_of_elements=\"" + str(len(self.nodes)) + "\">\n")

            for id, node in self.nodes.items():
                file.write("<value node_number=\"" + str(id) + "\">")
                file.write(str(node.value[0]) + "," + str(node.value[1]) + "," + str(node.value[2]))
                file.write("</value>\n")

            file.write("</field>")

    def write_barycenters_xml(self, file_name):
        print("Generating barycenters")
        with open(file_name, 'w+') as file:
            file.write("<root>\n") 
            file.write("<mesh_barycenters name=\"box_hexahedrons\" type=\" \" dimensions=\"3\" number_of_elements=\"" + str(len(self.cells)) + "\">\n")

            for id, cell in self.cells.items():
                file.write("<barycenter cell_id=\"" + str(id) + "\">")
                file.write(str(cell.barycenter[0]) + "," + str(cell.barycenter[1]) + "," + str(cell.barycenter[2]))
                file.write("</barycenter>\n")

            file.write("</mesh_barycenters>\n")
            file.write("</root>")
            print("Done generating barycenters")



    def __check_mesh_file(self, root):
        mesh_node = root.find('mesh')
        mesh_type = mesh_node.get('mesh_type')

        if mesh_type != "volume_mesh":
            raise Exception(__name__, self.__check_mesh_file.__name__, 'volume_mesh not defined in file')

    def __create_nodes(self, root):
        for element in root.find('nodes').findall('node'):
            node = Node()

            node.id = int(element.get('node_number'))
            node.x_coord = float(element.get('x_coord'))/1000.0
            node.y_coord = float(element.get('y_coord'))/1000.0
            node.z_coord = float(element.get('z_coord'))/1000.0

            if node.id not in self.nodes:
                self.nodes[node.id] = node
            else:
                raise Exception(__name__, self.__create_nodes.__name__, 'Node with id {} allready in list.'.format(node.id))

    def __create_cells(self, root):
        for element in root.find('elements').findall('element'):
            cell = Cell()
            node_ids = self.__get_node_ids(element.find('nodes').text)
            nodes = []

            for id in node_ids:
                nodes.append(self.nodes[id])

            cell.id = int(element.get('elem_number'))
            cell.type = int(element.get('elm_type'))
            cell.set_nodes(nodes)

            if cell.id not in self.cells:
                self.cells[cell.id] = cell
            else:
                raise Exception(__name__, self.__create_nodes.__name__, 'Cell with id {} allready in list.'.format(cell.id))

    def __get_node_ids(self, text):
        node_ids = []

        for str in text.split(','):
            node_ids.append(int(str))

        return node_ids

    def __calc_volume(self):
        self.volume = 0.0

        for id, cell in self.cells.items():
            if cell.volume != None:
                self.volume += cell.volume
