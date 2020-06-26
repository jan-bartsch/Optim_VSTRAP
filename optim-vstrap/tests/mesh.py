import unittest
import sys

sys.path.append('../.')

from toolset.mesh import Mesh
from toolset.mesh import Cell
from toolset.mesh import Node

class MeshTest(unittest.TestCase):
    def test_read_mesh_xml(self):
        mesh = Mesh()
        execption = False

        try:
            mesh.read_mesh_xml('test_data/cube.xml')
        except Exception as e:
            print(e)
            execption = True

        self.assertTrue(not execption)
        self.assertAlmostEqual(1.0, mesh.volume)

        try:
            mesh.read_mesh_xml('test_data/box_vol_regular_refined.xml')
        except Exception as e:
            print(e)
            execption = True

        self.assertTrue(not execption)
        self.assertAlmostEqual(1.0, mesh.volume)

    def test_read_control_csv(self):
        mesh = Mesh()
        execption = False

        try:
            mesh.read_mesh_xml('test_data/cube.xml')
            mesh.read_control_csv('test_data/control.csv')
        except Exception as e:
            print(e)
            execption = True

        self.assertTrue(not execption)

    def test_read_control_xml(self):
        mesh = Mesh()
        execption = False

        try:
            mesh.read_mesh_xml('test_data/box_vol_regular_refined.xml')
            mesh.read_control_xml('test_data/Control_field.xml')
        except Exception as e:
            print(e)
            execption = True

        self.assertTrue(not execption)

    def test_interpolate_cell2node(self):
        mesh = Mesh()
        execption = False

        try:
            mesh.read_mesh_xml('test_data/cube.xml')
            mesh.read_control_csv('test_data/control.csv')
        except Exception as e:
            print(e)
            execption = True

        self.assertTrue(not execption)

        mesh.interpolate_cell2node()

class CellTest(unittest.TestCase):
    def test_calc_volume(self):
        cell = Cell()
        node1 = Node(1, (0.0, 0.0, 0.0))
        node2 = Node(2, (1.0, 0.0, 0.0))
        node3 = Node(3, (0.0, 1.0, 0.0))
        node4 = Node(4, (0.0, 0.0, 1.0))
        nodes = (node1, node2, node3, node4)

        cell.type = 4
        volume = cell.calc_volume(nodes)

        self.assertEqual(1.0/6.0, volume)
