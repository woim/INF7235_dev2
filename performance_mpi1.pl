#!/usr/bin/perl
# use strict;
# use warnings;

my @seq_result = ();
my $nRun = 5;
my @images = ("parrot_large.ppm","parrot_medium.ppm","parrot_small.ppm");
my @Np = (1,2,4,8,16,32,64,128);

foreach my $image (@images)
{
  my $sum = 0;
  for my $i (1 .. $nRun)
  {
    my $result =`./resampling_sequential $image`;
    $sum = $sum + $result;
  }
  my $seq_time = $sum/$nRun;

  my $par_time = 0;
  my $acc = 0;
  my $count = 0;
  my $eff = 0;
  foreach (@Np)
  {
    $sum = 0;
    $count = 0;
    for my $i (1 .. $nRun)
    {
      my @result =`mpirun -np $_ resampling_parallel $image`;
      foreach (@result)
      {
        $sum = $sum + $_;
        $count = $count + 1;
      }
    } 
    $par_time = $sum/$count;
    $acc = $seq_time/$par_time;
    $eff = $acc/$_;
    print "$image NP: $_ $seq_time $par_time $acc $eff\n";
  }
}
`rm -rf *.bmp`;
`rm -f *~`;
`rm -rf *.o`;
`rm resampling_sequential resampling_parallel`;
