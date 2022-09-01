from tkinter import Frame
import matplotlib.pyplot as plt
import matplotlib.animation as animation
import numpy as np

NUM_PARTICLES = 100
FRAME_RATE = 30

# Read file
data_file_dir='./out.txt'
fileIO=open(data_file_dir,"r")
fileArray=np.loadtxt(fileIO,unpack=False,skiprows=0) 
fileIO.close()

# Format array
fileArray = fileArray.reshape((-1, NUM_PARTICLES, 2))

# Animate


numframes = FRAME_RATE*5
color_data = '#0362fc'

fig = plt.figure()
scat = plt.scatter(fileArray[0,:, 0], fileArray[0,:, 1], c=color_data, s=10)


def update_plot(i, data, scat):
    scat.set_offsets(data[i])
    return scat

ani = animation.FuncAnimation(fig, update_plot, frames=range(numframes),
                                fargs=(fileArray, scat))
#plt.show()
ani.save('out.mp4', fps=FRAME_RATE, dpi=200)