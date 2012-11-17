info = {
    "team":   "Carwash Demo",
    "members":   "Sanketh, Soroush, Pranava, and Nathan",
    "description":  
        """This is a simulation demo of the classic Carwash model. 
           I hope you enjoy!""",
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

sigma = {
    "model": "Car_Wash",    
    "runtime":{"name": "RUNTIME",
               "display": "Running Time",
               "default": 10000
              },
    "parameters": [
        {   "name": "QUEUE",
            "display": "Initial Queue",
            "default": 3
        },

        {   "name": "SERVERS",
            "display": "Number of Washers",
            "default": 4
        },
    ]
}

rdp_enable = True

graphs = [
    {   "graph_name": "Queue and Server vs Time",
        "x-axis": "Time",
        "x-display": "Time (seconds)",
        "y-axis": ["QUEUE", "SERVERS"],
        "y-display": "Number in Queue or Servers Idle",
        "lines": ["Number In Queue", "Number In Server"],
        "rdp_epsilon": [1, 1.5]
    },

    {   "graph_name": "Server vs Time",
        "x-axis": "Time",
        "x-display": "Time (seconds)",
        "y-axis": ["SERVERS"],
        "y-display": "Servers Available",
        "lines": ["Server vs Time"],
        "rdp_epsilon": [1]
    }
]

server = {
    "hostname": "localhost",
    "port": 9000
}
