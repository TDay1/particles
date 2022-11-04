import matplotlib.pyplot as plt
import matplotlib.animation as animation
import numpy as np
import sys

# Read file
data_file_dir=sys.argv[1]
fileIO=open(data_file_dir,"r")
fileArray=np.loadtxt(fileIO,unpack=False,skiprows=0) 
fileIO.close()

data_file_dir2=sys.argv[2]
fileIO2=open(data_file_dir2,"r")
fileArray2=np.loadtxt(fileIO2,unpack=False,skiprows=0) 
fileIO2.close()

for i in range(150):
    print((fileArray[i, 0] - fileArray2[i, 0]) + (fileArray[i, 1] - fileArray2[i, 1]))

