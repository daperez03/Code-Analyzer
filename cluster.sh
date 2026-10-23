#!/bin/bash 
# 
#SBATCH --job-name=code-analyzer 
#SBATCH --output=code-analyzer.out 
# 
#SBATCH --nodes=1
#SBATCH --mem=45gb
#SBATCH --partition=gpu-wide 
#SBATCH --gres=gpu:1 
module load miniconda3 cuda11.4 
source ~/.bashrc 
conda activate python3.9 
python -m pip install -r Code-Analyzer/requirements.txt
srun python Code-Analyzer/src/main.py