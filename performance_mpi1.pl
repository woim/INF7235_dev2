#!/usr/bin/perl
# use strict;
# use warnings;

my @seq_result = ();
my $nRun = 5;
my $sum = 0;

for my $i (1 .. $nRun)
{
  my $result =`./resampling_sequential parrot.ppm`;
  $sum = $sum + $result;
}
my $seq_time = $sum/$nRun;


my @Np = (1,2,4,8,16,32,64,128);
my $par_time = 0;
my $acc = 0;
my $count = 0;
foreach (@Np)
{
  $sum = 0;
  $count = 0;
  for my $i (1 .. $nRun)
  {
    my @result =`mpirun -np $_ resampling_parallel parrot.ppm`;
    foreach (@result)
    {
      $sum = $sum + $_;
      $count = $count + 1;
    }
  } 
  $par_time = $sum/$count;
  $acc = $seq_time/$par_time;
  print "NP: $_ $par_time $acc\n";
}
`rm -rf *.bmp`;
`rm -f *~`;
`rm -rf *.o`;
`rm resampling_sequential resampling_parallel`;
