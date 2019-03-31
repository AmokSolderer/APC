#!/usr/bin/perl

$buf1 = 0;
$buf2 = 0;

open(FH, "< Data.wav");
binmode(FH);
open(FOUT, "> Data.BIN");
binmode(FOUT);

for (my $i = 0; $i < 44; $i++) {
    read(FH, $buf1, 1);}                    # skip the Header

until (eof(FH)) {
    read(FH, $buf1, 1);                     # the read uses chars
    read(FH, $buf2, 1);                     # get two two have a 16 bit value
    #read(FH, $buf1, 1);                     # uncomment this line and the next when using 
    #read(FH, $buf2, 1);                     # stereo input files to skip one channel
    $buf1 = unpack("C", $buf1);             # convert to numbers
    $buf2 = unpack("C", $buf2);
    $buf = ($buf2 << 8) | $buf1; 						# and to 16 bit
    $buf = $buf >> 4;                       # reduce to 12 bit
    if ($buf < 2048) {											# move the zero to 2048 
        $buf = $buf + 2048;}
    else {
        $buf = $buf - 2048;}
    syswrite (FOUT, pack("v", $buf), 2);}   # and fumble it into the output file

close(FH);
close(FOUT);

