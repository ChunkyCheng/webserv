#!/usr/bin/env python3

print("Content-Type: text/html\r")
print("Status: 200 OK\r")
print("X-Custom-Header: hello\r")
print("X-Another-Header: world\r")
print("\r")
print("<h1>Multiple Headers!</h1>")