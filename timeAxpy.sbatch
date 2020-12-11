#!/bin/bash
#SBATCH -J time
#SBATCH -p v100_dev_q
#SBATCH -N 1
#SBATCH -t 04:00
#SBATCH --gres=gpu:1
#SBATCH -A cmda3634alloc

module purge
module load cuda
nvcc timeAxpy.cu -o timeAxpy
./timeAxpy 400
./timeAxpy 2000
./timeAxpy 10000
./timeAxpy 50000
./timeAxpy 250000
