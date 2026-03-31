#!/usr/bin/env python3
import os

print("Content-Type: text/html\r")
print("Status: 200 OK\r")
print("\r")
print("<h1>ENV TEST</h1>")
print(f"METHOD: {os.environ.get('REQUEST_METHOD', 'NOT SET')}")
print(f"QUERY: {os.environ.get('QUERY_STRING', 'NOT SET')}")