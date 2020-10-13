# state file generated using paraview version 5.8.0

# ----------------------------------------------------------------
# setup views used in the visualization
# ----------------------------------------------------------------

# trace generated using paraview version 5.8.0
#
# To ensure correct image size when batch processing, please search 
# for and uncomment the line `# renderView*.ViewSize = [*,*]`

#### import the simple module from the paraview
from paraview.simple import *
#### import argument parsing
import argparse
#### import module fo rgenerating lists of result files
import glob

parser = argparse.ArgumentParser(prog="Generates animation by paraview", description='Needs results directory')
parser.add_argument('results', type=str, help='path to results dir')
#parser.add_argument('out', type=str, help='animation output directory')
args = parser.parse_args()

print(" Generating animation from files in "+ args.results)


#### disable automatic camera reset on 'Show'
paraview.simple._DisableFirstRenderCameraReset()

# Create a new 'Render View'
renderView1 = CreateView('RenderView')
renderView1.ViewSize = [451, 465]
renderView1.AxesGrid = 'GridAxes3DActor'
renderView1.CenterOfRotation = [-4.999999999999449e-05, 0.000129000000000018, -2.2500000000008624e-05]
renderView1.StereoType = 'Crystal Eyes'
renderView1.CameraPosition = [-4.999999999999449e-05, -3.345379669046588, -2.2500000000008624e-05]
renderView1.CameraFocalPoint = [-4.999999999999449e-05, 0.000129000000000018, -2.2500000000008624e-05]
renderView1.CameraViewUp = [0.0, 0.0, 1.0]
renderView1.CameraParallelScale = 0.8658813591048429
renderView1.Background = [1.0, 1.0, 1.0]
renderView1.BackEnd = 'OSPRay raycaster'
renderView1.BackgroundNorth = [1.0, 0.0, 0.0]
renderView1.BackgroundEast = [0.0, 1.0, 0.0]




# ----------------------------------------------------------------
# restore active view
SetActiveView(renderView1)
# ----------------------------------------------------------------

# ----------------------------------------------------------------
# setup the data processing pipelines
# ----------------------------------------------------------------

# create a new 'CSV Reader'

fileNameList_forward = glob.glob(args.results+'plasma_state_batch_1_forward_particles_CPU_*.csv')
print(fileNameList_forward)

plasma_state_batch_1_forward_particles_CPU_csv = CSVReader(FileName=fileNameList_forward)

# create a new 'Table To Points'
tableToPoints2 = TableToPoints(Input=plasma_state_batch_1_forward_particles_CPU_csv)
tableToPoints2.XColumn = '#pos_x'
tableToPoints2.YColumn = '#pos_y'
tableToPoints2.ZColumn = '#pos_z'



# ----------------------------------------------------------------
# setup the visualization in view 'renderView1'
# ----------------------------------------------------------------

# show data from tableToPoints2
tableToPoints2Display = Show(tableToPoints2, renderView1, 'GeometryRepresentation')

# trace defaults for the display properties.
tableToPoints2Display.Representation = 'Surface'
tableToPoints2Display.ColorArrayName = [None, '']
tableToPoints2Display.DiffuseColor = [0.0, 0.0, 0.0]
tableToPoints2Display.OSPRayScaleArray = '#cell_id'
tableToPoints2Display.OSPRayScaleFunction = 'PiecewiseFunction'
tableToPoints2Display.SelectOrientationVectors = 'None'
tableToPoints2Display.ScaleFactor = 0.099993
tableToPoints2Display.SelectScaleArray = 'None'
tableToPoints2Display.GlyphType = 'Arrow'
tableToPoints2Display.GlyphTableIndexArray = 'None'
tableToPoints2Display.GaussianRadius = 0.0049996500000000004
tableToPoints2Display.SetScaleArray = ['POINTS', '#cell_id']
tableToPoints2Display.ScaleTransferFunction = 'PiecewiseFunction'
tableToPoints2Display.OpacityArray = ['POINTS', '#cell_id']
tableToPoints2Display.OpacityTransferFunction = 'PiecewiseFunction'
tableToPoints2Display.DataAxesGrid = 'GridAxesRepresentation'
tableToPoints2Display.PolarAxes = 'PolarAxesRepresentation'

# ----------------------------------------------------------------
# finally, restore active source
SetActiveSource(None)
SetActiveView(renderView1)
# ----------------------------------------------------------------

print("Starting to save animation\n")

#save animation
renderView = GetActiveView()
animationScene = GetAnimationScene()
#animationScene.StartTime = 0
#animationScene.EndTime = 19
animationScene.PlayMode = 'Snap To TimeSteps'
animationScene.GoToFirst() #output happens here
print("Number of timesteps: "+str(animationScene.EndTime))
i = 0
while True:
   imageName = "image_%04d.jpg" % (i)
   renderView.Update()
   SaveScreenshot(imageName, renderView, ImageResolution=[1920, 1080])
   print("Timestep: " + str(i))
   if i == animationScene.EndTime:
      break
   animationScene.GoToNext()
   i = i + 1


