#!/usr/bin/env python3
import os
import sys

cgi_vars = [
    'REQUEST_METHOD', 'QUERY_STRING', 'CONTENT_TYPE', 'CONTENT_LENGTH',
    'SCRIPT_NAME', 'PATH_INFO', 'SERVER_NAME', 'SERVER_PORT',
    'SERVER_PROTOCOL', 'GATEWAY_INTERFACE', 'REDIRECT_STATUS',
]

rows = ''
for key in cgi_vars:
    val = os.environ.get(key, '<not set>')
    rows += '<tr><td>' + key + '</td><td>' + val + '</td></tr>'

# Also show any HTTP_ vars set by the server
for k, v in os.environ.items():
    if k.startswith('HTTP_'):
        rows += '<tr><td>' + k + '</td><td>' + v + '</td></tr>'

body = (
    '<html><head><style>'
    'body{font-family:monospace;background:#0a0b0e;color:#c8cdd8;padding:20px}'
    'table{border-collapse:collapse;width:100%}'
    'td{border:1px solid #2a2d38;padding:6px 12px}'
    'td:first-child{color:#39ff14;width:220px}'
    'h2{color:#f0f2f7;margin-bottom:16px}'
    '</style></head><body>'
    '<h2>CGI Environment Variables</h2>'
    '<table>' + rows + '</table>'
    '</body></html>'
)

sys.stdout.write('Content-Type: text/html\r\n')
sys.stdout.write('Content-Length: ' + str(len(body)) + '\r\n')
sys.stdout.write('\r\n')
sys.stdout.write(body)
