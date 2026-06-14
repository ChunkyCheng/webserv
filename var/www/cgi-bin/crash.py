#!/usr/bin/env python3
import sys

# Intentional crash — tests CGI error handling.
# Process exits with non-zero before writing any output.
# Server must return 500 Internal Server Error.
raise RuntimeError("intentional crash — CGI error handling test")
