#!/usr/bin/perl


package ceilingLight;

use strict;
use Device::SerialPort;
use Time::HiRes qw(usleep);
use POSIX;


my $port;

sub init()
{

	$port = Device::SerialPort->new("/dev/cu.usbserial-A100DDXM");
	$port->databits(8);
	$port->baudrate(500000);
	$port->parity("none");
	$port->stopbits(1);
}


sub read()
{
	return $port->read(1);
}

sub setColor($$$$$)
{
	my $addr = shift;
	my $r = shift;
	my $g = shift;
	my $b = shift;
	my $w = shift;

	if($addr == 0)
	{
		$addr = 255;
	}
	else
	{
		$addr+=239;
	}


	my $return=$port->write(chr(0x42).esc(chr($addr).chr(int $r).chr(int $g).chr(int $b).chr(int $w)));
	usleep(1000);
}



sub esc($)
{
    my $data = shift;
    
    
	$data =~ s/\x65/\x65\x3/go;
	$data =~ s/\x23/\x65\x1/go;
	$data =~ s/\x42/\x65\x2/go;
	$data =~ s/\x66/\x65\x4/go;
                                                            
	return $data;
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
}

1;

