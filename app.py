from util.stats import *
from util.parser import parse
from util.decorators import run_in_env
from random import randint
from time import time
from subprocess import Popen, PIPE, STDOUT
import os, sys, shutil, settings

PROJECT_PATH = os.getcwd()

def clean_tmp():
    if os.path.exists("tmp"):
        print "folder exists"
        shutil.rmtree("tmp")
    else:
        print "folder doesnt exist"
        os.mkdir("tmp")
        
@run_in_env("tmp")  
def run_simulation():
    """
    Takes in the variables to be written from the client forms
    and the params from the settings file which determines the
    order of the parameters. 
    Returns a parsed SigmaOutput instance of output file.
    """
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
    
    p = Popen(["%s\\model\\bin\\%s.exe" % (PROJECT_PATH, model), 
               expfile_name], stdout=PIPE, stderr=STDOUT)
    p.wait()
    
    out_inst = parse(outfile_name)
    os.remove(expfile_name)
    os.remove(outfile_name)
    
    return out_inst
    
def build_graphs(POST, graphs):

    return 0  