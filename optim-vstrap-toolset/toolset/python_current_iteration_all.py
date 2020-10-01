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

# Create a new 'Bar Chart View'
barChartView1 = CreateView('XYBarChartView')
barChartView1.ViewSize = [394, 295]
barChartView1.ChartTitle = 'Distribution of particles at timestep ${TIME}'
barChartView1.LegendPosition = [297, 253]
barChartView1.LeftAxisTitle = 'numbers of particle (absolute numbers)'
barChartView1.LeftAxisRangeMaximum = 250.0
barChartView1.BottomAxisTitle = 'position (x-direction)'
barChartView1.BottomAxisRangeMinimum = -0.6000000000000001
barChartView1.BottomAxisRangeMaximum = 0.6000000000000001
barChartView1.RightAxisTitleFontFile = 'Arial'
barChartView1.RightAxisRangeMaximum = 6.66
barChartView1.TopAxisRangeMaximum = 6.66

# Create a new 'Bar Chart View'
barChartView2 = CreateView('XYBarChartView')
barChartView2.ViewSize = [341, 218]
barChartView2.LegendPosition = [297, 181]
barChartView2.LeftAxisRangeMaximum = 250.0
barChartView2.BottomAxisTitle = 'position (y-direction)'
barChartView2.BottomAxisRangeMinimum = -0.6000000000000001
barChartView2.BottomAxisRangeMaximum = 0.6000000000000001
barChartView2.RightAxisTitleFontFile = 'Arial'
barChartView2.RightAxisRangeMaximum = 6.66
barChartView2.TopAxisRangeMaximum = 6.66

# Create a new 'Bar Chart View'
barChartView3 = CreateView('XYBarChartView')
barChartView3.ViewSize = [341, 218]
barChartView3.LegendPosition = [297, 181]
barChartView3.LeftAxisRangeMaximum = 250.0
barChartView3.BottomAxisTitle = 'position (z-direction)'
barChartView3.BottomAxisRangeMinimum = -0.6000000000000001
barChartView3.BottomAxisRangeMaximum = 0.6000000000000001
barChartView3.RightAxisTitleFontFile = 'Arial'
barChartView3.RightAxisRangeMaximum = 6.66
barChartView3.TopAxisRangeMaximum = 6.66

# Create a new 'Bar Chart View'
barChartView4 = CreateView('XYBarChartView')
barChartView4.ViewSize = [287, 295]
barChartView4.LegendPosition = [296, 256]
barChartView4.LeftAxisRangeMaximum = 600.0
barChartView4.BottomAxisTitle = 'velocity (x-direction)'
barChartView4.BottomAxisRangeMinimum = -1000.0
barChartView4.BottomAxisRangeMaximum = 1000.0
barChartView4.RightAxisTitleFontFile = 'Arial'
barChartView4.RightAxisRangeMaximum = 6.66
barChartView4.TopAxisRangeMaximum = 6.66

# Create a new 'Bar Chart View'
barChartView5 = CreateView('XYBarChartView')
barChartView5.ViewSize = [340, 218]
barChartView5.LegendPosition = [243, 182]
barChartView5.LeftAxisRangeMaximum = 500.0
barChartView5.BottomAxisTitle = 'velocity (y-direction)'
barChartView5.BottomAxisRangeMinimum = -1000.0
barChartView5.BottomAxisRangeMaximum = 1000.0
barChartView5.RightAxisTitleFontFile = 'Arial'
barChartView5.RightAxisRangeMaximum = 6.66
barChartView5.TopAxisRangeMaximum = 6.66

# Create a new 'Bar Chart View'
barChartView6 = CreateView('XYBarChartView')
barChartView6.ViewSize = [340, 218]
barChartView6.LegendPosition = [296, 181]
barChartView6.LeftAxisRangeMaximum = 500.0
barChartView6.BottomAxisTitle = 'velocity (z-direction)'
barChartView6.BottomAxisRangeMinimum = -1000.0
barChartView6.BottomAxisRangeMaximum = 1500.0
barChartView6.RightAxisTitleFontFile = 'Arial'
barChartView6.RightAxisRangeMaximum = 6.66
barChartView6.TopAxisRangeMaximum = 6.66

# Create a new 'Bar Chart View'
barChartView7 = CreateView('XYBarChartView')
barChartView7.ViewSize = [793, 397]
barChartView7.LegendPosition = [678, 355]
barChartView7.LeftAxisUseCustomRange = 1
barChartView7.LeftAxisRangeMinimum = 157.14285564422607
barChartView7.LeftAxisRangeMaximum = 5157.142855644226
barChartView7.BottomAxisUseCustomRange = 1
barChartView7.BottomAxisRangeMinimum = -2709.589035987854
barChartView7.BottomAxisRangeMaximum = 1790.410964012146
barChartView7.RightAxisUseCustomRange = 1
barChartView7.RightAxisRangeMaximum = 6.66
barChartView7.TopAxisUseCustomRange = 1
barChartView7.TopAxisRangeMaximum = 6.66

# Create a new 'Quartile Chart View'
quartileChartView1 = CreateView('QuartileChartView')
quartileChartView1.ViewSize = [361, 302]
quartileChartView1.LegendPosition = [160, 355]
quartileChartView1.LeftAxisRangeMinimum = -3.5
quartileChartView1.LeftAxisRangeMaximum = -1.5
quartileChartView1.BottomAxisRangeMaximum = 20.0
quartileChartView1.RightAxisTitleFontFile = 'Arial'
quartileChartView1.RightAxisRangeMaximum = 6.66
quartileChartView1.TopAxisRangeMaximum = 6.66

# Create a new 'Quartile Chart View'
quartileChartView2 = CreateView('QuartileChartView')
quartileChartView2.ViewSize = [265, 302]
quartileChartView2.LegendPosition = [76, 355]
quartileChartView2.LeftAxisRangeMinimum = -5.5
quartileChartView2.LeftAxisRangeMaximum = -2.5
quartileChartView2.BottomAxisRangeMaximum = 20.0
quartileChartView2.RightAxisTitleFontFile = 'Arial'
quartileChartView2.RightAxisRangeMaximum = 6.66
quartileChartView2.TopAxisRangeMaximum = 6.66

# Create a new 'Quartile Chart View'
quartileChartView3 = CreateView('QuartileChartView')
quartileChartView3.ViewSize = [265, 302]
quartileChartView3.LegendPosition = [76, 355]
quartileChartView3.LeftAxisRangeMinimum = -1.5
quartileChartView3.BottomAxisRangeMaximum = 20.0
quartileChartView3.RightAxisTitleFontFile = 'Arial'
quartileChartView3.RightAxisRangeMaximum = 6.66
quartileChartView3.TopAxisRangeMaximum = 6.66

# Create a new 'Quartile Chart View'
quartileChartView4 = CreateView('QuartileChartView')
quartileChartView4.ViewSize = [450, 465]
quartileChartView4.LegendPosition = [288, 439]
quartileChartView4.LeftAxisRangeMinimum = 0.0001
quartileChartView4.LeftAxisRangeMaximum = 0.0012000000000000001
quartileChartView4.BottomAxisRangeMaximum = 20.0
quartileChartView4.RightAxisRangeMaximum = 6.66
quartileChartView4.TopAxisRangeMaximum = 6.66

# Create a new 'Quartile Chart View'
quartileChartView5 = CreateView('QuartileChartView')
quartileChartView5.ViewSize = [1596, 397]
quartileChartView5.LegendPosition = [1438, 338]
quartileChartView5.LeftAxisRangeMinimum = -0.264
quartileChartView5.LeftAxisRangeMaximum = -0.259
quartileChartView5.BottomAxisRangeMaximum = 9.0
quartileChartView5.RightAxisRangeMaximum = 6.66
quartileChartView5.TopAxisRangeMaximum = 6.66

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

# get the material library
materialLibrary1 = GetMaterialLibrary()

# Create a new 'Render View'
renderView2 = CreateView('RenderView')
renderView2.ViewSize = [793, 397]
renderView2.AxesGrid = 'GridAxes3DActor'
renderView2.CenterOfRotation = [-0.0021829999999999905, -6.649999999999712e-05, 6.499999999992623e-06]
renderView2.StereoType = 'Crystal Eyes'
renderView2.CameraPosition = [-0.0021829999999999905, -6.649999999999712e-05, 3.340927815551877]
renderView2.CameraFocalPoint = [-0.0021829999999999905, -6.649999999999712e-05, 6.499999999992623e-06]
renderView2.CameraFocalDisk = 1.0
renderView2.CameraParallelScale = 0.8646940646537942
renderView2.BackEnd = 'OSPRay raycaster'
renderView2.OSPRayMaterialLibrary = materialLibrary1

SetActiveView(None)

# ----------------------------------------------------------------
# setup view layouts
# ----------------------------------------------------------------

# create new layout object 'Layout #1'
layout1 = CreateLayout(name='Layout #1')
layout1.SplitHorizontal(0, 0.568040)
layout1.SplitVertical(1, 0.597448)
layout1.SplitHorizontal(3, 0.500000)
layout1.AssignView(7, renderView1)
layout1.AssignView(8, quartileChartView4)
layout1.SplitHorizontal(4, 0.401752)
layout1.AssignView(9, quartileChartView1)
layout1.SplitHorizontal(10, 0.500000)
layout1.AssignView(21, quartileChartView2)
layout1.AssignView(22, quartileChartView3)
layout1.SplitVertical(2, 0.393029)
layout1.SplitHorizontal(5, 0.576812)
layout1.AssignView(11, barChartView1)
layout1.AssignView(12, barChartView4)
layout1.SplitVertical(6, 0.500000)
layout1.SplitHorizontal(13, 0.500000)
layout1.AssignView(27, barChartView2)
layout1.AssignView(28, barChartView5)
layout1.SplitHorizontal(14, 0.500000)
layout1.AssignView(29, barChartView3)
layout1.AssignView(30, barChartView6)

# create new layout object 'Layout #2'
layout2 = CreateLayout(name='Layout #2')
layout2.SplitVertical(0, 0.500000)
layout2.SplitHorizontal(1, 0.500000)
layout2.AssignView(3, renderView2)
layout2.AssignView(4, barChartView7)
layout2.AssignView(2, quartileChartView5)

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

# create a new 'Histogram'
histogram5 = Histogram(Input=plasma_state_batch_1_forward_particles_CPU_csv)
histogram5.SelectInputArray = ['ROWS', '#vel_x']
histogram5.BinCount = 60
histogram5.CustomBinRanges = [1.0, 64.0]

# create a new 'Histogram'
histogram6 = Histogram(Input=plasma_state_batch_1_forward_particles_CPU_csv)
histogram6.SelectInputArray = ['ROWS', '#vel_y']
histogram6.BinCount = 70
histogram6.CustomBinRanges = [1.0, 64.0]

# create a new 'Histogram'
histogram4 = Histogram(Input=plasma_state_batch_1_forward_particles_CPU_csv)
histogram4.SelectInputArray = ['ROWS', '#pos_z']
histogram4.BinCount = 55
histogram4.UseCustomBinRanges = 1
histogram4.CustomBinRanges = [-0.5, 0.5]

# create a new 'Table To Points'
tableToPoints3_velocity = TableToPoints(Input=plasma_state_batch_1_forward_particles_CPU_csv)
tableToPoints3_velocity.XColumn = '#vel_x'
tableToPoints3_velocity.YColumn = '#vel_y'
tableToPoints3_velocity.ZColumn = '#vel_z'

# create a new 'Histogram'
histogram1 = Histogram(Input=plasma_state_batch_1_forward_particles_CPU_csv)
histogram1.SelectInputArray = ['ROWS', '#pos_x']
histogram1.BinCount = 55
histogram1.UseCustomBinRanges = 1
histogram1.CustomBinRanges = [-0.5, 0.5]

# create a new 'Histogram'
histogram7 = Histogram(Input=plasma_state_batch_1_forward_particles_CPU_csv)
histogram7.SelectInputArray = ['ROWS', '#vel_z']
histogram7.BinCount = 70
histogram7.CustomBinRanges = [1.0, 64.0]

# create a new 'CSV Reader'


fileNameList_backward = glob.glob(args.results+'plasma_state_batch_1_adjoint_particles_CPU_*.csv')
print(fileNameList_backward)

plasma_state_batch_1_adjoint_particles_CPU_csv = CSVReader(FileName=fileNameList_backward)

# create a new 'Table To Points'
tableToPoints1 = TableToPoints(Input=plasma_state_batch_1_adjoint_particles_CPU_csv)
tableToPoints1.XColumn = '#pos_x'
tableToPoints1.YColumn = '#pos_y'
tableToPoints1.ZColumn = '#pos_z'

# create a new 'Table To Points'
tableToPoints3_velocity_1 = TableToPoints(Input=plasma_state_batch_1_adjoint_particles_CPU_csv)
tableToPoints3_velocity_1.XColumn = '#vel_x'
tableToPoints3_velocity_1.YColumn = '#vel_y'
tableToPoints3_velocity_1.ZColumn = '#vel_z'

# create a new 'Histogram'
histogram2 = Histogram(Input=plasma_state_batch_1_adjoint_particles_CPU_csv)
histogram2.SelectInputArray = ['ROWS', '#vel_z']
histogram2.BinCount = 86
histogram2.CustomBinRanges = [-300.0, 300.0]

# create a new 'Plot Data Over Time'
plotDataOverTime2 = PlotDataOverTime(Input=plasma_state_batch_1_adjoint_particles_CPU_csv)
plotDataOverTime2.FieldAssociation = 'Rows'

# create a new 'Plot Data Over Time'
plotDataOverTime1 = PlotDataOverTime(Input=plasma_state_batch_1_forward_particles_CPU_csv)
plotDataOverTime1.FieldAssociation = 'Rows'

# create a new 'Histogram'
histogram3 = Histogram(Input=plasma_state_batch_1_forward_particles_CPU_csv)
histogram3.SelectInputArray = ['ROWS', '#pos_y']
histogram3.BinCount = 55
histogram3.UseCustomBinRanges = 1
histogram3.CustomBinRanges = [-0.5, 0.5]

# ----------------------------------------------------------------
# setup the visualization in view 'barChartView1'
# ----------------------------------------------------------------

# show data from histogram1
histogram1Display = Show(histogram1, barChartView1, 'XYBarChartRepresentation')

# trace defaults for the display properties.
histogram1Display.CompositeDataSetIndex = [0]
histogram1Display.AttributeType = 'Row Data'
histogram1Display.UseIndexForXAxis = 0
histogram1Display.XArrayName = 'bin_extents'
histogram1Display.SeriesVisibility = ['bin_values']
histogram1Display.SeriesLabel = ['bin_extents', 'bin_extents', 'bin_values', 'n']
histogram1Display.SeriesColor = ['bin_extents', '0', '0', '0', 'bin_values', '0', '0', '0']
histogram1Display.SeriesPlotCorner = ['bin_extents', '0', 'bin_values', '0']
histogram1Display.SeriesLabelPrefix = ''

# ----------------------------------------------------------------
# setup the visualization in view 'barChartView2'
# ----------------------------------------------------------------

# show data from histogram3
histogram3Display = Show(histogram3, barChartView2, 'XYBarChartRepresentation')

# trace defaults for the display properties.
histogram3Display.CompositeDataSetIndex = [0]
histogram3Display.AttributeType = 'Row Data'
histogram3Display.UseIndexForXAxis = 0
histogram3Display.XArrayName = 'bin_extents'
histogram3Display.SeriesVisibility = ['bin_values']
histogram3Display.SeriesLabel = ['bin_extents', 'bin_extents', 'bin_values', 'n']
histogram3Display.SeriesColor = ['bin_extents', '0', '0', '0', 'bin_values', '0', '0', '0']
histogram3Display.SeriesPlotCorner = ['bin_extents', '0', 'bin_values', '0']
histogram3Display.SeriesLabelPrefix = ''

# ----------------------------------------------------------------
# setup the visualization in view 'barChartView3'
# ----------------------------------------------------------------

# show data from histogram4
histogram4Display = Show(histogram4, barChartView3, 'XYBarChartRepresentation')

# trace defaults for the display properties.
histogram4Display.CompositeDataSetIndex = [0]
histogram4Display.AttributeType = 'Row Data'
histogram4Display.UseIndexForXAxis = 0
histogram4Display.XArrayName = 'bin_extents'
histogram4Display.SeriesVisibility = ['bin_values']
histogram4Display.SeriesLabel = ['bin_extents', 'bin_extents', 'bin_values', 'n']
histogram4Display.SeriesColor = ['bin_extents', '0', '0', '0', 'bin_values', '0', '0', '0']
histogram4Display.SeriesPlotCorner = ['bin_extents', '0', 'bin_values', '0']
histogram4Display.SeriesLabelPrefix = ''

# ----------------------------------------------------------------
# setup the visualization in view 'barChartView4'
# ----------------------------------------------------------------

# show data from histogram5
histogram5Display = Show(histogram5, barChartView4, 'XYBarChartRepresentation')

# trace defaults for the display properties.
histogram5Display.CompositeDataSetIndex = [0]
histogram5Display.AttributeType = 'Row Data'
histogram5Display.UseIndexForXAxis = 0
histogram5Display.XArrayName = 'bin_extents'
histogram5Display.SeriesVisibility = ['bin_values']
histogram5Display.SeriesLabel = ['bin_extents', 'bin_extents', 'bin_values', 'n']
histogram5Display.SeriesColor = ['bin_extents', '0', '0', '0', 'bin_values', '0', '0', '0']
histogram5Display.SeriesPlotCorner = ['bin_extents', '0', 'bin_values', '0']
histogram5Display.SeriesLabelPrefix = ''

# ----------------------------------------------------------------
# setup the visualization in view 'barChartView5'
# ----------------------------------------------------------------

# show data from histogram6
histogram6Display = Show(histogram6, barChartView5, 'XYBarChartRepresentation')

# trace defaults for the display properties.
histogram6Display.CompositeDataSetIndex = [0]
histogram6Display.AttributeType = 'Row Data'
histogram6Display.UseIndexForXAxis = 0
histogram6Display.XArrayName = 'bin_extents'
histogram6Display.SeriesVisibility = ['bin_values']
histogram6Display.SeriesLabel = ['bin_extents', 'bin_extents', 'bin_values', 'n']
histogram6Display.SeriesColor = ['bin_extents', '1', '1', '1', 'bin_values', '0', '0', '0']
histogram6Display.SeriesPlotCorner = ['bin_extents', '0', 'bin_values', '0']
histogram6Display.SeriesLabelPrefix = ''

# ----------------------------------------------------------------
# setup the visualization in view 'barChartView6'
# ----------------------------------------------------------------

# show data from histogram7
histogram7Display = Show(histogram7, barChartView6, 'XYBarChartRepresentation')

# trace defaults for the display properties.
histogram7Display.CompositeDataSetIndex = [0]
histogram7Display.AttributeType = 'Row Data'
histogram7Display.UseIndexForXAxis = 0
histogram7Display.XArrayName = 'bin_extents'
histogram7Display.SeriesVisibility = ['bin_values']
histogram7Display.SeriesLabel = ['bin_extents', 'bin_extents', 'bin_values', 'n']
histogram7Display.SeriesColor = ['bin_extents', '0', '0', '0', 'bin_values', '0', '0', '0']
histogram7Display.SeriesPlotCorner = ['bin_extents', '0', 'bin_values', '0']
histogram7Display.SeriesLabelPrefix = ''

# ----------------------------------------------------------------
# setup the visualization in view 'barChartView7'
# ----------------------------------------------------------------

# show data from histogram2
histogram2Display = Show(histogram2, barChartView7, 'XYBarChartRepresentation')

# trace defaults for the display properties.
histogram2Display.CompositeDataSetIndex = [0]
histogram2Display.AttributeType = 'Row Data'
histogram2Display.UseIndexForXAxis = 0
histogram2Display.XArrayName = 'bin_extents'
histogram2Display.SeriesVisibility = ['bin_values']
histogram2Display.SeriesLabel = ['bin_extents', 'bin_extents', 'bin_values', 'bin_values']
histogram2Display.SeriesColor = ['bin_extents', '0', '0', '0', 'bin_values', '0.89', '0.1', '0.11']
histogram2Display.SeriesPlotCorner = ['bin_extents', '0', 'bin_values', '0']
histogram2Display.SeriesLabelPrefix = ''

# ----------------------------------------------------------------
# setup the visualization in view 'quartileChartView1'
# ----------------------------------------------------------------

# show data from plotDataOverTime1
plotDataOverTime1Display = Show(plotDataOverTime1, quartileChartView1, 'QuartileChartRepresentation')

# trace defaults for the display properties.
plotDataOverTime1Display.AttributeType = 'Row Data'
plotDataOverTime1Display.XArrayName = 'avg(#pos_x)'
plotDataOverTime1Display.SeriesVisibility = ['#vel_x (stats)']
plotDataOverTime1Display.SeriesLabel = ['#cell_id (stats)', '#cell_id (stats)', '#charge_number (stats)', '#charge_number (stats)', '#mass (stats)', '#mass (stats)', '#pos_x (stats)', '#pos_x (stats)', '#pos_y (stats)', '#pos_y (stats)', '#pos_z (stats)', '#pos_z (stats)', '#potential (stats)', '#potential (stats)', '#vel_x (stats)', '#vel_x (stats)', '#vel_y (stats)', '#vel_y (stats)', '#vel_z (stats)', '#vel_z (stats)', '#weight (stats)', '#weight (stats)', 'N (stats)', 'N (stats)', 'Time (stats)', 'Time (stats)', 'vtkValidPointMask (stats)', 'vtkValidPointMask (stats)', '#charge (stats)', '#charge (stats)']
plotDataOverTime1Display.SeriesColor = ['#cell_id (stats)', '0', '0', '0', '#charge_number (stats)', '0.889998', '0.100008', '0.110002', '#mass (stats)', '0.220005', '0.489998', '0.719997', '#pos_x (stats)', '0.300008', '0.689998', '0.289998', '#pos_y (stats)', '0.6', '0.310002', '0.639994', '#pos_z (stats)', '1', '0.500008', '0', '#potential (stats)', '0.650004', '0.340002', '0.160006', '#vel_x (stats)', '0', '0', '0', '#vel_y (stats)', '0.889998', '0.100008', '0.110002', '#vel_z (stats)', '0.220005', '0.489998', '0.719997', '#weight (stats)', '0.300008', '0.689998', '0.289998', 'N (stats)', '0.6', '0.310002', '0.639994', 'Time (stats)', '1', '0.500008', '0', 'vtkValidPointMask (stats)', '0.650004', '0.340002', '0.160006', '#charge (stats)', '0', '0', '0']
plotDataOverTime1Display.SeriesPlotCorner = ['#cell_id (stats)', '0', '#charge (stats)', '0', '#charge_number (stats)', '0', '#mass (stats)', '0', '#pos_x (stats)', '0', '#pos_y (stats)', '0', '#pos_z (stats)', '0', '#potential (stats)', '0', '#vel_x (stats)', '0', '#vel_y (stats)', '0', '#vel_z (stats)', '0', '#weight (stats)', '0', 'N (stats)', '0', 'Time (stats)', '0', 'vtkValidPointMask (stats)', '0']
plotDataOverTime1Display.SeriesLabelPrefix = ''
plotDataOverTime1Display.SeriesLineStyle = ['#cell_id (stats)', '1', '#charge (stats)', '1', '#charge_number (stats)', '1', '#mass (stats)', '1', '#pos_x (stats)', '1', '#pos_y (stats)', '1', '#pos_z (stats)', '1', '#potential (stats)', '1', '#vel_x (stats)', '1', '#vel_y (stats)', '1', '#vel_z (stats)', '1', '#weight (stats)', '1', 'N (stats)', '1', 'Time (stats)', '1', 'vtkValidPointMask (stats)', '1']
plotDataOverTime1Display.SeriesLineThickness = ['#cell_id (stats)', '2', '#charge (stats)', '2', '#charge_number (stats)', '2', '#mass (stats)', '2', '#pos_x (stats)', '2', '#pos_y (stats)', '2', '#pos_z (stats)', '2', '#potential (stats)', '2', '#vel_x (stats)', '2', '#vel_y (stats)', '2', '#vel_z (stats)', '2', '#weight (stats)', '2', 'N (stats)', '2', 'Time (stats)', '2', 'vtkValidPointMask (stats)', '2']
plotDataOverTime1Display.SeriesMarkerStyle = ['#cell_id (stats)', '0', '#charge (stats)', '0', '#charge_number (stats)', '0', '#mass (stats)', '0', '#pos_x (stats)', '0', '#pos_y (stats)', '0', '#pos_z (stats)', '0', '#potential (stats)', '0', '#vel_x (stats)', '0', '#vel_y (stats)', '0', '#vel_z (stats)', '0', '#weight (stats)', '0', 'N (stats)', '0', 'Time (stats)', '0', 'vtkValidPointMask (stats)', '0']
plotDataOverTime1Display.SeriesMarkerSize = ['#cell_id (stats)', '4', '#charge (stats)', '4', '#mass (stats)', '4', '#pos_x (stats)', '4', '#pos_y (stats)', '4', '#pos_z (stats)', '4', '#potential (stats)', '4', '#vel_x (stats)', '4', '#vel_y (stats)', '4', '#vel_z (stats)', '4', '#weight (stats)', '4', 'N (stats)', '4', 'Time (stats)', '4', 'vtkValidPointMask (stats)', '4']
plotDataOverTime1Display.ShowQuartiles = 0
plotDataOverTime1Display.ShowRanges = 0

# ----------------------------------------------------------------
# setup the visualization in view 'quartileChartView2'
# ----------------------------------------------------------------

# show data from plotDataOverTime1
plotDataOverTime1Display_1 = Show(plotDataOverTime1, quartileChartView2, 'QuartileChartRepresentation')

# trace defaults for the display properties.
plotDataOverTime1Display_1.AttributeType = 'Row Data'
plotDataOverTime1Display_1.XArrayName = 'avg(#pos_y)'
plotDataOverTime1Display_1.SeriesVisibility = ['#vel_y (stats)']
plotDataOverTime1Display_1.SeriesLabel = ['#cell_id (stats)', '#cell_id (stats)', '#charge_number (stats)', '#charge_number (stats)', '#mass (stats)', '#mass (stats)', '#pos_x (stats)', '#pos_x (stats)', '#pos_y (stats)', '#pos_y (stats)', '#pos_z (stats)', '#pos_z (stats)', '#potential (stats)', '#potential (stats)', '#vel_x (stats)', '#vel_x (stats)', '#vel_y (stats)', '#vel_y (stats)', '#vel_z (stats)', '#vel_z (stats)', '#weight (stats)', '#weight (stats)', 'N (stats)', 'N (stats)', 'Time (stats)', 'Time (stats)', 'vtkValidPointMask (stats)', 'vtkValidPointMask (stats)', '#charge (stats)', '#charge (stats)']
plotDataOverTime1Display_1.SeriesColor = ['#cell_id (stats)', '0', '0', '0', '#charge_number (stats)', '0.889998', '0.100008', '0.110002', '#mass (stats)', '0.220005', '0.489998', '0.719997', '#pos_x (stats)', '0.300008', '0.689998', '0.289998', '#pos_y (stats)', '0.6', '0.310002', '0.639994', '#pos_z (stats)', '1', '0.500008', '0', '#potential (stats)', '0.650004', '0.340002', '0.160006', '#vel_x (stats)', '0', '0', '0', '#vel_y (stats)', '0', '0', '0', '#vel_z (stats)', '0.220005', '0.489998', '0.719997', '#weight (stats)', '0.300008', '0.689998', '0.289998', 'N (stats)', '0.6', '0.310002', '0.639994', 'Time (stats)', '1', '0.500008', '0', 'vtkValidPointMask (stats)', '0.650004', '0.340002', '0.160006', '#charge (stats)', '0', '0', '0']
plotDataOverTime1Display_1.SeriesPlotCorner = ['#cell_id (stats)', '0', '#charge (stats)', '0', '#charge_number (stats)', '0', '#mass (stats)', '0', '#pos_x (stats)', '0', '#pos_y (stats)', '0', '#pos_z (stats)', '0', '#potential (stats)', '0', '#vel_x (stats)', '0', '#vel_y (stats)', '0', '#vel_z (stats)', '0', '#weight (stats)', '0', 'N (stats)', '0', 'Time (stats)', '0', 'vtkValidPointMask (stats)', '0']
plotDataOverTime1Display_1.SeriesLabelPrefix = ''
plotDataOverTime1Display_1.SeriesLineStyle = ['#cell_id (stats)', '1', '#charge (stats)', '1', '#charge_number (stats)', '1', '#mass (stats)', '1', '#pos_x (stats)', '1', '#pos_y (stats)', '1', '#pos_z (stats)', '1', '#potential (stats)', '1', '#vel_x (stats)', '1', '#vel_y (stats)', '1', '#vel_z (stats)', '1', '#weight (stats)', '1', 'N (stats)', '1', 'Time (stats)', '1', 'vtkValidPointMask (stats)', '1']
plotDataOverTime1Display_1.SeriesLineThickness = ['#cell_id (stats)', '2', '#charge (stats)', '2', '#charge_number (stats)', '2', '#mass (stats)', '2', '#pos_x (stats)', '2', '#pos_y (stats)', '2', '#pos_z (stats)', '2', '#potential (stats)', '2', '#vel_x (stats)', '2', '#vel_y (stats)', '2', '#vel_z (stats)', '2', '#weight (stats)', '2', 'N (stats)', '2', 'Time (stats)', '2', 'vtkValidPointMask (stats)', '2']
plotDataOverTime1Display_1.SeriesMarkerStyle = ['#cell_id (stats)', '0', '#charge (stats)', '0', '#charge_number (stats)', '0', '#mass (stats)', '0', '#pos_x (stats)', '0', '#pos_y (stats)', '0', '#pos_z (stats)', '0', '#potential (stats)', '0', '#vel_x (stats)', '0', '#vel_y (stats)', '0', '#vel_z (stats)', '0', '#weight (stats)', '0', 'N (stats)', '0', 'Time (stats)', '0', 'vtkValidPointMask (stats)', '0']
plotDataOverTime1Display_1.SeriesMarkerSize = ['#cell_id (stats)', '4', '#charge (stats)', '4', '#mass (stats)', '4', '#pos_x (stats)', '4', '#pos_y (stats)', '4', '#pos_z (stats)', '4', '#potential (stats)', '4', '#vel_x (stats)', '4', '#vel_y (stats)', '4', '#vel_z (stats)', '4', '#weight (stats)', '4', 'N (stats)', '4', 'Time (stats)', '4', 'vtkValidPointMask (stats)', '4']
plotDataOverTime1Display_1.ShowQuartiles = 0
plotDataOverTime1Display_1.ShowRanges = 0

# ----------------------------------------------------------------
# setup the visualization in view 'quartileChartView3'
# ----------------------------------------------------------------

# show data from plotDataOverTime1
plotDataOverTime1Display_2 = Show(plotDataOverTime1, quartileChartView3, 'QuartileChartRepresentation')

# trace defaults for the display properties.
plotDataOverTime1Display_2.AttributeType = 'Row Data'
plotDataOverTime1Display_2.XArrayName = 'avg(#pos_z)'
plotDataOverTime1Display_2.SeriesVisibility = ['#vel_z (stats)']
plotDataOverTime1Display_2.SeriesLabel = ['#cell_id (stats)', '#cell_id (stats)', '#charge_number (stats)', '#charge_number (stats)', '#mass (stats)', '#mass (stats)', '#pos_x (stats)', '#pos_x (stats)', '#pos_y (stats)', '#pos_y (stats)', '#pos_z (stats)', '#pos_z (stats)', '#potential (stats)', '#potential (stats)', '#vel_x (stats)', '#vel_x (stats)', '#vel_y (stats)', '#vel_y (stats)', '#vel_z (stats)', '#vel_z (stats)', '#weight (stats)', '#weight (stats)', 'N (stats)', 'N (stats)', 'Time (stats)', 'Time (stats)', 'vtkValidPointMask (stats)', 'vtkValidPointMask (stats)', '#charge (stats)', '#charge (stats)']
plotDataOverTime1Display_2.SeriesColor = ['#cell_id (stats)', '0', '0', '0', '#charge_number (stats)', '0.889998', '0.100008', '0.110002', '#mass (stats)', '0.220005', '0.489998', '0.719997', '#pos_x (stats)', '0.300008', '0.689998', '0.289998', '#pos_y (stats)', '0.6', '0.310002', '0.639994', '#pos_z (stats)', '1', '0.500008', '0', '#potential (stats)', '0.650004', '0.340002', '0.160006', '#vel_x (stats)', '0', '0', '0', '#vel_y (stats)', '0.889998', '0.100008', '0.110002', '#vel_z (stats)', '0', '0', '0', '#weight (stats)', '0.300008', '0.689998', '0.289998', 'N (stats)', '0.6', '0.310002', '0.639994', 'Time (stats)', '1', '0.500008', '0', 'vtkValidPointMask (stats)', '0.650004', '0.340002', '0.160006', '#charge (stats)', '0', '0', '0']
plotDataOverTime1Display_2.SeriesPlotCorner = ['#cell_id (stats)', '0', '#charge (stats)', '0', '#charge_number (stats)', '0', '#mass (stats)', '0', '#pos_x (stats)', '0', '#pos_y (stats)', '0', '#pos_z (stats)', '0', '#potential (stats)', '0', '#vel_x (stats)', '0', '#vel_y (stats)', '0', '#vel_z (stats)', '0', '#weight (stats)', '0', 'N (stats)', '0', 'Time (stats)', '0', 'vtkValidPointMask (stats)', '0']
plotDataOverTime1Display_2.SeriesLabelPrefix = ''
plotDataOverTime1Display_2.SeriesLineStyle = ['#cell_id (stats)', '1', '#charge (stats)', '1', '#charge_number (stats)', '1', '#mass (stats)', '1', '#pos_x (stats)', '1', '#pos_y (stats)', '1', '#pos_z (stats)', '1', '#potential (stats)', '1', '#vel_x (stats)', '1', '#vel_y (stats)', '1', '#vel_z (stats)', '1', '#weight (stats)', '1', 'N (stats)', '1', 'Time (stats)', '1', 'vtkValidPointMask (stats)', '1']
plotDataOverTime1Display_2.SeriesLineThickness = ['#cell_id (stats)', '2', '#charge (stats)', '2', '#charge_number (stats)', '2', '#mass (stats)', '2', '#pos_x (stats)', '2', '#pos_y (stats)', '2', '#pos_z (stats)', '2', '#potential (stats)', '2', '#vel_x (stats)', '2', '#vel_y (stats)', '2', '#vel_z (stats)', '2', '#weight (stats)', '2', 'N (stats)', '2', 'Time (stats)', '2', 'vtkValidPointMask (stats)', '2']
plotDataOverTime1Display_2.SeriesMarkerStyle = ['#cell_id (stats)', '0', '#charge (stats)', '0', '#charge_number (stats)', '0', '#mass (stats)', '0', '#pos_x (stats)', '0', '#pos_y (stats)', '0', '#pos_z (stats)', '0', '#potential (stats)', '0', '#vel_x (stats)', '0', '#vel_y (stats)', '0', '#vel_z (stats)', '0', '#weight (stats)', '0', 'N (stats)', '0', 'Time (stats)', '0', 'vtkValidPointMask (stats)', '0']
plotDataOverTime1Display_2.SeriesMarkerSize = ['#cell_id (stats)', '4', '#charge (stats)', '4', '#mass (stats)', '4', '#pos_x (stats)', '4', '#pos_y (stats)', '4', '#pos_z (stats)', '4', '#potential (stats)', '4', '#vel_x (stats)', '4', '#vel_y (stats)', '4', '#vel_z (stats)', '4', '#weight (stats)', '4', 'N (stats)', '4', 'Time (stats)', '4', 'vtkValidPointMask (stats)', '4']
plotDataOverTime1Display_2.ShowQuartiles = 0
plotDataOverTime1Display_2.ShowRanges = 0

# ----------------------------------------------------------------
# setup the visualization in view 'quartileChartView4'
# ----------------------------------------------------------------

# show data from plotDataOverTime1
plotDataOverTime1Display_3 = Show(plotDataOverTime1, quartileChartView4, 'QuartileChartRepresentation')

# trace defaults for the display properties.
plotDataOverTime1Display_3.AttributeType = 'Row Data'
plotDataOverTime1Display_3.UseIndexForXAxis = 0
plotDataOverTime1Display_3.XArrayName = 'Time'
plotDataOverTime1Display_3.SeriesVisibility = ['#pos_x (stats)']
plotDataOverTime1Display_3.SeriesLabel = ['#cell_id (stats)', '#cell_id (stats)', '#charge (stats)', '#charge (stats)', '#mass (stats)', '#mass (stats)', '#pos_x (stats)', '#pos_x (stats)', '#pos_y (stats)', '#pos_y (stats)', '#pos_z (stats)', '#pos_z (stats)', '#potential (stats)', '#potential (stats)', '#vel_x (stats)', '#vel_x (stats)', '#vel_y (stats)', '#vel_y (stats)', '#vel_z (stats)', '#vel_z (stats)', '#weight (stats)', '#weight (stats)', 'N (stats)', 'N (stats)', 'Time (stats)', 'Time (stats)', 'vtkValidPointMask (stats)', 'vtkValidPointMask (stats)']
plotDataOverTime1Display_3.SeriesColor = ['#cell_id (stats)', '0', '0', '0', '#charge (stats)', '0.889998', '0.100008', '0.110002', '#mass (stats)', '0.220005', '0.489998', '0.719997', '#pos_x (stats)', '0', '0', '0', '#pos_y (stats)', '0.6', '0.310002', '0.639994', '#pos_z (stats)', '1', '0.500008', '0', '#potential (stats)', '0.650004', '0.340002', '0.160006', '#vel_x (stats)', '0', '0', '0', '#vel_y (stats)', '0.889998', '0.100008', '0.110002', '#vel_z (stats)', '0.220005', '0.489998', '0.719997', '#weight (stats)', '0.300008', '0.689998', '0.289998', 'N (stats)', '0.6', '0.310002', '0.639994', 'Time (stats)', '1', '0.500008', '0', 'vtkValidPointMask (stats)', '0.650004', '0.340002', '0.160006']
plotDataOverTime1Display_3.SeriesPlotCorner = ['#cell_id (stats)', '0', '#charge (stats)', '0', '#mass (stats)', '0', '#pos_x (stats)', '0', '#pos_y (stats)', '0', '#pos_z (stats)', '0', '#potential (stats)', '0', '#vel_x (stats)', '0', '#vel_y (stats)', '0', '#vel_z (stats)', '0', '#weight (stats)', '0', 'N (stats)', '0', 'Time (stats)', '0', 'vtkValidPointMask (stats)', '0']
plotDataOverTime1Display_3.SeriesLabelPrefix = ''
plotDataOverTime1Display_3.SeriesLineStyle = ['#cell_id (stats)', '1', '#charge (stats)', '1', '#mass (stats)', '1', '#pos_x (stats)', '1', '#pos_y (stats)', '1', '#pos_z (stats)', '1', '#potential (stats)', '1', '#vel_x (stats)', '1', '#vel_y (stats)', '1', '#vel_z (stats)', '1', '#weight (stats)', '1', 'N (stats)', '1', 'Time (stats)', '1', 'vtkValidPointMask (stats)', '1']
plotDataOverTime1Display_3.SeriesLineThickness = ['#cell_id (stats)', '2', '#charge (stats)', '2', '#mass (stats)', '2', '#pos_x (stats)', '2', '#pos_y (stats)', '2', '#pos_z (stats)', '2', '#potential (stats)', '2', '#vel_x (stats)', '2', '#vel_y (stats)', '2', '#vel_z (stats)', '2', '#weight (stats)', '2', 'N (stats)', '2', 'Time (stats)', '2', 'vtkValidPointMask (stats)', '2']
plotDataOverTime1Display_3.SeriesMarkerStyle = ['#cell_id (stats)', '0', '#charge (stats)', '0', '#mass (stats)', '0', '#pos_x (stats)', '0', '#pos_y (stats)', '0', '#pos_z (stats)', '0', '#potential (stats)', '0', '#vel_x (stats)', '0', '#vel_y (stats)', '0', '#vel_z (stats)', '0', '#weight (stats)', '0', 'N (stats)', '0', 'Time (stats)', '0', 'vtkValidPointMask (stats)', '0']
plotDataOverTime1Display_3.SeriesMarkerSize = ['#cell_id (stats)', '4', '#charge (stats)', '4', '#mass (stats)', '4', '#pos_x (stats)', '4', '#pos_y (stats)', '4', '#pos_z (stats)', '4', '#potential (stats)', '4', '#vel_x (stats)', '4', '#vel_y (stats)', '4', '#vel_z (stats)', '4', '#weight (stats)', '4', 'N (stats)', '4', 'Time (stats)', '4', 'vtkValidPointMask (stats)', '4']
plotDataOverTime1Display_3.ShowQuartiles = 0
plotDataOverTime1Display_3.ShowRanges = 0

# ----------------------------------------------------------------
# setup the visualization in view 'quartileChartView5'
# ----------------------------------------------------------------

# show data from plotDataOverTime2
plotDataOverTime2Display = Show(plotDataOverTime2, quartileChartView5, 'QuartileChartRepresentation')

# trace defaults for the display properties.
plotDataOverTime2Display.AttributeType = 'Row Data'
plotDataOverTime2Display.UseIndexForXAxis = 0
plotDataOverTime2Display.XArrayName = 'Time'
plotDataOverTime2Display.SeriesVisibility = ['#pos_x (stats)']
plotDataOverTime2Display.SeriesLabel = ['#cell_id (stats)', '#cell_id (stats)', '#charge (stats)', '#charge (stats)', '#mass (stats)', '#mass (stats)', '#pos_x (stats)', '#pos_x (stats)', '#pos_y (stats)', '#pos_y (stats)', '#pos_z (stats)', '#pos_z (stats)', '#potential (stats)', '#potential (stats)', '#vel_x (stats)', '#vel_x (stats)', '#vel_y (stats)', '#vel_y (stats)', '#vel_z (stats)', '#vel_z (stats)', '#weight (stats)', '#weight (stats)', 'N (stats)', 'N (stats)', 'Time (stats)', 'Time (stats)', 'vtkValidPointMask (stats)', 'vtkValidPointMask (stats)']
plotDataOverTime2Display.SeriesColor = ['#cell_id (stats)', '0', '0', '0', '#charge (stats)', '0.889998', '0.100008', '0.110002', '#mass (stats)', '0.220005', '0.489998', '0.719997', '#pos_x (stats)', '0.300008', '0.689998', '0.289998', '#pos_y (stats)', '0.6', '0.310002', '0.639994', '#pos_z (stats)', '1', '0.500008', '0', '#potential (stats)', '0.650004', '0.340002', '0.160006', '#vel_x (stats)', '0', '0', '0', '#vel_y (stats)', '0.889998', '0.100008', '0.110002', '#vel_z (stats)', '0.220005', '0.489998', '0.719997', '#weight (stats)', '0.300008', '0.689998', '0.289998', 'N (stats)', '0.6', '0.310002', '0.639994', 'Time (stats)', '1', '0.500008', '0', 'vtkValidPointMask (stats)', '0.650004', '0.340002', '0.160006']
plotDataOverTime2Display.SeriesPlotCorner = ['#cell_id (stats)', '0', '#charge (stats)', '0', '#mass (stats)', '0', '#pos_x (stats)', '0', '#pos_y (stats)', '0', '#pos_z (stats)', '0', '#potential (stats)', '0', '#vel_x (stats)', '0', '#vel_y (stats)', '0', '#vel_z (stats)', '0', '#weight (stats)', '0', 'N (stats)', '0', 'Time (stats)', '0', 'vtkValidPointMask (stats)', '0']
plotDataOverTime2Display.SeriesLabelPrefix = ''
plotDataOverTime2Display.SeriesLineStyle = ['#cell_id (stats)', '1', '#charge (stats)', '1', '#mass (stats)', '1', '#pos_x (stats)', '1', '#pos_y (stats)', '1', '#pos_z (stats)', '1', '#potential (stats)', '1', '#vel_x (stats)', '1', '#vel_y (stats)', '1', '#vel_z (stats)', '1', '#weight (stats)', '1', 'N (stats)', '1', 'Time (stats)', '1', 'vtkValidPointMask (stats)', '1']
plotDataOverTime2Display.SeriesLineThickness = ['#cell_id (stats)', '2', '#charge (stats)', '2', '#mass (stats)', '2', '#pos_x (stats)', '2', '#pos_y (stats)', '2', '#pos_z (stats)', '2', '#potential (stats)', '2', '#vel_x (stats)', '2', '#vel_y (stats)', '2', '#vel_z (stats)', '2', '#weight (stats)', '2', 'N (stats)', '2', 'Time (stats)', '2', 'vtkValidPointMask (stats)', '2']
plotDataOverTime2Display.SeriesMarkerStyle = ['#cell_id (stats)', '0', '#charge (stats)', '0', '#mass (stats)', '0', '#pos_x (stats)', '0', '#pos_y (stats)', '0', '#pos_z (stats)', '0', '#potential (stats)', '0', '#vel_x (stats)', '0', '#vel_y (stats)', '0', '#vel_z (stats)', '0', '#weight (stats)', '0', 'N (stats)', '0', 'Time (stats)', '0', 'vtkValidPointMask (stats)', '0']
plotDataOverTime2Display.SeriesMarkerSize = ['#cell_id (stats)', '4', '#charge (stats)', '4', '#mass (stats)', '4', '#pos_x (stats)', '4', '#pos_y (stats)', '4', '#pos_z (stats)', '4', '#potential (stats)', '4', '#vel_x (stats)', '4', '#vel_y (stats)', '4', '#vel_z (stats)', '4', '#weight (stats)', '4', 'N (stats)', '4', 'Time (stats)', '4', 'vtkValidPointMask (stats)', '4']
plotDataOverTime2Display.ShowQuartiles = 0
plotDataOverTime2Display.ShowRanges = 0

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
# setup the visualization in view 'renderView2'
# ----------------------------------------------------------------

# show data from tableToPoints1
tableToPoints1Display = Show(tableToPoints1, renderView2, 'GeometryRepresentation')

# trace defaults for the display properties.
tableToPoints1Display.Representation = 'Points'
tableToPoints1Display.AmbientColor = [0.0, 0.0, 0.0]
tableToPoints1Display.ColorArrayName = [None, '']
tableToPoints1Display.DiffuseColor = [0.0, 0.0, 0.0]
tableToPoints1Display.OSPRayScaleArray = '#cell_id'
tableToPoints1Display.OSPRayScaleFunction = 'PiecewiseFunction'
tableToPoints1Display.SelectOrientationVectors = '#cell_id'
tableToPoints1Display.ScaleFactor = 0.09999290000000001
tableToPoints1Display.SelectScaleArray = '#cell_id'
tableToPoints1Display.GlyphType = 'Arrow'
tableToPoints1Display.GlyphTableIndexArray = '#cell_id'
tableToPoints1Display.GaussianRadius = 0.0049996450000000005
tableToPoints1Display.SetScaleArray = ['POINTS', '#cell_id']
tableToPoints1Display.ScaleTransferFunction = 'PiecewiseFunction'
tableToPoints1Display.OpacityArray = ['POINTS', '#cell_id']
tableToPoints1Display.OpacityTransferFunction = 'PiecewiseFunction'
tableToPoints1Display.DataAxesGrid = 'GridAxesRepresentation'
tableToPoints1Display.PolarAxes = 'PolarAxesRepresentation'

# init the 'PiecewiseFunction' selected for 'ScaleTransferFunction'
tableToPoints1Display.ScaleTransferFunction.Points = [1.0, 0.0, 0.5, 0.0, 512.0, 1.0, 0.5, 0.0]

# init the 'PiecewiseFunction' selected for 'OpacityTransferFunction'
tableToPoints1Display.OpacityTransferFunction.Points = [1.0, 0.0, 0.5, 0.0, 512.0, 1.0, 0.5, 0.0]

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
animationScene.GoToFirst()
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


