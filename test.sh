#!/bin/bash
#SBATCH -N1 --ntasks-per-node=10
#SBATCH -t 00:05:00
#SBATCH -p normal_q
#SBATCH -A cmda3634alloc

#Record the node we ran on
echo "Job ran on: $SLURM_NODELIST"

#Load modules
module purge; module load gcc; module load openmpi

#Build code
mpicc -o test test.c serialVector.c parallelVector.c -lm

#Run code with a few different numbers of processors
echo "$( date ): Start loop"
for np in 1 2 4 6 8 10; do
	echo "-----------------------------------"
	echo "Number of processes: $np "
	mpirun -np $np ./test
	echo  "----------------------------------"
done
echo  "$( date ): End loop"
