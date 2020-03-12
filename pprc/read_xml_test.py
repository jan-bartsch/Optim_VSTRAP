import xml.etree.ElementTree as ET
import xml.dom.minidom as minidom

tree = ET.parse("../data/box_shifting/interpolated_control_field.xml");
root = tree.getroot()

#for child in root:
#    print(child.tag, child.attrib, child.text)


#list of control vectors
control = []


doc = minidom.parse("../data/box_shifting/interpolated_control_field.xml");
field = doc.getElementsByTagName('field')[0]

#value = field.getElementsByTagName('value')[1]
#print(value.getAttribute('node_number'))


for v in field.getElementsByTagName('value'):
	print(v.firstChild.data)
	u_x,u_y,u_z = v.firstChild.data.split(",")
	control.insert(1,[float(u_x),float(u_y),float(u_z)])


print(control)
