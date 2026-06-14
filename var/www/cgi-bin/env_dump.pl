#!/usr/bin/perl
use strict;
use warnings;

my @cgi_vars = qw(
    REQUEST_METHOD QUERY_STRING CONTENT_TYPE CONTENT_LENGTH
    SCRIPT_NAME PATH_INFO SERVER_NAME SERVER_PORT
    SERVER_PROTOCOL GATEWAY_INTERFACE REDIRECT_STATUS
);

my $rows = '';
for my $key (@cgi_vars) {
    my $val = defined $ENV{$key} ? $ENV{$key} : '&lt;not set&gt;';
    $rows .= "<tr><td>$key</td><td>$val</td></tr>\n";
}

for my $key (sort keys %ENV) {
    next unless $key =~ /^HTTP_/;
    $rows .= "<tr><td>$key</td><td>$ENV{$key}</td></tr>\n";
}

my $body = <<HTML;
<html><head><style>
body{font-family:monospace;background:#0a0b0e;color:#c8cdd8;padding:20px}
table{border-collapse:collapse;width:100%}
td{border:1px solid #2a2d38;padding:6px 12px}
td:first-child{color:#39ff14;width:220px}
h2{color:#f0f2f7;margin-bottom:16px}
</style></head><body>
<h2>CGI Environment Variables (Perl)</h2>
<table>$rows</table>
</body></html>
HTML

print "Content-Type: text/html\r\n";
print "Content-Length: " . length($body) . "\r\n";
print "\r\n";
print $body;
