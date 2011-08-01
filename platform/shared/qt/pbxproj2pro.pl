#!/usr/bin/perl
while($line=<STDIN>) {
 if ($line =~ /\;\s*path\s*=\s*([^\;]+)\;/) {
  $filepath = $1;
  if ($filepath =~ /\.(h|hpp)$/) {
   $type = 0;
  } elsif ($filepath =~ /\.(c|cpp)$/) {
   $type = 1;
  } else {
   $type = 2;
  }
  push @{$FILES[$type]}, $filepath;
 }
}

for $type (0..$#FILES) {
 for $filepath (@{$FILES[$type]}) {
  print "$filepath\\\n";
 }
 print "\n";
}
