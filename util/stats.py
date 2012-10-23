# Some Stats Utilities
from math import sqrt

def mean(values):
    """
    Takes in a list of values and returns the mean
    """
    return float(sum(values)) / len(values)

def median(values):
    """
    Takes in a list of values and returns the median
    """
    values.sort()
    length = len(values)
    if length % 2 == 1:
        med = values[(length-1)/2]
    else:
        med = float(values[(length-1)/2] + values[length/2]) / 2
    return med

def stdev(values):
    """
    Calls the variance function on the list of values, 
    and returns the square root.
    """
    return sqrt(variance(values))

def variance(values):
    """ 
    Calculates the online variance for a sample population
    """
    n = 0
    mean = 0
    M2 = 0

    for x in values:
        n = n + 1
        delta = x - mean
        mean = mean + delta/n
        M2 = M2 + delta*(x - mean)

    variance = M2/(n - 1)
    return variance
