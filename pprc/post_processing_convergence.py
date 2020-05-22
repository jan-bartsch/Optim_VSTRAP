from matplotlib import pyplot as plt
import tikzplotlib

fig_objective = plt.figure()

pathToBuildSRC = "../../build-Optim/src/"

####
# objective
####
objective_track_file = open(pathToBuildSRC + "objectiveTrack.txt")

objective_track = objective_track_file.read().split("\n")
objective_track_float = [];


for i in range(0,len(objective_track)-1):
	objective_track_float.insert(len(objective_track_float),float(objective_track[i])/abs(float(objective_track[0])))


ax1 = plt.subplot(221)
plt.plot(objective_track_float)
plt.title("History of convergence of relative value of objective")
ax1.set_yscale('linear')


####
# gradient
####
norm_gradient_file = open(pathToBuildSRC + "normGradientTrack.txt")

norm_gradient = norm_gradient_file.read().split("\n")
norm_gradient_float = []

#print(norm_gradient)

for i in range(0,len(norm_gradient)-1):
	norm_gradient_float.insert(len(norm_gradient_float),float(norm_gradient[i])/float(norm_gradient[0]))

ax2 = plt.subplot(222)
plt.plot(norm_gradient_float)
plt.title("Norm relative gradient")
ax2.set_yscale('linear')

####
# control
####
norm_control_file = open(pathToBuildSRC + "normControlTrack.txt")

norm_control = norm_control_file.read().split("\n")
norm_control_float = []

for i in range(0,len(norm_control)-1):
	norm_control_float.insert(len(norm_control_float),float(norm_control[i]))
	
ax3 = plt.subplot(223)
plt.plot(norm_control_float)
plt.title("Norm control")
ax3.set_yscale('linear')



####
# wasserstein distance
####
try:
	wd_file = open(pathToBuildSRC + "wassersteinDistanceTrack.txt")


	wd = wd_file.read().split("\n")
	wd_float = []

	for i in range(0,len(wd)-1):
		wd_float.insert(len(wd_float),float(wd[i]))

	ax4 = plt.subplot(224)
	plt.plot(wd_float)
	plt.title("Wasserstein distance (pre/post linesearch)")
	ax4.set_yscale('linear')
except:
	print("No wasserstein Distance file existing")

tikzplotlib.save("fig/post_processing_converging.tex")
plt.show()

