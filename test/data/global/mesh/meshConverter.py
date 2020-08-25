import numpy as np
import argparse

parser = argparse.ArgumentParser(description="Converts gmsh file to xml file.")

#Necessary arguments
parser.add_argument("meshfile", metavar="-mf", help="mesh file name (no extension)") 

#Mutually exclusive arguments
group = parser.add_mutually_exclusive_group()
group.add_argument("-v", "--volume", help="for volume mesh", action="store_true")
group.add_argument("-s", "--surface", help="for surface mesh", action="store_true")

args = parser.parse_args()
def writeFile(inputName, outputName, meshType):
    inputFile = open(""+inputName+".msh","r")
    outputFile = open(""+outputName+".xml","w+")
    fLines = inputFile.readlines()
    
    writeHeader(fLines, outputFile, meshType)
    writePhysicalNames(fLines, outputFile)
    writeNodes(fLines, outputFile)
    writeElements(fLines, outputFile)
    writeEnd(outputFile)

    inputFile.close()
    outputFile.close()

def writeHeader(fLines, outputFile, meshType):
    lines = findInFile(fLines,"MeshFormat")
    splitLine = lines[0].rstrip().split(" ")
    if (len(splitLine) != 3):
        raise ValueError('Issue in header writer')
    if (meshType == "volume"):
        meshType = "volume_mesh"
    elif (meshType == "surface"):
        meshType = "surface_mesh"
    else:
        raise ValueError('Entered mesh type is not recognized! Given <'+meshType+'>')

    if (splitLine[0] != "2.2"):
        raise ValueError('Version number of mesh file is not readable! (needs to be 2.2)')
        
    outputFile.write('<mesh  type="Gmsh">\n')
    outputFile.write('\n')
    outputFile.write('<mesh  version_number ="'+splitLine[0]+'" file_type ="'+splitLine[1]+'" data_size ="'+splitLine[2]+'" mesh_type="'+meshType+'"/>\n')
    outputFile.write("\n")

def writePhysicalNames(fLines, outputFile):
    lines = findInFile(fLines,"PhysicalNames")
    outputFile.write('<physical_names number_of_names="'+lines[0].rstrip()+'">\n')
    outputFile.write('\n')
    for i in range(1, len(lines)):
        writePhysicalName(lines[i], outputFile)
    outputFile.write('\n')
    outputFile.write('</physical_names>\n')
    outputFile.write('\n')

def writePhysicalName(line, outputFile):
    splitLine = line.rstrip().split(" ")
    if (len(splitLine) != 3):
        raise ValueError('Issue in physical name writer')
    outputFile.write('<physical_name physical_dimension="'+splitLine[0]+'" physical_number="'+splitLine[1]+'" physical_name='+splitLine[2]+'/>\n')

def writeNodes(fLines, outputFile):
    lines = findInFile(fLines,"Nodes")
    outputFile.write('<nodes number_of_nodes="'+lines[0].rstrip()+'">\n')
    outputFile.write('\n')
    for i in range(1, len(lines)):
        writeNode(lines[i], outputFile)
    outputFile.write('\n')
    outputFile.write('</nodes>\n')
    outputFile.write('\n')

def writeNode(line, outputFile):
    splitLine = line.rstrip().split(" ")
    if (len(splitLine) != 4):
        raise ValueError('Issue in node writer')
    outputFile.write('<node node_number="'+splitLine[0]+'" x_coord="'+splitLine[1]+'" y_coord="'+splitLine[2]+'" z_coord="'+splitLine[3]+'"/>\n')

def writeElements(fLines, outputFile):
    lines = findInFile(fLines,"Elements")
    outputFile.write('<elements number_of_elements="'+lines[0].rstrip()+'">\n')
    outputFile.write('\n')
    for i in range(1, len(lines)):
        writeElement(lines[i], outputFile)
    outputFile.write('\n')
    outputFile.write('</elements>\n')
    outputFile.write('\n')

def writeElement(line, outputFile):
    splitLine = line.rstrip().split(" ")
    elementLine = groupElementLine(splitLine)
    outputFile.write('<element elem_number="'+elementLine[0]+'" elm_type="'+elementLine[1]+'">\n')
    outputFile.write('<tags number_of_tags="'+elementLine[2]+'">\n')
    outputFile.write(elementLine[3] + '\n')
    outputFile.write('</tags>\n')
    outputFile.write('<nodes>\n')
    outputFile.write(elementLine[4] + '\n')
    outputFile.write('</nodes>\n')
    outputFile.write("</element>\n")

def groupElementLine(splitLine):
    print(splitLine)
    elementType = int(splitLine[1])
    tagNumber = int(splitLine[2])

    if (tagNumber == 0):
        tags = ""
    else:  
        tags = splitLine[3]
        for i in range(1, tagNumber):
            tags = tags + "," + splitLine[3+i]
    
    nodes = splitLine[3+tagNumber]
    for i in range(1, nodesPerElementType(elementType)):
        nodes = nodes + "," + splitLine[3+tagNumber+i]
    
    return [splitLine[0], splitLine[1], splitLine[2], tags, nodes]

def nodesPerElementType(elementType):
    if (elementType == 1):
         return 2
    elif (elementType == 2):
         return 3
    elif (elementType == 3):
         return 4
    elif (elementType == 4):
         return 4
    elif (elementType == 5):
         return 8
    elif (elementType == 6):
         return 6
    elif (elementType == 7):
         return 5
    else:
        raise ValueError('Element Type not recognized, given <'+str(elementType)+'>')
         

def writeEnd(outputFile):
    outputFile.write("</mesh>\n")

def findInFile(fLines, keyword):
    result = []
    found = False
    for line in fLines:
        if (line == "$End"+keyword+"\n"):
            return result
        if (found):
            result.append(line)
        if (line == "$"+keyword+"\n"):
            found = True
    if (found):
        raise ValueError('End not found')
    else:
        raise ValueError('Keyword <'+keyword+'> not found')
    

def main():
    if args.volume:
        meshType = "volume"
    elif args.surface:
        meshType = "surface"
    else:
        raise ValueError("No mesh type given: use -v for volume or -s for surface")

    writeFile(args.meshfile, args.meshfile, meshType)

if (__name__ == "__main__"):
    main()
