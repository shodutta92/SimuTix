from util.stats import *
from util.parser import parse
from util.decorators import run_in_env
from random import randint
from time import time
from subprocess import Popen, PIPE, STDOUT
import os, sys
from glob import glob
import math

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
    
def prepare_graphs(parsed_data, graph_settings, rdp_enable):
    """
    Takes in parsed SigmaOutput type data from run_simulation,
    returns a dictionary ready to be sent to the client.
    """

    graphData = []
    for graph in graph_settings:
        lines = []
        prev_point = None
        for line in range(len(graph['lines'])):
            cur_graph = {'key': graph['lines'][line], 'values': []}
            for i in range(len(parsed_data.getColumn(graph['x-axis']))):
                if prev_point == None:
                    prev_point = {"x": parsed_data.getColumn(graph['x-axis'])[i], "y": parsed_data.getColumn(graph['y-axis'][line])[i]}
                elif prev_point["x"] == parsed_data.getColumn(graph['x-axis'])[i]:
                    prev_point["y"] = parsed_data.getColumn(graph['y-axis'][line])[i]
                else:
                    cur_graph['values'].append({"x": parsed_data.getColumn(graph['x-axis'])[i],
                                                "y": parsed_data.getColumn(graph['y-axis'][line])[i]})
                    prev_point = cur_graph['values'][-1]
                    
            if rdp_enable:
                cur_graph['values'] = rdp_reduce(cur_graph['values'], graph['rdp_epsilon'][line])

            lines.append(cur_graph)
        graphData.append(lines)
    
    out = {"data": graphData, "graphs": graph_settings}
    return out
    

    
def perpendicular_distance(point, line):
    """ 
    Takes in a point and a line, and returns the minimum distance between them
    point is of form: {"x": 1, "y": 2}, line is a tuple of 2 points.
    """
    slope = float(line[1]["y"] - line[0]["y"]) / (line[1]["x"] - line[0]["x"])
    intercept = line[0]["y"] - slope*line[0]["x"]
    distance = float(abs(slope*point["x"] - point["y"] + intercept)) / math.sqrt(math.pow(slope, 2) + 1)
    return distance
    
def rdp_reduce(point_list, epsilon):
    """
    Takes in a list of points [{"x": 1, "y": 2}, {"x": 3, "y": 4}, ...]
    and returns a list of reduced points, according to the given epsilon
    """

    dmax = 0
    index = 0
    for i in range(2, len(point_list)):
        d = perpendicular_distance(point_list[i], (point_list[1], point_list[-1]))
        if d > dmax:
            index = i
            dmax = d
            
    if dmax >= epsilon:
        rec_results1 = rdp_reduce(point_list[1:index], epsilon)
        rec_results2 = rdp_reduce(point_list[index:], epsilon)

        result_list = rec_results1[1:-1] + rec_results2[1:]
    
    else:
        result_list = point_list

    return result_list
    
    

