import xml.etree.ElementTree as ET
import xml.dom.minidom as minidom

from mesh import*

from matplotlib import pyplot as plt
from mpl_toolkits.mplot3d import Axes3D
from matplotlib.patches import FancyArrowPatch
from mpl_toolkits.mplot3d import proj3d

import tikzplotlib


class Arrow3D(FancyArrowPatch):
    def __init__(self, xs, ys, zs, *args, **kwargs):
        FancyArrowPatch.__init__(self, (0,0), (0,0), *args, **kwargs)
        self._verts3d = xs, ys, zs

    def draw(self, renderer):
        xs3d, ys3d, zs3d = self._verts3d
        xs, ys, zs = proj3d.proj_transform(xs3d, ys3d, zs3d, renderer.M)
        self.set_positions((xs[0],ys[0]),(xs[1],ys[1]))
        FancyArrowPatch.draw(self, renderer)



class Control_field:
	def __init__(self):
		self.control = []
		self.nodesMesh = []
		self.endPoints = []
	def __str__(self):
		str = ""
		str += "Control:\n"
		str += "NodesMesh:\n"
		str += "EndPoints:"
		return str
	def clear(self):
		self.control = []
		self.nodesMesh = []
		self.endPoints = []

	def create_Lists(self,controlFile, meshFile, scaling):
		print("Reading control file...")
		doc = minidom.parse(controlFile);
		field = doc.getElementsByTagName('field')[0]

		for v in field.getElementsByTagName('value'):
			u_x,u_y,u_z = v.firstChild.data.split(",")
			self.control.insert(len(self.control),[float(u_x),float(u_y),float(u_z)])

		print("Generated control without errors.\n")
		print("Reading mesh file...")

		mesh = Mesh()
		mesh.read_mesh_xml(str(meshFile))

		for n in mesh.nodes:
			#print(n)
			#print(mesh.nodes[n].x_coord)
			self.nodesMesh.insert(len(self.nodesMesh),[float(mesh.nodes[n].x_coord),mesh.nodes[n].y_coord,mesh.nodes[n].z_coord])

		control_scaling = scaling
		print("Control scaling:"+ str(control_scaling))

		print("Length of control: " + str(len(self.control)))
		for n in mesh.nodes:
			#print(n)
			self.endPoints.insert(len(self.endPoints),[self.nodesMesh[n-1][0]+control_scaling*self.control[n-1][0],self.nodesMesh[n-1][1]+control_scaling*self.control[n-1][1],self.nodesMesh[n-1][2]+control_scaling*self.control[n-1][2]])


		print("Generating nodes and endpoints without errors.\n")


	def plot_Control_field(self,nodesMesh,endPoints,scaling,directorySRC,boxlim):
		print("Plotting force field...")

		plt.style.use("ggplot")

		fig = plt.figure(figsize=(10,10))
		ax = fig.add_subplot(111, projection='3d')
		ax.set(xlim=(-boxlim,boxlim), ylim=(-boxlim, boxlim),zlim=(-boxlim,boxlim))
		ax.set_xlabel('x axis')
		ax.set_ylabel('y axis')
		ax.set_zlabel('z axis')
		ax.view_init(azim=-90, elev=0)

		for n in range(1,len(nodesMesh)):
			a = Arrow3D([nodesMesh[n-1][0], endPoints[n-1][0]], [nodesMesh[n-1][1], endPoints[n-1][1]], [nodesMesh[n-1][2], endPoints[n-1][2]], mutation_scale=20, lw=1, arrowstyle="-|>", color="k")
			ax.add_artist(a)

		plt.title('Force field (scaling factor: '+str(scaling) +')')
		plt.grid(False)


		plt.draw()

		print("Generating tikz file...")
		tikzplotlib.save(directorySRC + "control_field.tex")
		plt.savefig(directorySRC + "force_field.png")
		#plt.show()
