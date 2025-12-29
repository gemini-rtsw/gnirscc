#!/usr/bin/perl -wT

my @stdkwd;
open (FILE,'<',"genericHeaders.txt");
while (<FILE>) {
   chomp;
   push @stdkwd , $_;
   }
close FILE;

open (FILE,'<',"/home/mbec/michelle.fits.header.proc");
while (<FILE>) {
   chomp;
   my %hash;
   my @names = qw (key val com);
   my $regexp = "(\\w+)\\s*=\\s*('.+'|[^/|^\\s]+)\\s*/\\s*(.*)\\s*";
   @hash{@names} = $_ =~ $regexp;
   
   foreach my $stdk ( @stdkwd ) {
      if ( $hash{'key'} eq $stdk ) {
         goto END;
         }
      }
   
#    if ( $hash{'val'} =~ "'(.+)'" ) {
#       printf "hsender -dict add %-10s                ;# %s\n", $hash{'key'},$hash{'com'};
#       }
#    elsif ($hash{'val'} =~ "^-?\\d+\$"  ) {
#       printf "hsender -dict add %-10s -type INT      ;# %s\n", $hash{'key'},$hash{'com'};
#       }
#    elsif ($hash{'val'} =~ "^([+-]?)(?=\\d|\.\\d)\\d*(\\.\\d*)?([Ee]([+-]?\\d+))?\$"  ) {
#       printf "hsender -dict add %-10s -type DOUBLE   ;# %s\n", $hash{'key'},$hash{'com'};
#       }
#    elsif ($hash{'val'} =~ "^(F|T)\$"  ) {
#       printf "hsender -dict add %-10s -type BOOLEAN  ;# %s\n", $hash{'key'},$hash{'com'};
#       }
#    else {
#       printf "?? %-10s %-20s %s\n", $hash{'key'}, $hash{'val'}, $hash{'com'};
#       }


   if ( $hash{'val'} =~ "'(.+)'" ) {
      printf "OK michelle %-10s STRING  %-10s F  NULL           NONE    NULL    \"%s\"\n", $hash{'key'},$hash{'key'},$hash{'com'};
      }
   elsif ($hash{'val'} =~ "^-?\\d+\$"  ) {
      printf "OK michelle %-10s INT     %-10s F  NULL           NONE    NULL    \"%s\"\n", $hash{'key'},$hash{'key'},$hash{'com'};
      }
   elsif ($hash{'val'} =~ "^([+-]?)(?=\\d|\.\\d)\\d*(\\.\\d*)?([Ee]([+-]?\\d+))?\$"  ) {
      printf "OK michelle %-10s FLOAT   %-10s F  NULL           NONE    NULL    \"%s\"\n", $hash{'key'},$hash{'key'},$hash{'com'};
      }
   elsif ($hash{'val'} =~ "^(F|T)\$"  ) {
      printf "OK michelle %-10s BOOLEAN %-10s F  NULL           NONE    NULL    \"%s\"\n", $hash{'key'},$hash{'key'},$hash{'com'};
      }
   else {
      printf "?? %-10s %-20s %s\n", $hash{'key'}, $hash{'val'}, $hash{'com'};
      }

END:

   }
close FILE;



