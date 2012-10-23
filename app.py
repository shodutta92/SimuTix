from util.stats import *
from util.parser import parse
from random import randint
from subprocess import Popen, PIPE, STDOUT
import os
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
    outfile = tempfile.NamedTemporaryFile()
    randomseed = randint(0, 65534)
    exp = [outfile.name, "no", str(randomseed), "1000", "1", "3", "5"] 
    expstr = "\n".join(exp) + "n"
    print expstr
    print os.getcwd()+settings.sigma['model']
    
    p = Popen(os.getcwd()+settings.sigma['model'], stdin=PIPE, universal_newlines=True)
    p.communicate(input=expstr)
    
    print outfile.read()
    
test()