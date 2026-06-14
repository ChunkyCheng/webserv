#!/usr/bin/perl
use strict;
use warnings;

my $method  = $ENV{REQUEST_METHOD} // '';
my $query   = $ENV{QUERY_STRING}   // '';
my $ctype   = $ENV{CONTENT_TYPE}   // '';
my $clen    = $ENV{CONTENT_LENGTH} // 0;

my $body = '';
if ($clen > 0) {
    read(STDIN, $body, $clen);
}

my $html = "<html><body>";
$html .= "<h1>CGI Perl works</h1>";
$html .= "<p>Method: $method</p>";
$html .= "<p>Query: $query</p>";
$html .= "<p>Content-Type: $ctype</p>";
$html .= "<p>Body: $body</p>";
$html .= "</body></html>";

print "Content-Type: text/html\r\n";
print "Content-Length: " . length($html) . "\r\n";
print "\r\n";
print $html;
