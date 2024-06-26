#!/usr/bin/perl

$buf1 = 0;
$buf2 = 0;
$i = 0;
$size = 0;
@data = ('d','a','t','a');                  # 'data' is indicating the beginning of the data chunk

open(FH, "< Data.wav");
binmode(FH);
open(FOUT, "> Data.snd");
binmode(FOUT);

do {                                        # search for the 'data' keyword
    read(FH, $buf1, 1);                     # read charakter
    if ($buf1 eq $data[$i]) {               # does it belong to the keyword?
        $i++;}                              # increase hit counter
    else {
        $i = 0;}                            # reset hit counter
    } until ($i > 3);                       # all four characters found?
read(FH, $buf1, 1);                         # read LSByte of the data chunk size
$size = ord($buf1);
read(FH, $buf1, 1);
$size = $size | (ord($buf1) << 8);
read(FH, $buf1, 1);
$size = $size | (ord($buf1) << 16);
read(FH, $buf1, 1);                         # read MSByte of the data chunk size
$size = $size | (ord($buf1) << 24);
$i = 0;
do {                                        # read all bytes of the data chunk
    read(FH, $buf1, 1);                     # the read uses chars
    $i++;
    read(FH, $buf2, 1);                     # get two two have a 16 bit value
    $i++;
    #read(FH, $buf1, 1);                     # uncomment these four lines when using 
    #$i++;
    #read(FH, $buf2, 1);                     # stereo input files to skip one channel
    #$i++;
    $buf1 = ord($buf1);                     # convert to numbers
    $buf2 = ord($buf2);
    $buf = ($buf2 << 8) | $buf1; 						# and to 16 bit
    $buf = $buf >> 4;                       # reduce to 12 bit
    if ($buf < 2048) {											# move the zero to 2048 
        $buf = $buf + 2048;}
    else {
        $buf = $buf - 2048;}
    syswrite (FOUT, pack("v", $buf), 2);    # and fumble it into the output file
    } while ($i < $size);                   # all byte of the data chunk read?
close(FH);
close(FOUT);

