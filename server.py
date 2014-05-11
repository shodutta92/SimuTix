from bottle import *
import settings
import util.compile as compile
from app import *
import json
from gevent.pywsgi import WSGIServer
from geventwebsocket.handler import WebSocketHandler
from geventwebsocket import WebSocketError

app = Bottle()

#File where runs will be stored
runsFile = 'runs.txt'

@app.get('/')
def index():
    return template("index", {"info": settings.info})


@app.get('/simulate')
def simulate_get():
    return template("simulate", {"info": settings.info,
                                 "sigma": settings.sigma})


@app.post('/simulate')
def simulate_post():
    parsed = run_simulation(request.json, settings.sigma, wsock)
    parsedJSON = parsed.toJSON()
    with open(runsFile, 'ab') as f:
        f.write(parsedJSON + '\n')


@app.get('/results')
def results_get():
    return template("results")

@app.get('/fetchResults')
def fetchResults_get():
    global cursor
    try:
        with open(runsFile, 'rb') as f:
            testResults = f.readlines()
    except:
        testResults = []
    return {'data': testResults}

@app.get('/websocket')
def handle_websocket():
    global wsock
    wsock = request.environ.get('wsgi.websocket')
    if not wsock:
        abort(400, 'Expected WebSocket request.')

    #Receives websocket messages - not used here but could be
    while True:
        try:
            message = wsock.receive()
            wsock.send("Your message was: %r" % message)
        except WebSocketError:
            break

@app.error(404)
def error404(error):
    return "Sorry, can't find that page!"

# Static Routes
@app.get('/<filename:re:.*\.js>')
def javascripts(filename):
    return static_file(filename, root='static/js')


@app.get('/<filename:re:.*\.css>')
def stylesheets(filename):
    return static_file(filename, root='static/css')


@app.get('/<filename:re:.*\.(jpg|png|gif|ico)>')
def images(filename):
    return static_file(filename, root='static/img')


@app.get('/<filename:re:.*\.(eot|ttf|woff|svg)>')
def fonts(filename):
    return static_file(filename, root='static/fonts', mimetype='font/truetype')


# Re-Compile Sigma Model and Clean\Create tmp directory
clean_tmp()
compile.build(settings.sigma['model'])

# Custom Template Path and Run Server with Debug on
TEMPLATE_PATH.insert(0, "./templates/")

#initialize websocket variable
wsock = None

#start server
print "SigmaWeb Running At: http://%s:%s" % (settings.server['hostname'], settings.server['port'])

server = WSGIServer((settings.server['hostname'], settings.server['port']), app,
                    handler_class=WebSocketHandler)
server.serve_forever()