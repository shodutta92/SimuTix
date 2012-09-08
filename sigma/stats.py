# Some Stats Utilities

def mean(values):
    """
    Takes in a list of values and returns the mean
    """
    return float(sum(values)) / len(values)

def median(values):
    values.sort()
    length = len(values)
    if length % 2 == 1:
        med = values[(length-1)/2]
    else:
        med = float(values[(length-1)/2] + values[length/2]) / 2
    return med

def stdev(values):
    return 0

def variance(values):
    sd = stdev(values)
    return sd*sd
