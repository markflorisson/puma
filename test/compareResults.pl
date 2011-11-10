#!/usr/bin/perl -w

use strict;


my @iterList = (0 .. 500);
my @animalList = ("hare","puma");

my $interval = 10;
my @arrayC = ();
my @arrayM = ();

&main();
&loadProgramOutput();


sub main
{
	foreach my $iter (@iterList)
	{
		next if(0 == ($iter % $interval));
		
		foreach my $animal (@animalList)
		{
			my $fileC = $animal."_".$iter.".C.dat";
			die "Could not find file $fileC" if(! -f $fileC);

			my $fileM = $animal."_".$iter.".M.dat";
			die "Could not find file $fileM" if(! -f $fileM);

			&loadProgramOutput($fileC,\@arrayC);
			&loadProgramOutput($fileM,\@arrayM);
		
			&compareArrays(\@arrayC,\@arrayM);
			undef(@arrayC);
			undef(@arrayM);
		}
	}
}

sub compareArrays
{
	my $arrayCRef = shift;
	my $arrayMRef = shift;

	print scalar (@$arrayCRef)."\n";
	print scalar (@$arrayMRef)."\n";

	die "Number of rows not equal" if( (scalar(@$arrayCRef)) != (scalar(@$arrayMRef)) );

	for(my $i=0;$i<(scalar(@$arrayCRef));$i++)
	{
		my $columnsC = $arrayCRef->[$i];
		my $columnsM = $arrayMRef->[$i];

		die "Number of columns not equal" if( scalar(@$columnsC) != scalar(@$columnsM) );

		for(my $j=0;$j<scalar(@$columnsC);$j++)
		{
			if($columnsC->[$j] != $columnsM->[$j])
			{
				print "Mismatch in values\n";
			}
		}	
	}

}


sub loadProgramOutput
{
	my $file = shift;
	my $arr = shift;
	my $i = 0;

	printf "Processing file $file\n";

	open(FILE,"<$file") or die "Could not open $file";
	while(<FILE>)
	{
		chomp;
		my @tmpArr = split(/ /,$_);
		@$arr[$i++] = \@tmpArr;
	}
	close FILE;
}
