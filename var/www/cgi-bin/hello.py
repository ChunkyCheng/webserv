#!/usr/bin/env python3
import os
import sys

method = os.environ.get('REQUEST_METHOD', '')
query  = os.environ.get('QUERY_STRING', '')

body = "<html><body><h1>CGI GET works</h1>"
body += "<p>Method: " + method + "</p>"
body += "<p>Query: " + query + "</p>"
body += "</body></html>"

sys.stdout.write("Content-Type: text/html\r\n")
sys.stdout.write("Content-Length: " + str(len(body)) + "\r\n")
sys.stdout.write("\r\n")
sys.stdout.write(body)
