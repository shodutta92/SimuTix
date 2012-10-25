from util.stats import *
from util.parser import parse
from random import randint
from time import time
from subprocess import Popen, PIPE, STDOUT
import os
import sys
import settings

def run_simulation(vars, params):
    """
    Takes in the variables to be written from the client forms
    and the params from the settings file which determines the
    order of the parameters. 
    Returns a parsed SigmaOutput instance of output file.
    """
    randomseed = random.randint(0, 65534)
    expstr = outfile + "\n" + randomseed

    p = Popen(settings['sigma']['model'], stdout=PIPE, stderr=STDOUT, stdin=PIPE)
    p.communicate(input=expstr)

    return outdict

def build_graphs(POST, graphs):

    return 0

    
def test():
    model = settings.sigma['model']

    f_name = int(time())
    outfile_name = "%s.out" % f_name
    expfile_name = "%s.exp" % f_name
    randomseed = randint(0, 65534)
    exp = [outfile_name, "n", str(randomseed), "1000", "1", "3", "5"] 
    expstr = " ".join(exp)
    
    expfile = open(expfile_name, 'w')
    expfile.write(expstr)
    expfile.flush()
    expfile.close()
    
    p = Popen(["model\\bin\\%s.exe" % model, 
               expfile_name], stdout=PIPE, stderr=STDOUT)
    p.wait()
    
    out_inst = parse(outfile_name)
    os.remove(expfile_name)
    os.remove(outfile_name)
    
    print out_inst
test()
