import matplotlib.pyplot as plt
import matplotlib.animation as animation
import numpy as np


# Read file
data_file_dir='./out_cg.txt'
fileIO=open(data_file_dir,"r")
fileArray=np.loadtxt(fileIO,unpack=False,skiprows=0) 
fileIO.close()

plt.scatter(fileArray[:, 0], fileArray[:, 1], s=5)
plt.xlim(0,100)
plt.ylim(0,100)

plt.savefig("out_cg.png")
