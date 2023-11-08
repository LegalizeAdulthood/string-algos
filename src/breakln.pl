#!/usr/bin/perl
#
# breakln [-i n] [-w n] [-m] [files...]
#
#   fold text lines, breaking at whitespace, and inserting indentation to
#   indicate folded lines.  This is useful for making the output of
#   find used with xargs and Makefiles more readable.
#
# OPTIONS
# -i n
#   Set the indentation to be the specified number.
# -w n
#   Set the width of the output lines.
# -m
#   Crush multiple white space on lines.
# -b
#   Eliminate blank lines from the input.
#
# November 4th, 1993
#

local($indent) = 4;
local($width) = 75;
local($crush) = 0;
local($blanks) = 0;

sub usage {
    local($me) = `basename $0`;
    print <<"_Usage_";
Usage:

$me [-i n] [-w n] [-m] [-b] [files...]
    break (insert a carraige return) long lines at whitespace, and indent 

Options:
    -i n    Indent n columns on lines (default $indent)
    -w n    Break output lines at column n (default $width)
    -m      Crush multiple whitespace
    -b      Eliminate blank lines
_Usage_
    exit -1
}

sub breaklines {
  local($indent) = ' ' x $indent;
  local($_, $pad, $space);

  while (<ARGV>) {
    chop;

    # purge blank lines if -b
    unless (length($_)) {
      print "\n" unless $blanks;
      next;
    }

    # crush whitespace if -m
    s/\s+$//;
    if ($crush) {
      s/^\s+//;
      s/(\S)\s+(\S)/$1 $2/g;
    }

    $pad = 0;
    while ($pad + length($_) > $width) {
      # first try and split on whitespace
      if (($space = rindex($_, ' ', $width - $pad)) > 0) {
        print substr($_, 0, $space), "\n", $indent;
        $pad = length($indent);
        substr($_, 0, $space+1) = '';
      } elsif (($space = rindex($_, "\t", $width - $pad)) > 0) {
        # next try breaking on a tab
        print substr($_, 0, $space), "\n", $indent;
        $pad = length($indent);
        substr($_, 0, $space+1) = '';
      } else {
        # next try to split on non-alphabetic
        my $line = substr($_, 0, $width - $pad);
        if ($line =~ /(.*)(\W)(\w*)/) {
          my $printed = $1 . $2;
          print $printed, "\n", $indent;
          $pad = length($indent);
          substr($_, 0, length($printed)) = '';
        } else {
          # can't do anything but be as stupid as fold
          print substr($_, 0, $width-$pad), "\n", $indent;
          $pad = length($indent);
          substr($_, 0, $width-$pad) = '';
        }
      }
    }
    print $_, "\n" if length($_);
  }
}

sub main {
  local($_, @files);

  # first, parse arguments
  while (scalar(@ARGV)) {
    $_ = shift @ARGV;
    /^[^-]/ && (push(@files, $_), next);
    /^-i/ && ($indent = shift @ARGV, next);
    /^-w/ && ($width = shift @ARGV, next);
    /^-m/ && ($crush = 1, next);
    /^-b/ && ($blanks = 1, next);
    &usage;
  }

  select(STDOUT); $| = 1;

  @ARGV = @files;
  &breaklines;
}

&main;

