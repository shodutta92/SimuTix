from subprocess import Popen
import os, sys

INCLUDE_PATH_PREFIX = "C:\PROGRA~2"
if not os.path.exists("%s\BIOPRO~1" % INCLUDE_PATH_PREFIX):
    INCLUDE_PATH_PREFIX = "C:\PROGRA~1"

STD_C_INCLUDES = "%s\BIOPRO~1\Sigma\Libs" % INCLUDE_PATH_PREFIX

def compile(model):
    compile_p = Popen(["%s\wpp386" % STD_C_INCLUDES,
           "src\%s.c" % model,
           "-bc", "-d0", "-w3", "-xs", "-ot", "-bt=nt", 
           "-3s", "-mf", "-q", "-i=%s\\" % STD_C_INCLUDES, 
           "-i=%s\std_inc" % STD_C_INCLUDES])
    compile_p.wait()

def link(model):
    link_p = Popen(["%s\wlink" % STD_C_INCLUDES, 
            "name", "bin\%s.exe" % model,
            "file", "%s.obj" % model,
            "option", "quiet",
            "libpath", "%s\std_lib" % STD_C_INCLUDES,
            "libpath", STD_C_INCLUDES,
            "library", "kernel32,user32,advapi32,Sigmalib",
            "format", "windows", "nt",
            "runtime", "console=4.0"])	
    link_p.wait()
    
def build(model):
    original_path = os.getcwd()
    os.chdir("%s\model" % original_path)

    if os.path.exists("bin\%s.exe" % model):
        os.remove("bin\%s.exe" % model)   
    if os.path.exists("%s.obj" % model):
        os.remove("%s.obj" % model)

    compile(model)
    link(model)

    if os.path.exists("%s.obj" % model):
        os.remove("%s.obj" % model)    
    
    os.chdir(original_path)