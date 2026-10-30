#!/bin/bash 
# 
#SBATCH --mail-user=daniel.perezmorera@ucr.ac.cr
#SBATCH --mail-type=BEGIN,END,FAIL
#SBATCH --job-name=code-analyzer
#SBATCH --output=code-analyzer.out
#
#SBATCH --nodes=1
#SBATCH --mem=45gb
#SBATCH --partition=gpu-extended
#SBATCH --gres=gpu:2

module load miniconda3 cuda11.4
conda activate python3.14.4
srun python src/main.py