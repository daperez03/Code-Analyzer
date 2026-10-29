# !/bin/bash

# Add modules
module add cuda11.4
module add miniconda3

# Create a new conda environment
conda create --name python3.14.4 python=3.14.4
conda init bash
conda activate python3.14.4

# Install lfs
conda install -c conda-forge git-lfs
git lfs version
git lfs install
git submodule foreach --recursive 'git lfs pull'

# Install dependencies
pip install -r requirements.txt
python -c "from tree_sitter_language_pack import download; print(download(['python', 'php', 'c', 'cpp', 'java']))"
