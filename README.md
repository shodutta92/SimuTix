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
- 'description' is a tagline description of the product




- The order of sigma.parameters is **very** important
    - It **must** match the order of the parameters in your Sigma model
- If the server is throwing a Key Error, you likely have a mismatch between a parameter in your settings.json and the Variables in your sigma model. Go back to sigma and make sure all parameters are defined.

- dat is optional!, If your model does not require a dat file, please leave the field blank.
