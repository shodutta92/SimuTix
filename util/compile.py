from subprocess import Popen
import os, sys
from decorators import run_in_env

INCLUDE_PATH_PREFIX = "C:\\PROGRA~2"
if not os.path.exists("%s\\BIOPRO~1" % INCLUDE_PATH_PREFIX):
    INCLUDE_PATH_PREFIX = "C:\\PROGRA~1"

STD_C_INCLUDES = "%s\\BIOPRO~1\\Sigma\\Libs" % INCLUDE_PATH_PREFIX


def fix(model):
    source_file = open("src\\%s.c" % model, "r")
    fixed_file = open("src\\%s_fixed.c" % model, "w")
    old = source_file.read()
    new = old.replace("exp_file_name = _strdup(argv[0]);", "exp_file_name = _strdup(argv[1]);")
    fixed_file.write(new)
    fixed_file.flush()
    fixed_file.close()

def compile(model):
    compile_p = Popen(["%s\\wpp386" % STD_C_INCLUDES,
           "src\\%s.c" % model,
           "-bc", "-d0", "-w3", "-xs", "-ot", "-bt=nt", 
           "-3s", "-mf", "-q", "-i=%s\\" % STD_C_INCLUDES, 
           "-i=%s\\std_inc" % STD_C_INCLUDES])
    compile_p.wait()

def link(model):
    link_p = Popen(["%s\\wlink" % STD_C_INCLUDES, 
            "name", "bin\%s.exe" % model,
            "file", "%s_fixed.obj" % model,
            "option", "quiet",
            "libpath", "%s\std_lib" % STD_C_INCLUDES,
            "libpath", STD_C_INCLUDES,
            "library", "kernel32,user32,advapi32,Sigmalib",
            "format", "windows", "nt",
            "runtime", "console=4.0"])	
    link_p.wait()

@run_in_env("sigma") 
def build(model):
    fix(model)
    
    if os.path.exists("bin\\%s.exe" % model):
        os.remove("bin\\%s.exe" % model)   
    if os.path.exists("%s_fixed.obj" % model):
        os.remove("%s_fixed.obj" % model)

    compile("%s_fixed" % model)
    link(model)

    if os.path.exists("%s_fixed.obj" % model):
        os.remove("%s_fixed.obj" % model)       
    if os.path.exists("src\\%s_fixed.c" % model):
        os.remove("src\\%s_fixed.c" % model)    