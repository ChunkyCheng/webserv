### Prerequisites

```bash
# Python 3 required
python3 --version

# Shell scripts need this added to webserv.conf's /cgi-bin location block:
# cgi .sh /bin/bash
```

---

### `get_hello.py` — Hello World (GET/POST)

Reads `first_name` and `last_name` from query string or POST body.

```bash
# GET with query string
curl "http://localhost:8080/cgi-bin/get_hello.py?first_name=John&last_name=Doe"

# POST with form data
curl -X POST -d "first_name=John&last_name=Doe" http://localhost:8080/cgi-bin/get_hello.py
```

---

### `time.py` — Current Time

Returns current server time as HTML.

```bash
curl http://localhost:8080/cgi-bin/time.py
```

---

### `env.py` — Environment Variables

Dumps all CGI environment variables the server passes to the script.

```bash
curl http://localhost:8080/cgi-bin/env.py
```

---

### `calc.py` — Calculator (GET/POST)

Parameters: `f_num`, `oper` (`+`, `-`, `*`, `/`, `^`, `r`), `s_num`. Numbers must be ≤ 10000.

```bash
# Addition: 5 + 3
curl "http://localhost:8080/cgi-bin/calc.py?f_num=5&oper=%2B&s_num=3"

# Division: 10 / 4
curl "http://localhost:8080/cgi-bin/calc.py?f_num=10&oper=%2F&s_num=4"

# Power: 2 ^ 8
curl "http://localhost:8080/cgi-bin/calc.py?f_num=2&oper=%5E&s_num=8"

# nth root: cube root of 27
curl "http://localhost:8080/cgi-bin/calc.py?f_num=27&oper=r&s_num=3"

# POST
curl -X POST -d "f_num=10&oper=/&s_num=4" http://localhost:8080/cgi-bin/calc.py
```

---

### `set_cookie.py` — Set Cookie

Parameters: `key`, `value`. Returns a `Set-Cookie` header (204 response).

```bash
curl -v "http://localhost:8080/cgi-bin/set_cookie.py?key=color&value=blue"
```

---

### `get_cookie.py` — Read Cookie

Parameter: `key`. Reads the cookie from the `Cookie` header.

```bash
# Set then get
curl -v "http://localhost:8080/cgi-bin/set_cookie.py?key=color&value=blue"
curl -b "color=blue" "http://localhost:8080/cgi-bin/get_cookie.py?key=color"
```

---

### `acc.py` — Session Login System

Full register/login/session flow using cookies and pickle-based storage.

**Setup** (required before first use):

```bash
mkdir -p var/www/cgi-bin/sessions
```

**Register:**

```bash
curl "http://localhost:8080/cgi-bin/acc.py?username=alice&password=secret&firstname=Alice"
```

**Login** (returns `Set-Cookie: SID=<token>`):

```bash
curl -v "http://localhost:8080/cgi-bin/acc.py?username=alice&password=secret"
```

**Access account page** (use the SID from login):

```bash
curl -b "SID=<token-from-login>" http://localhost:8080/cgi-bin/acc.py
```

Sessions expire after 2 minutes.

---

### `upload.py` — File Upload (CGI-side)

Reads a multipart form field named `filename`. Saves to `cgi-bin/tmp/`.

**Setup:**

```bash
mkdir -p var/www/cgi-bin/tmp
```

```bash
curl -X POST -F "filename=@/path/to/file.txt" http://localhost:8080/cgi-bin/upload.py
```

> Note: the `/uploads` location in `webserv.conf` handles server-side file upload without CGI. Use that for general file uploads.

---

### Shell Scripts (`.sh`)

`calendar.sh`, `env.sh`, and `image.sh` require `.sh` support enabled in `webserv.conf`:

```nginx
location /cgi-bin {
    cgi .py /usr/bin/python3;
    cgi .sh /bin/bash;    # add this line
}
```

Then:

```bash
curl http://localhost:8080/cgi-bin/calendar.sh   # calendar + date
curl http://localhost:8080/cgi-bin/env.sh        # shell env dump
```

`image.sh` streams a PNG — edit the hardcoded path inside the script before use.

---
