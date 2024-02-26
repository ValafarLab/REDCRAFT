#!/usr/bin/perl
package List::Util;

use List::Util qw(max min);

$argSize = @ARGV;

if($argSize < 2)
{
	print "Usage: $0 <INPUT> <OUTPUT> [1 if ivert]\n";
	exit;

}

$inputFile = $ARGV[0];
$outputFile = $ARGV[1];
if ($ARGV[2] ne '' && $ARGV[2] == 1) {
	$INVERT = 1;
} else {
	$INVERT = 0;
}


open(fileHandle,"./$inputFile");
print "InputFile: $inputFile \n";

@C_N;
@N_H;
@C_H;
@CA_HA;
@HA_H;
@HAP_H;
@residualNames;

while(<fileHandle>)
{
	chomp;	

	@token = split(" ",$_);

	push(@residualNames,$token[0]);


	$_ = <fileHandle>;

	chomp;

	@token = split(" ",$_);


	if($token[0] != 999)
	{
		push(@C_N,$token[0]);
	}

	$_ = <fileHandle>;	
	chomp;	
	@token = split(" ",$_);


	if($token[0] != "999")
	{
		push(@N_H, $token[0]);
	}

	$_ = <fileHandle>;
	chomp;
	@token = split(" ",$_);


	if($token[0] != "999")
	{
		push(@C_H,$token[0]);
	}

	$_ = <fileHandle>;	

	chomp;

	@token = split(" ",$_);


	if($token[0] != "999")
	{
		push(@CA_HA,$token[0]);
	} 

	$_ = <fileHandle>;

	chomp;

	@token = split(" ",$_); 

	if($token[0] != "999")
	{
		push(@HA_H,$token[0]);
	}

	$_ = <fileHandle>;

	chomp;

	@token = split(" ",$_);



	if($token[0] != "999")
	{
		push(@HAP_H,$token[0]);
	}

}





$C_NError;
$N_HError = 1;
$CA_HAError;
$C_HError;
$HA_HError;
$HAP_HError;

$rangeOfN_H = (max(@N_H)-min(@N_H));
$rangeOfC_N = (max(@C_N)-min(@C_N));
$rangeOfCA_HA = (max(@CA_HA)-min(@CA_HA));
$rangeOfC_H = (max(@C_H)-min(@C_H));
$rangeOfHA_H = (max(@HA_H)-min(@HA_H));
$rangeOfHAP_H = (max(@HAP_H)-min(@HAP_H));


if ($INVERT == 0){
	$C_NError   = $rangeOfC_N / $rangeOfN_H;
	$CA_HAError = $rangeOfCA_HA / $rangeOfN_H;
	$C_HError   = $rangeOfC_H / $rangeOfN_H;
	$HA_HError  = $rangeOfHA_H / $rangeOfN_H;
	$HAP_HError = $rangeOfHAP_H / $rangeOfN_H;
} else {
	#$rangeOfC_N = 1 if ($rangeOfC_N == 0);
	#$rangeOfCA_HA = 1 if ($rangeOfCA_HA == 0);
	#$rangeOfC_H = 1 if ($rangeOfC_H == 0);
	#$rangeOfHA_H = 1 if ($rangeOfHA_H == 0);
	#$rangeOfHAP_H = 1 if ($rangeOfHAP_H == 0);

	$C_NError =   $rangeOfC_N == 0 ? 1 : $rangeOfN_H / $rangeOfC_N;
	$CA_HAError = $rangeOfCA_HA == 0 ? 1 : $rangeOfN_H / $rangeOfCA_HA;
	$C_HError =   $rangeOfC_H == 0 ? 1 : $rangeOfN_H / $rangeOfC_H;
	$HA_HError =  $rangeOfHA_H == 0 ? 1 : $rangeOfN_H / $rangeOfHA_H;
	$HAP_HError = $rangeOfHAP_H == 0 ? 1 : $rangeOfN_H / $rangeOfHAP_H;
}

print "C_NError  : $C_NError \n";
print "N_HError  : $N_HError \n";
print "C_HError  : $C_HError \n";
print "CA_HAErr  : $CA_HAError \n";
print "HA_HError : $HA_HError \n";
print "HAP_HError: $HAP_HError \n";


#now to create our output


open(fileHandle1 , "$inputFile");
open(outputHandle, ">wtemp");


while(<fileHandle1>)
{

	chomp;

	$line = $_;

	print outputHandle "$line\n";	

	$_ = <fileHandle1>;

	chomp;
	$line = $_;
	@token = split(" ",$line);

	printf outputHandle ("$token[0] %.2f \n", $C_NError);


	$_ = <fileHandle1>;	
	chomp;
	$line = $_;
	@token=split(" ",$line);

	printf outputHandle ("$token[0] %.2f \n", $N_HError);



	$_ = <fileHandle1>;	
	chomp;
	$line = $_;
	@token=split(" ",$line);

	printf outputHandle ("$token[0] %.2f \n", $C_HError);



	$_ = <fileHandle1>;	
	chomp;
	$line = $_;
	@token=split(" ",$line);

	printf outputHandle ("$token[0] %.2f \n", $CA_HAError);


	$_ = <fileHandle1>;	
	chomp;
	$line = $_;
	@token=split(" ",$line);

	printf outputHandle ("$token[0] %.2f \n", $HA_HError);


	$_ = <fileHandle1>;	
	chomp;
	$line = $_;
	@token=split(" ",$line);

	printf outputHandle ("$token[0] %.2f \n", $HAP_HError);

}

`mv wtemp $outputFile`;
