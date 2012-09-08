from bottle import *
import json
import sigma.parser as parser
from sigma.app import *

# Import Settings Into a Dictionary 
settings = json.load(open('settings.json'))

# Main Page
@get('/')
def index():  
    settings['__extra'] = {'simulate': "", 'results': ""}
    return template("index", settings)

@get('/simulate')
def simulate():  
    settings['__extra'] = {'simulate': "active", 'results': ""}
    return template("simulate", settings)

@get('/results')
def simulate():  
    settings['__extra'] = {'simulate': "", 'results': "active"}
    return template("results", settings)

# Error 404 (when page is not found)
@error(404)
def error404(error):
    return "Sorry, can't find that page!"

# Static Routes
@get('/<filename:re:.*\.js>')
def javascripts(filename):
    return static_file(filename, root='static/js')

@get('/<filename:re:.*\.css>')
def stylesheets(filename):
    return static_file(filename, root='static/css')

@get('/<filename:re:.*\.(jpg|png|gif|ico)>')
def images(filename):
    return static_file(filename, root='static/img')

@get('/<filename:re:.*\.(eot|ttf|woff|svg)>')
def fonts(filename):
    return static_file(filename, root='static/fonts')
  
# Custom Template Path and Run Server with Debug on
TEMPLATE_PATH.insert(0, "./templates/")
debug(True)
run(host='localhost', port=9000, reloader=True)
