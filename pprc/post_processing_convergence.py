from matplotlib import pyplot as plt

fig_objective = plt.figure()

#objective
objective_track_file = open("../../build/src/objectiveTrack.txt")

objective_track = objective_track_file.read().split("\n")

for i in range(0,len(objective_track)-1):
	objective_track[i] = float(objective_track[i])

#print(objective_track)

plt.subplot(221)
plt.plot(objective_track)
plt.title("History objective track")



#gradient
norm_gradient_file = open("../../build/src/normGradientTrack.txt")

norm_gradient = norm_gradient_file.read().split("\n")

for i in range(0,len(norm_gradient)-1):
	norm_gradient[i] = float(norm_gradient[i])/float(norm_gradient[0])

#print(norm_gradient)

plt.subplot(222)
plt.plot(norm_gradient)
plt.title("Norm relative gradient")

#control
norm_control_file = open("../../build/src/normControlTrack.txt")

norm_control = norm_control_file.read().split("\n");

for i in range(0,len(norm_control)-1):
	norm_control[i] = float(norm_control[i])

#print(norm_control)

plt.subplot(223)
plt.plot(norm_control)
plt.title("Norm control")

plt.show()

