#!/usr/bin/env python3
import os, sys

content_length = os.environ.get('CONTENT_LENGTH', '0')
body = sys.stdin.read(int(content_length)) if content_length != '0' else sys.stdin.read()

print("Content-Type: text/html\r")
print("Status: 200 OK\r")
print("\r")
print(f"<p>Received body: {body}</p>")