#!/usr/bin/perl

$PatX = 9;										# X dimension of segment pattern
$PatY = 9;										# Y dimension of segment pattern
@Pattern = qw(									
	.aaaaaaa.
	f.......b
	f.......b
	f.......b
	.ggggggg.
	e.......c
	e.......c
	e.......c
	.ddddddd.);											# segment pattern
$Segments = 7 ;										# number of segments present in this display type
#@PinDefine = ('d','c','b','a','e','f','g','.','j','h','m','k','p','r','.','n'); # segments to APC pins (upper line Alphanumeric)
#@PinDefine = ('.','g','f','b','a','d','e','c','j','m','n','.','r','p','k','h'); # segments to APC pins (lower line Alphanumeric)
@PinDefine = ('.','g','f','b','a','d','e','c');# segments to APC pins (lower line Big Guns)
#@Segment = ('a','b','c','d','e','f','g','h','j','k','m','n','p','r'); # list of segments present in this display type (upper line Big Guns)
@Segment = ('a','b','c','d','e','f','g'); # list of segments present in this display type (lower line Big Guns)
@Present = (0,0,0,0,0,0,0,0,0,0,0,0,0,0);				# stores which segments are active

do {
	$Puffer = <STDIN>;									# read keyboard
	chomp($Puffer);										# remove linefeed
	for ($i=0; $i<$Segments; $i++) {					# for all segments
			if ($Puffer eq $Segment[$i]) {				# check if segment is chosen
				$Present[$i] = !$Present[$i];}}			# if yes negate its state
	for (my $y=0; $y<$PatY; $y++) {						# for all lines of the pattern
		print("     ");									# start with a few blanks
		for (my $x=0; $x<$PatX; $x++) {					# for all columns of the pattern
			$Seg = substr($Pattern[$y], $x, 1);			# get the selected pattern position
			if ($Seg eq '.') {							# does it belong to a segment?
				print(" ");}							# if no print a blank
			else {										# if yes
				for ($i=0; $i<$Segments; $i++) {		# cycle through all segments
					if ($Seg eq $Segment[$i]) {			# if its the right one
						if ($Present[$i]) {				# check if segments is enabled
							print("*");}				# if yes print a star
						else {
							print(" ");}}}}}			# if not print a blank
		print("\n");}									# print a linefeed at the end of each line
	print("\n");
	$Result1 = 0;										# First result byte
	$Result2 = 0;										# Second result byte
	for (my $x=0; $x<$Segments; $x++) {					# for all segments
		if ($Present[$x]) {								# segment active?
			$Mask = 1;									# set bit mask
			$i = 0;										# reset counter
			until ($Segment[$x] eq $PinDefine[$i]) { 	# search for segment in PinDefine
				$i++;									# returns the position of the segment in PinDefine
				$Mask = $Mask << 1;}					# and the bit mask shifted to the corresponding position
			if ($i > 7) {								# position in second result byte?
				$Result2 = $Result2 | ($Mask >> 8);} 	# set the bit for the pin
			else {										# position in first result byte
				$Result1 = $Result1 | $Mask;}}}			# set the bit for the pin
	print("\n","\n",$Result1,", ",$Result2,"\n");}
while($Puffer ne 'q');									# leave the program on q

