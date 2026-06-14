#!/usr/bin/perl
use strict;
use warnings;

# Intentional crash — tests CGI error handling for .pl interpreter.
# die() before any output — server must return 500.
die "intentional crash — CGI error handling test\n";
