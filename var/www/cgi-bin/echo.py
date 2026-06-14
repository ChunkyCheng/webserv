#!/usr/bin/env python3
import os
import sys
import json

method  = os.environ.get('REQUEST_METHOD', '')
query   = os.environ.get('QUERY_STRING', '')
length  = int(os.environ.get('CONTENT_LENGTH', '0') or '0')
body    = sys.stdin.read(length) if length > 0 else ''

payload = json.dumps({"method": method, "query": query, "body": body})

sys.stdout.write("Content-Type: application/json\r\n")
sys.stdout.write("Content-Length: " + str(len(payload)) + "\r\n")
sys.stdout.write("\r\n")
sys.stdout.write(payload)
