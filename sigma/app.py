from stats import *
from parser import parse
from time import time
from random import randint
from subprocess import Popen

def exp_write(vars, params):
    """
    Takes in the variables to be written from the client forms
    and the params from the settings file which determines the
    order of the parameters
    Returns the unique output filename (current epoch time)
    """

    fname = int(time())
    expfile = open(fname+'.exp', 'w')

    outfile = fname+'.out'
    randomseed = random.randint(0, 65534)
    exparr = [outfile, randomseed]
    exparr.extend(params)
    expstring = ' '.join(outarr)
    expfile.write(expstring)
    expfile.flush()
    expfile.close()

    return outfile

def run_simulation(POST, graphs):
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
