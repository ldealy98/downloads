import random

NJobs = 4
for i in range(NJobs):
	# generate my job submission script file name
	filename = "job"+str(i)+".sh"

	# open that file to write
	f = open(filename,"w")

	# print basic info about job
	f.write("#!/bin/bash\n")
	f.write("#SBATCH -t 0:01:00\n")
	f.write("#SBATCH -N 1 --ntasks-per-node=1\n")
	f.write("#SBATCH -A cmda3634alloc\n")
	f.write("#SBATCH -p dev_q\n")
	f.write("module reset\n")
	f.write("module load Anaconda\n")

	# build the code
	f.write("make buildSqr\n")	

	# randomly choose number of entries
	N = random.randint(40,4000)
	
	# print line that actually runs the code
	f.write("./sqr "+str(N)+"\n")
	# print line for visualization
	f.write("python plot.py\n")

	# close the file
	f.close()
