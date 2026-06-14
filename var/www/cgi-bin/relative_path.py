#!/usr/bin/env python3
import os
import sys

cwd = os.getcwd()

try:
    with open('data.txt', 'r') as f:
        content = f.read()
    status = 'SUCCESS'
    status_color = '#39ff14'
    file_block = '<pre style="background:#111318;padding:12px;white-space:pre-wrap">' + content + '</pre>'
except IOError as e:
    status = 'FAILED: ' + str(e)
    status_color = '#ff3300'
    file_block = '<p style="color:#ff3300">Could not open data.txt — chdir may not have run</p>'

body = (
    '<html><head><style>'
    'body{font-family:monospace;background:#0a0b0e;color:#c8cdd8;padding:20px}'
    'h2{color:#f0f2f7;margin-bottom:12px}'
    '.label{color:#555a6b;font-size:12px}'
    '</style></head><body>'
    '<h2>Relative Path Test — Python</h2>'
    '<p class="label">Working directory: <span style="color:#00e5ff">' + cwd + '</span></p>'
    '<p class="label">open("data.txt") result: <span style="color:' + status_color + '">' + status + '</span></p>'
    '<br>' + file_block +
    '</body></html>'
)

sys.stdout.write('Content-Type: text/html\r\n')
sys.stdout.write('Content-Length: ' + str(len(body)) + '\r\n')
sys.stdout.write('\r\n')
sys.stdout.write(body)
