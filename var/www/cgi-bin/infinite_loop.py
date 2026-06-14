#!/usr/bin/env python3
import sys
import os

# Intentional infinite loop — tests CGI timeout handling.
# Server kills this after CGI_TIMEOUT_SECS (10s) and returns 504.
while True:
    pass
