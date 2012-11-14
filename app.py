from util.stats import *
from util.parser import parse
from util.decorators import run_in_env
from random import randint
from time import time
from subprocess import Popen, PIPE, STDOUT
import os, sys
from glob import glob

PROJECT_PATH = os.getcwd()

def clean_tmp():
    """
    Removes any files leftover in the tmp directory. 
    Also creates the tmp directory if it does not exist.
    """
    if not os.path.exists("tmp"):
        os.mkdir("tmp")
    else:
        for f in glob('tmp/*.exp'):
            os.unlink(f)
        for f in glob('tmp/*.out'):
            os.unlink(f)
        
@run_in_env("tmp")  
def run_simulation(forms, sigma_settings):
    """
    Takes in the variables to be written from the client forms
    and the params from the settings file which determines the
    order of the parameters. 
    Returns a parsed SigmaOutput instance of output file.
    """

    f_name = int(time())
    outfile_name = "%s.out" % f_name
    expfile_name = "%s.exp" % f_name
    randomseed = randint(0, 65534)
    exp = [outfile_name, "n", str(randomseed), forms['RUNTIME'], "1"]
    
    for param in sigma_settings['parameters']:
        exp.append(forms[param['name']])
        
    expstr = " ".join(exp)

    expfile = open(expfile_name, 'w')
    expfile.write(expstr)
    expfile.flush()
    expfile.close()
    
    p = Popen(["%s\\sigma\\bin\\%s.exe" % (PROJECT_PATH, sigma_settings['model']), 
               expfile_name], stdout=PIPE, stderr=STDOUT)
    p.wait()
    
    out_inst = parse(outfile_name)
    os.remove(expfile_name)
    os.remove(outfile_name)
    
    return out_inst
    
def prepare_graphs(parsed_data, graph_settings):
    """
    Takes in parsed SigmaOutput type data from run_simulation,
    returns a dictionary ready to be sent to the client.
    """

    graphData = []
    for graph in graph_settings:
        lines = []
        for line in range(len(graph['lines'])):
            cur_graph = {'key': graph['lines'][line], 'values': []}
            for i in range(len(parsed_data.getColumn(graph['x-axis']))):
                cur_graph['values'].append({"x": parsed_data.getColumn(graph['x-axis'])[i],
                                            "y": parsed_data.getColumn(graph['y-axis'][line])[i]})
            lines.append(cur_graph)
        graphData.append(lines)
    out = {"data": graphData, "graphs": graph_settings}
    return out