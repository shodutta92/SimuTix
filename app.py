from util.stats import *
from util.parser import parse
from random import randint
from subprocess import Popen, PIPE, STDOUT
import os
import sys
import settings
import tempfile

def run_simulation(vars, params):
    """
    Takes in the variables to be written from the client forms
    and the params from the settings file which determines the
    order of the parameters. 
    Returns the parsed dictionary of output.
    """
    randomseed = random.randint(0, 65534)
    expstr = outfile + "\n" + randomseed

    p = Popen(settings['sigma']['model'], stdout=PIPE, stderr=STDOUT, stdin=PIPE)
    p.communicate(input=expstr)

    return outdict

def build_graphs(POST, graphs):

    return 0

    
def test():
    outfile = "test.out"#tempfile.NamedTemporaryFile()
    curr_path = os.getcwd()
    randomseed = randint(0, 65534)
    exp = [outfile, "yes", str(randomseed), "1000", "1", "3", "5"] 
    #expstr = " \r\n ".join(exp) + " \r"
    #expstr = "test.out\n no\"
    
    p = Popen(curr_path+settings.sigma['model'], stdin=PIPE)# stdout=PIPE, stderr=STDOUT)
    p.stdin.write("test.out \n")
    p.stdin.flush()
    p.stdin.write("no \n")
    p.stdin.flush()
    p.stdin.write("1234 \n")
    p.stdin.flush()
    p.stdin.write("1000 \n")

    #output = p.communicate(input=expstr)[0]
    
    #outfile.write("hello")
    #outfile.flush()
    #print output
    
test()
