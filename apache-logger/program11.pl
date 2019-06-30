#!/usr/bin/perl -w

# Name          : Kevin Zink
# Class         : CSCI 2850-001
# Program #     : 11
# Due Date      : October 23th, 2003
# Honor Pledge  : On my honor as a student at the University
#                 of Nebraska at Omaha, I have neither given nor received
#                 unauthorized help on this homework assignment.
#
# Signed: Kevin Zink, xxx-xx-3541, kzink@mail.unomaha.edu
#
# Collaborators: None
#
# This program parses an apache web server log file and analyzes the data.

use strict;
use Net::hostent;

my %hostname;
my %domainname;
my %urllist;
my %browser;
my %dates;
my %hours;
my %refererlist;
my %statuscodelist;
my $totalbytes = 0;
my $domain;
my $counter = 0;

if (!$ARGV[0])
{
    print "Usage is as follows: $0 <logfile> <logfile> ...\n";
    exit(1);
}

foreach(@ARGV)
{
    my $filename = $_;
    open (IN, $filename);

    while (my $line = <IN>)
    {
        $counter++;
        my ($ipaddy, $date, $time, $url, $statuscode, $bytes, $referer, $useragent) =
            ($line =~ /([^ ]*) - - \[([^:]*):(\d*):[^"]*"[^ ]* ([^ ]*)[^"]*" (.*) (.*) "([^"]*)" "([^"]*)"/);
        
        $bytes = 0 if $bytes eq "-";
        
        my $dtest = gethost($ipaddy);
        if ($dtest)
        {
            $domain = $dtest->name();
            $hostname{$domain} ++;
            ($domain) = ($domain =~ /.*\.(.*\..*)/);
            $domain = lc ($domain);
            $domain = "UNKNOWN (dotted-quad or other)" if $domain eq "";
        }
        else
        {
            $hostname{$ipaddy} ++;
            $domain = "UNKNOWN (dotted-quad or other)";
        }
        
        if ($referer =~ /http/)
        {
            $refererlist{$referer} ++;
        }
        else 
        {
            $refererlist{"UNKNOWN"} ++;
        }

        $domainname{$domain} ++;
        $urllist{$url} ++;
        $browser{$useragent} ++;
        $dates{$date} ++;
        $hours{$time} ++;
        $statuscodelist{$statuscode} ++;
        $totalbytes += $bytes;
    }
}

close IN;

open (OUT, ">Calvino.analysis");

print OUT "Processed $counter lines from the following files:\n";
foreach (@ARGV)
{
    print OUT "$_ ";
}
print OUT "\n";

printer_header("HOSTNAMES", \*OUT);
printer(\%hostname, \*OUT);
printer_header("DOMAINS", \*OUT);
printer(\%domainname, \*OUT);
printer_header("URLS", \*OUT);
printer(\%urllist, \*OUT);
printer_header("BROWSERS", \*OUT);
printer(\%browser, \*OUT);
printer_header("DATES", \*OUT);
printer(\%dates, \*OUT);
printer_header("HOURS", \*OUT);
printer(\%hours, \*OUT);
printer_header("REFERERS", \*OUT);
printer(\%refererlist, \*OUT);
printer_header("STATUS CODES", \*OUT);
printer(\%statuscodelist, \*OUT);
print OUT "\nTotal number of bytes served: $totalbytes\n\n";

close OUT;

sub printer_header
{
    my $title = shift;
    my $filehandle = shift;
    print $filehandle ("\n==============================================================================\n");
    print $filehandle ("$title\n");
    print $filehandle ("==============================================================================\n\n");
}

sub printer 
{
    my $hash = shift;
    my $filehandle = shift;
    my $total = 0.00;
    
    my @keyWords = sort {$a cmp $b} keys %$hash;

    foreach(@keyWords)
    {
        $total += $$hash{$_};
    }
    
    printf $filehandle ("%6s   %5s    %s", "Hits", "%-age", "Resource\n");
    printf $filehandle ("%6s   %5s    %s", "----", "-----", "--------\n");
    
    foreach(@keyWords)
    {
        printf $filehandle ("%6s   %5.2f   %s\n", $$hash{$_}, $$hash{$_} / $total * 100.00, $_);
    }

    print $filehandle " -----\n";
    printf $filehandle ("%6s entries displayed\n", $total);
}
