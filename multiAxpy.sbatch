#!/bin/bash
#SBATCH -J multi
#SBATCH -p v100_dev_q
#SBATCH -N 1
#SBATCH -t 04:00
#SBATCH --gres=gpu:1
#SBATCH -A cmda3634alloc

module purge
module load cuda
nvcc multiAxpy.cu -o multiAxpy
./multiAxpy 32
./multiAxpy 400
./multiAxpy 2000
./multiAxpy 10000
./multiAxpy 50000
./multiAxpy 250000
