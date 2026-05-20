import gurobipy as gp
from gurobipy import GRB
import pandas
import os
from dotenv import load_dotenv

def write_out_file(model):
    filename = "../output.txt"
    with open(filename, "w") as f:
        f.write(f"Maximum Flow {int(model.ObjVal)}\n")
        for (u, v), var in flow.items():
            if var.X > 0: 
                f.write(f'{int(var.UB)}/{int(var.X)}\n')
    
    
load_dotenv()

wls_access_id = os.getenv("WLSACCESSID")
wls_secret = os.getenv("WLSSECRET")
license_id = os.getenv("LICENSEID")
if wls_access_id and wls_secret and license_id:
    options = {
        "WLSACCESSID": wls_access_id,
        "WLSSECRET": wls_secret,
        "LICENSEID": int(license_id)
    }
    env = gp.Env(params=options)
else:
    env=gp.Env()

arcs, capacities = gp.multidict({
    ("s", "v1") : 18,
    ("s", "v2") : 15,
    ("v2","v1") : 5, 
    ("v1","v3") : 14,
    ("v2","v4") : 16,
    ("v3","v2") : 7,
    ("v4","v3") : 9,
    ("v3", "t") : 22,
    ("v4", "t") : 6 
})

model = gp.Model("Max Flow Linear Programming Solution", env=env)

# this implicit gives the non negative constraint
flow = model.addVars(arcs, ub=capacities, name="flow")

nodes = set(u for u, v in arcs).union(set(v for u, v in arcs))
middle_nodes = [n for n in nodes if n not in ['s', 't']]

model.addConstrs(
    (flow.sum('*', node) - flow.sum(node, '*') == 0 for node in middle_nodes), name='Conservation'
)

model.setObjective(flow.sum('s', '*'), GRB.MAXIMIZE)

model.optimize()

write_out_file(model)

