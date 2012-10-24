from bottle import *
import settings
import util.compile as compile

# Home Page
@get('/')
def index():  
    return template("index", {"info": settings.info})

# Simulate Page
@get('/simulate')
def simulate():  
    return template("simulate", {"info": settings.info, 
                                 "sigma": settings.sigma})

# Results Page
@get('/results')
def simulate():  
    return template("results", {"info": settings.info})

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
  
# Re-Compile Sigma Model
compile.build(settings.sigma['model'])

# Custom Template Path and Run Server with Debug on
TEMPLATE_PATH.insert(0, "./templates/")
debug(True)
run(host=settings.server['hostname'], port=settings.server['port'], reloader=True)
