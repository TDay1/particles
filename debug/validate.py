import numpy as np
import sys

if (len(sys.argv) < 3):
    print("Validate.py: compares two out_cg.txt files to validate the parallel implementation.")
    print("Usage: python3 debug/validate.py [out_cg1.txt] [out_cg2.txt]")

# Read file
data_file_dir=sys.argv[1]
fileIO=open(data_file_dir,"r")
fileArray=np.loadtxt(fileIO,unpack=False,skiprows=0) 
fileIO.close()

data_file_dir2=sys.argv[2]
fileIO2=open(data_file_dir2,"r")
fileArray2=np.loadtxt(fileIO2,unpack=False,skiprows=0) 
fileIO2.close()

running_sum = 0
for i in range(150):
    delta_x = fileArray[i, 0] - fileArray2[i, 0]
    delta_y = fileArray[i, 1] - fileArray2[i, 1]
    running_sum += np.sqrt( delta_x**2 + delta_y**2)

print(f"Mean distance between two CG's over the simulation: {running_sum/150}")
