#!/usr/bin/perl -w

# Name          : Kevin Zink
# Class         : CSCI 2850-001
# Program #     : 15
# Due Date      : November 26th, 2003
# Honor Pledge  : On my honor as a student at the University
#                 of Nebraska at Omaha, I have neither given nor received
#                 unauthorized help on this homework assignment.
#
# Signed: Kevin Zink, xxx-xx-3541, kzink@mail.unomaha.edu
#
# Collaborators: None
#
# This cgi accepts information from program15a.cgi and processes the information.

use strict;
use CGI;

my $page = new CGI;

my ($login, $name) = ($page->param('logins') =~ /([^ ]*)\s\((.*)\)/);
my $month = $page->param('radiobutton');
my $count = 0;
my $minutes = 0;
my $hours = 0;
my @lastoutput;
my @output;

if ($month eq "current")
{
    @lastoutput = `last $login`; #captures output from the 'last' command
}
else
{
    @lastoutput = `last -f /var/log/wtmp.1 $login`;
}
my @lastoutput = grep(/$login\s/, @lastoutput);

print $page->header() . $page->start_html(-title=>'User Info', -bgcolor=>'white', -text=>'black');

print $page->b("Here is a listing of logins for $login ($name) for the $month month:");
print $page->p();

# this loop calculates the total time for each user
foreach (@lastoutput) 
{
    # in the case that the line does not contain a valid time
    if (/still logged in/ || /crash/ || /down/) 
    {
        print $page->pre($_);
    }
    else 
    {
        my $days = 0;
        $count++;
        print $page->pre($_);
        my @pieces = split(/\(/, ); 
        my ($d, $h, $m) = ($pieces[1] =~ /(\d*)\+?(\d\d):(\d\d)/); 
        
        $days += $d if $d;
        $hours += $h + $days*24; #this converts days to hours
        $minutes += $m;
        if ($minutes >= 60) 
        {
            $minutes -= 60; 
            $hours++;
        }
    } 
}

#final output statements
print $page->h2("Summary");
print $page->b("$login ($name) logged in $count times for a total of $hours hours and $minutes minutes.") . $page->p();

print $page->a( {-href=>"program15a.cgi"}, "Search Again!");

print $page->end_html();
