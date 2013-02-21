#!/usr/bin/perl

package ceilingLight;

use strict;
use IO::Socket;
use Time::HiRes qw(usleep);
use IO::Socket::INET6;

my $socket;
my $window = 3;

sub init()
{
	$socket = IO::Socket::INET6->new(PeerAddr => 'bender',
									PeerPort => 1340,
									Proto    => "tcp",
									Type     => SOCK_STREAM)     or die "Couldn't connect : $@\n";
}

sub record()
{
	print $socket "05\r\n";
}


sub setColor($$$$$)
{
	my $x = shift;
	my $red = shift;
	my $green = shift;
	my $blue = shift;
	my $white = shift;

	$x = $x+0xf0;




	print $socket '02'.sprintf("%02x",$x).sprintf("%2x",$red).sprintf("%2x",$green).sprintf("%2x",$blue).sprintf("%2x",$white)."\r\n";
}


sub setLevel($)
{
	my $level=shift;

	print $socket '04'.sprintf("%02x", $level);
#	print $socket2 '04'.sprintf("%02x", $level);
		
	print $socket "\r\n";
#	print $socket2 "\r\n";

#	warn <$socket>;
}

sub readline()
{
#	my $liney = <$socket2>;
	my $line = <$socket>;
	return $line;
}

sub hsv2rgb {
    my ($h, $s, $v) = @_;
    my ($i, $f, $p, $q, $t);
    my ($r ,$b, $g);
	if ($s == 0) {
	$r = $b = $g = $v;
    } else {
	$h /= 60;
	$i = int $h;
	$f = $h - $i;
	$p = $v * (1 - $s);
	$q = $v * (1 - $s * $f);
	$t = $v * (1 - $s * (1 - $f));
      SWITCH: {
	  if ($i == 0) {
	      $r = $v;
	      $g = $t;
	      $b = $p;
	      last SWITCH;
	  }
	  if ($i == 1) {
	      $r = $q;
	      $g = $v;
	      $b = $p;
	      last SWITCH;
	  }
	  if ($i == 2) {
	      $r = $p;
	      $g = $v;
	      $b = $t;
	      last SWITCH;
	  }
	  if ($i == 3) {
	      $r = $p;
	      $g = $q;
	      $b = $v;
	      last SWITCH;
	  }
	  if ($i == 4) {
	      $r = $t;
	      $g = $p;
	      $b = $v;
	      last SWITCH;
	  }
	  $r = $v;
	  $g = $p;
	  $b = $q;
      } 
    }
    return ($r, $g, $b);
};


sub read()
{
}
1;


