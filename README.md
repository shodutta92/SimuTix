#IEOR 131 SigmaWeb

##What is this?
SigmaWeb is a simple framework for easily and quickly creating web interfaces for the Sigma simulation software

##Settings
- The "settings.json" file holds all your custom variable Settings
- Each time the server starts, it will read your file and use them throughout

###info
    "info": {
        "team":   "Carwash Demo",
        "members":   "Sanketh, Soroush, Pranava, and Nathan",
        "description":  
            "This is a simulation demo of the classic Carwash model. I hope you enjoy!"
    },

- `team` is your Project or Team name
- `members` can be a comma sperated list of the team members
- `description` is a tagline description of the product

###sigma
    "sigma": {
        "model": "sigma/carwash.exe",    
        "dat": "",
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
    },
- `model` should be the path to your compiled model, see Preparation on how to create that file.
- `dat` is an optional parameter, if you used `DISK` commands in sigma to read from a `.dat` file, provide the path to the file.
- `parameters` is a list of parameter objects, the order of this list is **very** important! The order of the parameters must match the order in your sigma model, otherwise the model will break.

Element in the `parameter` list:
- `name` is the actual parameter name, for consistency, please use the same names you defined in your sigma model.
- `display` is the friendly version of the variable name, this is what will appear in simulation form, it should be something descriptive and easy to understand.
- `default` is the default value the form should begin at. When there get to be lots of parameters, it will be very cumbersome to enter in each one every time. Instead we can begin with a default value.


- The order of sigma.parameters is **very** important
    - It **must** match the order of the parameters in your Sigma model
- If the server is throwing a Key Error, you likely have a mismatch between a parameter in your settings.json and the Variables in your sigma model. Go back to sigma and make sure all parameters are defined.

- dat is optional!, If your model does not require a dat file, please leave the field blank.
