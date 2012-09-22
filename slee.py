from time import sleep

"""
Demonstrates a long running process
"""

x = 0
while x < 10:
    x += 1
    print x
    sleep(.5)
