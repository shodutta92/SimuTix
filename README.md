#SigmaWeb IEOR 131

##What is this?
SigmaWeb is a simple framework for easily and quickly creating web interfaces for the Sigma simulation software. 
It is built as a self-contained package which requires only a default python installation to run. 
SigmaWeb was built for use in IEOR 131: Discrete Event Simulation, taught at U.C. Berkeley.

##Preparation
- In order to use SigmaWeb, you must first prepare your sigma model to be compatible.
- All variables that you want users to input should be placed in your `RUN` Event's parameters
- Then, do a Fast C compile and copy the `.c` file and place it in `SigmaWeb\sigma\src`
- If your model uses a `.dat` file place the file in `SigmaWeb\tmp` folder.

##Running
- To run the server, double click on `server.py`, then open up your favorite browser and navigate to the given url

##Settings
- The `settings.py` file holds all your custom variable settings
- Each time the server starts, it will read your file and use them throughout

### Components:
###info
```python
info = {
    "team":   "Carwash Demo",
    "members":   "Sanketh, Soroush, Pranava, and Nathan",
    "description":  
        """This is a simulation demo of the classic Carwash model. 
           I hope you enjoy!"""
    "additional":
        """<h3>Additional Info:</h3>
        <ul>
            <li>LOREM IPSUM DOLOR</li>
            <li>LOREM IPSUM DOLOR</li>
            <li>LOREM IPSUM DOLOR</li>
            <li>LOREM IPSUM DOLOR</li>
            <li>LOREM IPSUM DOLOR</li>
        </ul>
        """
}
```
- `team` is your project or team name
- `members` names of the team members
- `description` is a description of the product _note: HTML formatting is allowed here_
- `additional` shows up on the sidebar of the simulate page, use it to give additional information to your users about run parameters _note: HTML formatting is allowed here_

###sigma
```python
sigma = {
    "model": "Car_Wash",    
    "runtime":{"name": "RUNTIME",
               "display": "Running Time",
               "default": 1000
              },
    "parameters": [
        {   "name": "QUEUE",
            "display": "Initial Queue",
            "default": 0
        },

        {   "name": "SERVERS",
            "display": "Number of Washers",
            "default": 4
        },
    ]
}
```
- `model` should be the name of your `.c` file. However do **NOT** put the extension in this field.
- `runtime` is a special run parameter, you should only need to change `display` and/or `default` here.
- `parameters` is a list of parameter dictionaries, the order of this list is **very** important! The order of the parameters must match the order in your sigma model, otherwise the model will break.

Item in the `parameter` list:
- `name` is the actual parameter name, for consistency, please use the same names you defined in your sigma model.
- `display` is the friendly version of the variable name, this is what will appear in simulation form, it should be something descriptive and easy to understand.
- `default` is the default value the form should begin at. When there get to be lots of parameters, it will be very cumbersome to enter in each one every time. Instead we can begin with a default value.

###graphs
```python
graphs = [
    {   "name": "Queue vs Time",
        "x-axis": "TIME",
        "x-display": "Time (seconds)",
        "y-axis": "QUEUE",
        "y-display": "Number in Queue"
    },

    {   "name": "Server vs Time",
        "x-axis": "TIME",
        "x-display": "Time (seconds)",
        "y-axis": "SERVER",
        "y-display": "Servers Available"
    }

]
```
- `graphs` is a list of graph dictionaries to be displayed after the simulation completes. The order signifies the display order of the graphs on the results page.

Item in the `graph` list:
- `name` is the title that appears above the graph
- `x-axis` is the sigma variable to be used on the x-axis
- `x-display` is what is displayed on the x-axis, it can provide more description than the variable name
- `y-axis` is the sigma variable to be used on the y-axis
- `y-display` is what is displayed on the y-axis, it can provide more description than the variable name

###server
```python
server = {
    "hostname": "localhost",
    "port": 9000
}
```

- `server` are advanced settings for running the server. If you are unsure what to do here, leave the default values, you should not have a problem.
- `hostname` is the ip address or host for the server to run on
- `port` is the port number