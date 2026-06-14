#!/usr/bin/env python3
import os
import sys

SESSIONS_DIR = 'sessions'

# ── helpers ──────────────────────────────────────────────────────────────────

def parse_session_id(raw_cookie):
    """Return session_id value from Cookie header string, or '' if absent."""
    for part in raw_cookie.split(';'):
        part = part.strip()
        if part.startswith('session_id='):
            return part[len('session_id='):]
    return ''

def is_valid_sid(sid):
    """Accept only 32-char lowercase hex — blocks path traversal."""
    return len(sid) == 32 and all(c in '0123456789abcdef' for c in sid)

def session_path(sid):
    return os.path.join(SESSIONS_DIR, sid + '.txt')

def read_count(sid):
    try:
        with open(session_path(sid)) as f:
            return int(f.read().strip())
    except (IOError, OSError, ValueError):
        return 0

def write_count(sid, n):
    if not os.path.isdir(SESSIONS_DIR):
        os.makedirs(SESSIONS_DIR)
    with open(session_path(sid), 'w') as f:
        f.write(str(n))

# ── resolve session ───────────────────────────────────────────────────────────

raw_cookie = os.environ.get('HTTP_COOKIE', '')
sid        = parse_session_id(raw_cookie)

is_new = not is_valid_sid(sid) or not os.path.exists(session_path(sid))
if is_new:
    sid = os.urandom(16).hex()

count = read_count(sid) + 1
write_count(sid, count)

# ── build body ────────────────────────────────────────────────────────────────

status_text = '&#10024; New session &mdash; first visit.' if is_new else '&#8635; Existing session &mdash; welcome back.'

body = """\
<!DOCTYPE html>
<html lang="en">
<head>
  <meta charset="UTF-8">
  <title>Visit Counter</title>
  <link href="https://fonts.googleapis.com/css2?family=Space+Mono:wght@400;700&display=swap" rel="stylesheet">
  <style>
    *, *::before, *::after {{ box-sizing: border-box; margin: 0; padding: 0; }}
    body {{
      background: #0a0b0e; color: #c8cdd8;
      font-family: 'Space Mono', 'Courier New', monospace; font-size: 13px;
      display: flex; align-items: center; justify-content: center; min-height: 100vh;
    }}
    .card {{ border: 1px solid #2a2d38; background: #111318; padding: 32px 40px; width: 500px; }}
    .label {{ font-size: 10px; letter-spacing: .2em; text-transform: uppercase; color: #555a6b; margin-bottom: 4px; }}
    .value {{ font-size: 13px; color: #f0f2f7; margin-bottom: 20px; word-break: break-all; }}
    .count {{ font-size: 56px; font-weight: 700; color: #39ff14; letter-spacing: .05em; margin-bottom: 12px; }}
    .status {{ color: #ffcc00; margin-bottom: 24px; font-size: 12px; }}
    .note {{ font-size: 11px; color: #555a6b; line-height: 1.8; border-top: 1px solid #2a2d38; padding-top: 16px; }}
    .tag {{ color: #00e5ff; }}
  </style>
</head>
<body>
  <div class="card">
    <div class="label">session id</div>
    <div class="value">{sid}</div>
    <div class="label">visit count</div>
    <div class="count">{count}</div>
    <div class="status">{status_text}</div>
    <div class="note">
      <span class="tag">Cookie :</span> session_id={sid}<br>
      <span class="tag">Storage:</span> count lives in sessions/{sid}.txt (server-side file)<br>
      <span class="tag">Privacy:</span> cookie holds only an opaque ID &mdash; count never sent to browser
    </div>
  </div>
</body>
</html>""".format(sid=sid, count=count, status_text=status_text)

# ── emit response ─────────────────────────────────────────────────────────────

sys.stdout.write("Content-Type: text/html\r\n")
sys.stdout.write("Set-Cookie: session_id=" + sid + "; Path=/; HttpOnly\r\n")
sys.stdout.write("Content-Length: " + str(len(body.encode('utf-8'))) + "\r\n")
sys.stdout.write("\r\n")
sys.stdout.write(body)
