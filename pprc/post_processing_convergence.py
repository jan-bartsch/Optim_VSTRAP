from matplotlib import pyplot as plt
import tikzplotlib

fig_objective = plt.figure()

#objective
objective_track_file = open("../../build/src/objectiveTrack.txt")

objective_track = objective_track_file.read().split("\n")
objective_track_float = [];


for i in range(0,len(objective_track)-1):
	print(i)
	objective_track_float.insert(len(objective_track_float),float(objective_track[i]))


ax1 = plt.subplot(221)
plt.plot(objective_track_float)
plt.title("History objective track")
ax1.set_yscale('linear')


#gradient
norm_gradient_file = open("../../build/src/normGradientTrack.txt")

norm_gradient = norm_gradient_file.read().split("\n")
norm_gradient_float = []

for i in range(0,len(norm_gradient)-1):
	norm_gradient_float.insert(len(norm_gradient_float),float(norm_gradient[i])/float(norm_gradient[0]))

#print(norm_gradient)

ax2 = plt.subplot(222)
plt.plot(norm_gradient_float)
plt.title("Norm relative gradient")
#ax2.set_yscale('linear')

#control
norm_control_file = open("../../build/src/normControlTrack.txt")

norm_control = norm_control_file.read().split("\n");
norm_control_float = []

for i in range(0,len(norm_control)-1):
	norm_control_float.insert(len(norm_control_float),float(norm_control[i]))
	
#print(norm_control)

ax3 = plt.subplot(223)
plt.plot(norm_control_float)
plt.title("Norm control")
#ax3.set_yscale('linear')

tikzplotlib.save("fig/post_processing_converging.tex")

plt.show()


