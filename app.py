from util.stats import *
from util.parser import parse
from util.preprocessor import *
from random import randint
from time import time
from subprocess import Popen, PIPE, STDOUT
import os
from glob import glob
import json

PROJECT_PATH = os.getcwd()


def clean_tmp():
    """
    Removes any files leftover in the tmp directory.
    Also creates the tmp directory if it does not exist.
    """
    if not os.path.exists("tmp"):
        os.mkdir("tmp")
    else:
        for f in glob('tmp/*.exp'):
            os.unlink(f)
        for f in glob('tmp/*.out'):
            os.unlink(f)


def run_simulation(inputs, sigma_settings, wsock):
    """
    Takes in the variables to be written from the client form,
    the params from the settings file, and the websocket instance.
    Returns a parsed SigmaOutput instance of output file.
    """

    fileNames = prepareSimulation(inputs)
    if inputs['optimize']:
        out_inst = optimizeServers(inputs, sigma_settings, fileNames, wsock)
    else:
        out_inst = runSigma(sigma_settings, fileNames, inputs)
    return out_inst


def runSigma(sigma_settings, fileNames, inputs):
    p = Popen(
        ["%s\\sigma\\bin\\%s.exe" % (PROJECT_PATH, sigma_settings['model']),
            fileNames['expfile_name']], stdout=PIPE, stderr=STDOUT)
    p.wait()
    out_inst = parse(fileNames['outfile_name'], time(), inputs)
    os.remove(fileNames['expfile_name'])
    os.remove(fileNames['outfile_name'])
    return out_inst

def optimizeServers(inputs, sigma_settings, fileNames, wsock):
    """
    Finds the minimal number of servers needed to meet the condition
    that nobody misses their flight and that the queue does not exceed
    the length given in the inputs.
    """
    #initializes with a base number of servers and a non-optimized state
    optimized = False
    inputs['econServers'] = [0, 0, 0, 0, 0, 0]
    inputs['fcServers'] = [1, 1, 1, 1, 1, 1]
    randomseed = randint(0,65534)

    #Shift currently being optimized
    optimizeShift = 0

    while not optimized:
        #Run a new simulation
        fileNames = prepareSimulation(inputs, newDataFile = False, randomseed = randomseed)
        parsedOutput = runSigma(sigma_settings, fileNames, inputs)
        shiftLength = 1440/6.0
        maxEconQueues = []
        maxFCQueues = []
        missedFlights = []
        missedFlightsFC = []

        #Find the max queue lengths and number of missed flights
        for j in range(6):
            #Normal shifts
            shift = [i for i in parsedOutput.events if (i['Time'] < shiftLength * (j + 1) 
                and i['Time'] >= shiftLength * j)]
            #Shifts accounting for boarding time - useful for missed flight calculations
            shift2 = [i for i in parsedOutput.events if (i['Time'] < shiftLength * (j + 1) + 30 
                and i['Time'] >= shiftLength * j + 30)]
            if len(shift) > 0 and len(shift2) > 0:
                maxEconQueue = max(shift, key=lambda x: x['QUEUE[0]'])['QUEUE[0]']
                maxFCQueue = max(shift, key=lambda x: x['QUEUE[1]'])['QUEUE[1]']
                missedflight = shift2[len(shift2) - 1]['MISSFL']
                missedflightFC = shift2[len(shift2) - 1]['MISSFLFC']
                maxEconQueues.append(maxEconQueue)
                maxFCQueues.append(maxFCQueue)
                missedFlights.append(missedflight)
                missedFlightsFC.append(missedflightFC)
            else:
                maxEconQueues.append(0)
                maxFCQueues.append(0)
                missedFlights.append(0)
                missedFlightsFC.append(0)

        #Send result of run to browser to show as progress
        wsock.send(json.dumps({
          'econQueues': maxEconQueues,
          'fcQueues': maxFCQueues,
          'econServers': inputs['econServers'],
          'fcServers': inputs['fcServers'],
          'missfl': missedFlights,
          'missflfc': missedFlightsFC}))

        #Check for met constraints
        if (missedFlights[optimizeShift] == 0 
            and missedFlightsFC[optimizeShift] == 0 
            and maxEconQueues[optimizeShift] <= inputs['maxEcon'] 
            and maxFCQueues[optimizeShift] <= inputs['maxFC']):
            optimizeShift += 1
            if optimizeShift == len(inputs['econServers']):
                optimized = True

        #If not met, increment server and run again
        else:
            if missedFlightsFC[optimizeShift] > 0 or maxFCQueues[optimizeShift] > inputs['maxFC']:
                inputs['fcServers'][optimizeShift] += 1
            elif missedFlights[optimizeShift] > 0 or maxEconQueues[optimizeShift] > inputs['maxEcon']:
                inputs['econServers'][optimizeShift] += 1

    #Set output as optimized
    parsedOutput.setOptimized(True)
    return parsedOutput


def prepareSimulation(inputs, newDataFile = True, randomseed = None):
    """
    Prepares the exp, data, and output files
    """
    if randomseed is None:
        randomseed = randint(0,65534)
    dataFileName = 'data.dat'
    if newDataFile:
        createDataFile(inputs['flights'], dataFileName)
    f_name = int(time())
    outfile_name = "%s.out" % f_name
    expfile_name = "%s.exp" % f_name
    exp = [outfile_name, "n", str(randomseed), str(inputs['runTime']), "1"]

    exp.append(str(inputs['fcPercent']))

    for param in inputs['econServers']:
        exp.append(str(param))

    for param in inputs['fcServers']:
        exp.append(str(param))

    expstr = " ".join(exp)

    expfile = open(expfile_name, 'w')
    expfile.write(expstr)
    expfile.flush()
    expfile.close()
    return {'dataFileName': dataFileName, 'outfile_name': outfile_name, 'expfile_name': expfile_name}