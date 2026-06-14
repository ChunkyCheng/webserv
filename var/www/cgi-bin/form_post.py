#!/usr/bin/env python3
import os
import sys

try:
    from urllib.parse import parse_qs, unquote_plus
except ImportError:
    from urlparse import parse_qs
    from urllib import unquote_plus

method  = os.environ.get('REQUEST_METHOD', '')
length  = int(os.environ.get('CONTENT_LENGTH', '0') or '0')
raw     = sys.stdin.read(length) if length > 0 else ''

rows = ''
if raw:
    for pair in raw.split('&'):
        if '=' in pair:
            k, v = pair.split('=', 1)
            rows += '<tr><td>' + unquote_plus(k) + '</td><td>' + unquote_plus(v) + '</td></tr>'
    if not rows:
        rows = '<tr><td colspan="2">No fields parsed (empty or malformed body)</td></tr>'
else:
    rows = '<tr><td colspan="2">No body received (use POST method)</td></tr>'

body = (
    '<html><head><style>'
    'body{font-family:monospace;background:#0a0b0e;color:#c8cdd8;padding:20px}'
    'table{border-collapse:collapse;width:100%}'
    'td{border:1px solid #2a2d38;padding:6px 12px}'
    'td:first-child{color:#ffcc00;width:160px}'
    'h2{color:#f0f2f7;margin-bottom:8px}'
    'p{color:#555a6b;margin-bottom:16px;font-size:12px}'
    '</style></head><body>'
    '<h2>POST Form Data</h2>'
    '<p>Method: ' + method + ' &nbsp;|&nbsp; Content-Length: ' + str(length) + '</p>'
    '<table>' + rows + '</table>'
    '</body></html>'
)

sys.stdout.write('Content-Type: text/html\r\n')
sys.stdout.write('Content-Length: ' + str(len(body)) + '\r\n')
sys.stdout.write('\r\n')
sys.stdout.write(body)
