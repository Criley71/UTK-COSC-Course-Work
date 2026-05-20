# Setup
1. Create and start up python virtual environment in src directory
```
cd src
python -m venv .venv
source .venv/bin/activate
```
2. Install required modules
```
pip install -r requirements.txt
```

# Usage
1. To run the script
``` 
python max_flow.py
```

# Output
All output is written to `output.txt` in the outer directory  

# Notes
If a `.env` file is present in the src directory the gurobi api key info is gathered from it due to my system set up using WSL on windows and not having a method of installing the license for linux on my windows machine