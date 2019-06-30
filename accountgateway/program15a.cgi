#!/usr/bin/perl -w

# Name          : Kevin Zink
# Class         : CSCI 2850-001
# Program #     : 15
# Due Date      : November 16th, 2003
# Honor Pledge  : On my honor as a student at the University
#                 of Nebraska at Omaha, I have neither given nor received
#                 unauthorized help on this homework assignment.
#
# Signed: Kevin Zink, xxx-xx-3541, kzink@mail.unomaha.edu
#
# Collaborators: None
#
# This CGI script runs an account information gateway.

use strict;
use CGI;

my $inputfile = "/etc/passwd";
open (IN, $inputfile);
my @wholefile = <IN>;
my @loginlist;
close IN;

my $counter = 0;
foreach(@wholefile)
{
    my $userid = &get_user_id($_);
    my $fullname = &get_full_name($_);
    $loginlist[$counter] = "$userid ($fullname)";
    $counter++;
}
@loginlist = sort {$a cmp $b} @loginlist; 

my $page = new CGI;

print $page->header() . $page->start_html(-title=>'Login Information', -text=>'black',  
        -bgcolor=>'white');

print $page->startform("POST", "program15b.cgi");

print $page->h1('Account Summary Gateway');

print $page->h3('Select the name you would like to get a login summation for:');

print $page->scrolling_list(-name=>'logins',
                            -values=>\@loginlist,
                            -size=>15) . $page->p();
print $page->h4('Which month?');

my @rbuttons = qw (current previous);
print $page->radio_group(-name=>'radiobutton',
                         -values=>\@rbuttons,
                         -default=>$rbuttons[0],
                         -columns=> 2) . $page->p();

print $page->submit(-name=>'submitbutton',
                    -value=>'Go!') . $page->p();

print $page->endform();


sub get_user_id
{
    my @tokens = split(/:/, shift);
    return $tokens[0];
}

sub get_full_name
{
    my $rval = "";
    my @tokens = split(/:/, shift);
    my ($first, $middle, $last) = ($tokens[4] =~ /(\D+\s)?(\D+\s)?([A-Za-z]+)/);
    if ($first)
    {
        $rval .= $first;
    }
    if ($middle)
    {
        $rval .= $middle;
    }
    if ($last)
    {
        $rval .= $last;
    }
    return $rval;
}

