#!/usr/bin/perl
use strict;
use warnings;
use Cwd;

my $cwd     = cwd();
my $status  = '';
my $color   = '';
my $content = '';

if (open(my $fh, '<', 'data.txt')) {
    local $/;
    $content = <$fh>;
    close($fh);
    $status = 'SUCCESS';
    $color  = '#39ff14';
} else {
    $status = "FAILED: $!";
    $color  = '#ff3300';
    $content = 'Could not open data.txt — chdir may not have run';
}

my $file_block = "<pre style=\"background:#111318;padding:12px;white-space:pre-wrap\">$content</pre>";

my $body = <<HTML;
<html><head><style>
body{font-family:monospace;background:#0a0b0e;color:#c8cdd8;padding:20px}
h2{color:#f0f2f7;margin-bottom:12px}
.label{color:#555a6b;font-size:12px}
</style></head><body>
<h2>Relative Path Test — Perl</h2>
<p class="label">Working directory: <span style="color:#00e5ff">$cwd</span></p>
<p class="label">open("data.txt") result: <span style="color:$color">$status</span></p>
<br>$file_block
</body></html>
HTML

print "Content-Type: text/html\r\n";
print "Content-Length: " . length($body) . "\r\n";
print "\r\n";
print $body;
