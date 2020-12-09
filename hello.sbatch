#!/bin/bash
#SBATCH -J hello
#SBATCH -p v100_normal_q
#SBATCH -N 1  # this requests 1 node, 1 core.
#SBATCH -t 01:00 # 1 minute
#SBATCH --gres=gpu:1 # request 1 gpu
#SBATCH -A cmda3634alloc

module purge
module load cuda
nvcc hello.cu -o hello
./hello