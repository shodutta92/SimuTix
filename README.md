#IEOR 131 SigmaWeb

##What is this?
SigmaWeb is a simple framework for easily and quickly creating web interfaces for the Sigma simulation software

##Settings
- The "settings.json" file holds all your custom variable Settings
- Each time the server starts, it will read your file and use them throughout
- The order of sigma.parameters is **very** important
    - It **must** match the order of the parameters in your Sigma model

- If the server is throwing a Key Error, you likely have a mismatch between a parameter in your settings.json and the Variables in your sigma model. Go back to sigma and make sure all parameters are defined.

- dat is optional!, If your model does not require a dat file, please leave the field blank.
