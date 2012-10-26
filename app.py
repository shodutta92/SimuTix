from util.stats import *
from util.parser import parse
from util.decorators import run_in_env
from random import randint
from time import time
from subprocess import Popen, PIPE, STDOUT
import os, sys, shutil

PROJECT_PATH = os.getcwd()

def clean_tmp():
    """
    Removes any files leftover in the tmp directory. 
    Also creates the tmp directory if it does not exist.
    """
    if os.path.exists("tmp"):
        shutil.rmtree("tmp")
    os.mkdir("tmp")
        
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
    
    p = Popen(["%s\\model\\bin\\%s.exe" % (PROJECT_PATH, sigma_settings['model']), 
               expfile_name], stdout=PIPE, stderr=STDOUT)
    p.wait()
    
    out_inst = parse(outfile_name)
    os.remove(expfile_name)
    os.remove(outfile_name)
    
    return out_inst
    
def build_graphs(POST, graphs):

    return 0  