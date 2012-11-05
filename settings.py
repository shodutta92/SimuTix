info = {
    "team":   "Carwash Demo",
    "members":   "Sanketh, Soroush, Pranava, and Nathan",
    "description":  
        """This is a simulation demo of the classic Carwash model. 
           I hope you enjoy!""",
    "additional":
        """<h3>Additional Info</h3>
        <ul>
            <li>LOREM IPSUM DOLOR</li>
            <li>LOREM IPSUM DOLOR</li>
            <li>LOREM IPSUM DOLOR</li>
            <li>LOREM IPSUM DOLOR</li>
            <li>LOREM IPSUM DOLOR</li>
        </ul>
        """
}

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

graphs = [
    {   "name": "Queue vs Time",
        "x-axis": "Time",
        "x-display": "Time (seconds)",
        "y-axis": "QUEUE",
        "y-display": "Number in Queue"
    },

    {   "name": "Server vs Time",
        "x-axis": "Time",
        "x-display": "Time (seconds)",
        "y-axis": "SERVERS",
        "y-display": "Servers Available"
    }
]

server = {
    "hostname": "localhost",
    "port": 9000
}
