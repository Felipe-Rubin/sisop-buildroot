# T1 Felipe Rubin & Guilherme Girotto
# Lab Sisop 2018/2
#
#
import time
import BaseHTTPServer
# from subprocess import call
import subprocess



HOST_NAME = '0.0.0.0' #This is for localhost access
PORT_NUMBER = 80 #The Port will be 80, default http

def get_time():
    return subprocess.check_output(["date", "+%H:%M:%S"])
def get_date():
    return subprocess.check_output(["date", "+%d/%m/%Y"])
    # return subprocess.check_output(["ls", "-l"])
def get_upiddle():
    aux = subprocess.check_output(["cat", "/proc/uptime"])
    splitted = aux.split(' ')
    return "system uptime " + str(splitted[0]) + " system iddletime " + str(splitted[1])
#Note: Don't use '' on subprocess
#Note2: awk will receive a string so its not necessary to put " "{}" "
def get_upiddle2():
    p1 = subprocess.Popen(["cat", "/proc/uptime"], stdout=subprocess.PIPE)
    p2 = subprocess.Popen(["awk", "{print \"system uptime: \" $1 \" system iddletime: \" $2}"], stdin=p1.stdout, stdout=subprocess.PIPE)
    p1.stdout.close()  # Allow p1 to receive a SIGPIPE if p2 exits.
    output,err = p2.communicate()
    return output


print get_upiddle2()

class MyHandler(BaseHTTPServer.BaseHTTPRequestHandler):
    def do_HEAD(s):
        s.send_response(200)
        s.send_header("Content-type", "text/html")
        s.end_headers()
    def do_GET(s):
        """Respond to a GET request."""
        s.send_response(200)
        s.send_header("Content-type", "text/html")
        s.end_headers()
        s.wfile.write("<html>")
        s.wfile.write("<head><title>Web Monitor</title></head>")
        s.wfile.write("<body><p>This is a test.</p>")
        s.wfile.write("<h1>"+"Date & Time: "+get_date()+" "+get_time()+"</h1>")
        s.wfile.write("<h1>"+get_upiddle()+"</h1>")
        s.wfile.write("<h1>"+get_upiddle2()+"</h1>")
        # If someone went to "http://something.somewhere.net/foo/bar/",
        # then s.path equals "/foo/bar/".
        # s.wfile.write("<p>You accessed path: %s</p>" % s.path)

        s.wfile.write("</body>")
        s.wfile.write("</html>")

if __name__ == '__main__':
    server_class = BaseHTTPServer.HTTPServer
    httpd = server_class((HOST_NAME, PORT_NUMBER), MyHandler)
    # print time.asctime(), "Server Starts - %s:%s" % (HOST_NAME, PORT_NUMBER)
    try:
        httpd.serve_forever()
    except KeyboardInterrupt:
        pass
    httpd.server_close()
    # print time.asctime(), "Server Stops - %s:%s" % (HOST_NAME, PORT_NUMBER)

