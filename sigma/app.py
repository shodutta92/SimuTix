from stats import *
from parser import parse
from time import time
from random import randint
from subprocess import Popen, PIPE, STDOUT

def run_simulation(vars, params):
    """
    Takes in the variables to be written from the client forms
    and the params from the settings file which determines the
    order of the parameters. 
    Returns the parsed dictionary of output.
    """

    fname = int(time())

    outfile = fname+'.out'
    randomseed = random.randint(0, 65534)
    expstr = outfile + "\n" + randomseed

    p = Popen(settings['sigma']['model'], stdout=PIPE, stderr=STDOUT, stdin=PIPE)
    p.communicate(input=expstr)

    return outdict

def build_graphs(POST, graphs):
    """
    Takes in the POST variables from the simulate Page
    and returns the dictionary of variables neccesary for graphs

    Will throw a key error if defined graph variable in settings.json
    does not exist in the sigma trace output
    """

    outfile = exp_write(POST, params)
    p = Popen(model, shell=True)
    while p.poll() is None:

    data = parse(outfile)
        
    return data, graph
