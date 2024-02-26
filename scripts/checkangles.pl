#!/usr/bin/perl
use List::Util qw[min];

$pdb = $ARGV[0];
@a = split("\n",`/opt/mmtsb/perl/dihed.pl -list phi,psi $pdb`);
$offset = int($ARGV[1]);
#for ($q = 0; $q < $offset; $q++) {
#	shift(@a);
#}

$file = 1 + $offset;
for $q (@a) {
	if (!(-f $file.'.angles')) {
		#print "Couldn't find file $file\n"; last;
		next;
	}
	($res, $phi, $psi) = split(/\s+/, $q);
	$rphi = round($phi /10)*10;
	$rpsi = round($psi /10)*10;
	$gphi = join('\-', split('-', $rphi));
	$gpsi = join('\-', split('-', $rpsi));
	$found = `grep "$gphi $gpsi" $file.angles`;
	chomp($found);

	if ($found eq '') {
		@d =split("\n",`cat $file.angles | awk '{print sqrt((\$1 - $phi)^2 + (\$2 - $psi)^2)}'`);
		$min = min(@d);
		next if ($min < 14.15);
		#if ($min > 180) { $min = 360 - $min; }
		#if ($min <-180) { $min = $min + 360; }
		print "Could not find $phi $psi for $res \t  ($rphi $rpsi) in $file \t";
		print $min."\n";
	} else {
		#print "$file OK\n";
	}
	#$file++;
} continue {
	$file++;
}
print "Done.\n";

sub round {
    my($number) = shift;
    return int($number + (0.5 * ($number <=> 0)));
}
