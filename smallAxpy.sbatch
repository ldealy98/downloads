#!/bin/bash
#SBATCH -J small
#SBATCH -p v100_dev_q
#SBATCH -N 1
#SBATCH -t 01:00
#SBATCH --gres=gpu:1
#SBATCH -A cmda3634alloc

module purge
module load cuda
nvcc smallAxpy.cu -o smallAxpy
./smallAxpy 32
