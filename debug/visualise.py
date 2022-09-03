import matplotlib.pyplot as plt
import matplotlib.animation as animation
import numpy as np

NUM_PARTICLES = 500
FRAME_RATE = 60
SECONDS = 15

# Read file
data_file_dir='./out_visualise.txt'
fileIO=open(data_file_dir,"r")
fileArray=np.loadtxt(fileIO,unpack=False,skiprows=0) 
fileIO.close()

# Format array
fileArray = fileArray.reshape((-1, NUM_PARTICLES, 2))

# Animate
numframes = FRAME_RATE*SECONDS
color_data = '#0362fc'

fig = plt.figure()
scat = plt.scatter(fileArray[0,:, 0], fileArray[0,:, 1], c=color_data, s=20)
plt.xlim(0, 100)
plt.ylim(0, 100)



def update_plot(i, data, scat):
    scat.set_offsets(data[i])
    return scat

ani = animation.FuncAnimation(fig, update_plot, frames=range(numframes),
                                fargs=(fileArray, scat))
#plt.show()
ani.save('out.mp4', fps=FRAME_RATE, dpi=200)