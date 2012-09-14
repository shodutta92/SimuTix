#IEOR 131 SigmaWeb

##What is this?
SigmaWeb is a simple framework for easily and quickly creating web interfaces for the Sigma simulation software

##Settings
- The "settings.json" file holds all your custom variable Settings
- Each time the server starts, it will read your file and use them throughout

###info
    info: {
        "team": "",
        "members": "",
        "description: "",
    }






- Example settings.json:
    {   
        "info": {
            "team":   "Carwash Demo",
            "members":   "Sanketh, Soroush, Pranava, and Nathan", 
            "description":  
                "This is a simulation demo of the classic Carwash model. I hope you enjoy!" 
        },

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

        "graphs": [
            {   "name": "Queue vs Time",
                "x-axis": "TIME",
                "y-axis": "QUEUE"
            },

            {   "name": "Server vs Time",
                "x-axis": "TIME",
                "y-axis": "SERVER"
            }

        ],

        "server": {
            "hostname": "localhost",
            "port": 9000
        }
    }
     
- The order of sigma.parameters is **very** important
    - It **must** match the order of the parameters in your Sigma model
- If the server is throwing a Key Error, you likely have a mismatch between a parameter in your settings.json and the Variables in your sigma model. Go back to sigma and make sure all parameters are defined.

- dat is optional!, If your model does not require a dat file, please leave the field blank.
