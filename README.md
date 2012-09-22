#SigmaWeb IEOR 131

##What is this?
SigmaWeb is a simple framework for easily and quickly creating web interfaces for the Sigma simulation software. 
It is built as a self-contained package which requires only a default python installation to run. 
SigmaWeb was built for use in IEOR 131: Discrete Event Simulation, taught at U.C. Berkeley.

##Preparation
- In order to use SigmaWeb, you must first prepare your sigma model to be compatible.

##Settings
- The `settings.json` file holds all your custom variable settings
- Each time the server starts, it will read your file and use them throughout

### Components:
###info
```javascript
"info": {
    "team":   "Carwash Demo",
    "members":   "Sanketh, Soroush, Pranava, and Nathan",
    "description":  
        "This is a simulation demo of the classic Carwash model. I hope you enjoy!"
}
```
- `team` is your project or team name
- `members` can be a comma sperated list of the team members
- `description` is a tagline description of the product

###sigma
```javascript
"sigma": {
    "model": "sigma/carwash.exe",    
    "parameters": [
        {   "name": "QUEUE",
            "display": "Initial Queue",
            "default": 0
        },

        {   "name": "SERVER",
            "display": "Number of Washers",
            "default": 4
        },

        {   "name": "RUNTIME",
            "display": "Running Time",
            "default": 1000
        }
    ]
}
```
- `model` should be the path to your compiled model, see Preparation on how to create that file.
- `parameters` is a list of parameter objects, the order of this list is **very** important! The order of the parameters must match the order in your sigma model, otherwise the model will break.

Item in the `parameter` list:
- `name` is the actual parameter name, for consistency, please use the same names you defined in your sigma model.
- `display` is the friendly version of the variable name, this is what will appear in simulation form, it should be something descriptive and easy to understand.
- `default` is the default value the form should begin at. When there get to be lots of parameters, it will be very cumbersome to enter in each one every time. Instead we can begin with a default value.

###graphs
```javascript
"graphs": [
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
- `graphs` is a list of graphs objects to be displayed after the simulation completes. The order signifies the display order of the graph on the results page.

Item in the `graph` list:
- `name` is the title that appears above the graph
- `x-axis` is the sigma variable to be used on the x-axis
- `x-display` is what is displayed on the x-axis, it can provide more description than the variable name
- `y-axis` is the sigma variable to be used on the y-axis
- `y-display` is what is displayed on the y-axis, it can provide more description than the variable name

###server
```javascript
"server": {
    "hostname": "localhost",
    "port": 9000
}
```

- `server` are advanced settings for running the server. If you are unsure what to do here, leave the default values, you should not have a problem.
- `hostname` is the ip address or host for the server to run on
- `port` is the port number

## Debugging
- If the server is throwing a Key Value Error, you likely have a mismatch between a parameter in your settings.json and the Variables in your sigma model. Go back to sigma and make sure all parameters are defined.
- If you are getting a JSON Decode error, you likely have incorrect formatting in your `settings.json` file. Compare it to the default included file and look for errors, like missing commas. You can also copy and paste the file into a [JSON validator](http://jsonlint.com/) which will help you find errors.
