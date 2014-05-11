#SimuTix

Note: SimuTix was based off the SigmaWeb project found [here](https://github.com/sankethkatta/SigmaWeb). Look there for a more general example of how to attach a web interface to Sigma simulation models, as well as more information on the structure of SigmaWeb.

##What is SimuTix?
SimuTix allows airlines to determine the optimal number of staff to have at check-in desks during various shifts throughout the day. SimuTix allows you to input your flight schedule, and then either check the load with a given number of servers, or to automatically find the minimum number of servers to ensure that no flights are missed and that the queues do not exceed a maximum value.

##Prerequisites
- Install [Sigma](http://sigmawiki.com)
- Install Python v2.7
- Install the gevent-websocket python package

##Running
- To run the server, double click on `server.py`
- Open SimuTix in your browser and navigate to `http://localhost:9000`
- SimuTix should run on most browsers, but it has only been tested with Google Chrome, so some of the styling elements may not work in other browsers.

##Simulating
- Choose the 'simulate' option to run the model with the number of servers you have chosen
- Choose the 'optimize' option to determine how many servers need to be run to meet your constraints

##Results
- The graph will show you how the system performed over time
- The different lines on the graph can be toggled by click on their name in the legend
- Double-clicking the name of a line in the legend will only show that line on the graph
- The sidebar shows previous simulations you have run with statistics about the run. Click on the run show it on the graph.