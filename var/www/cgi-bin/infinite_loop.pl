#!/usr/bin/perl
use strict;
use warnings;

# Intentional infinite loop — tests CGI timeout for .pl interpreter.
# Server kills after CGI_TIMEOUT_SECS (10s) and returns 504.
while (1) {}
