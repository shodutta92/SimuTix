from stats import *
from parser import *
from time import time
from random import randint

def exp_write(vars, params):
    """
    Takes in the variables to be written as a list with their values
    Returns the unique filename (current epoch time)
    """
    fname = int(time())
    randomseed = random.randint(0, 65534)

    expfile = open(fname+'.exp', 'w')

    outfile = fname+'.out'


    return 0

def run_simulation(POST, graphs):
    """
    Takes in the POST variables from the simulate Page
    and returns the dictionary of variables neccesary for graphs

    Will throw a key error if defined graph variable in settings.json
    does not exist in the sigma trace output
    """
    return 0
