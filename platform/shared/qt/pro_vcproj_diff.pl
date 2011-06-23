#!/usr/bin/perl

open(PRO,$ARGV[0]);
while($line=<PRO>) {
 $line =~ s/^\s*(HEADERS|SOURCES)\s*\+=\s*//;
 if ($line =~ /^\s*([\w+\/\.]+\.(h|c|cpp))\s*\\?\s*$/) {
  $FILES{lc($1)} = 1;
 }
}
close(PRO);

open(VCPROJ,$ARGV[1]);
while($line=<VCPROJ>) {
 if ($line =~ /^\s*RelativePath\s*=\s*\"([\w+\\\.]+\.(h|c|cpp))\"\s*$/) {
  $filepath = $1;
  $filepath =~ s/\.\.\\\.\.\\shared\\/\.\.\/\.\.\//;
  $filepath =~ s/\\/\//g;
  $filepath =~ s/\.\.\/\.\.\/qt\/rhodes\///;
  $type = ($filepath =~ /\.h$/ ? 0 : 1);
  if (!$FILES{lc($filepath)}) {
   $NEW[$type]{$filepath} = 1;
  }
  $FILES{lc($filepath)} = 2;
 }
}
close(VCPROJ);

foreach $filepath (keys %FILES) {
 if ($FILES{$filepath}==1) {
  $type = ($filepath =~ /\.h$/ ? 0 : 1);
  $DEL[$type]{$filepath} = 1;
 }
}

print "NEW:\n\n";
for $type (0..$#NEW) {
 for $filepath (sort {$a cmp $b} keys %{$NEW[$type]}) {
  print "$filepath\\\n";
 }
 print "\n";
}

print "DELETE:\n\n";
for $type (0..$#DEL) {
 for $filepath (sort {$a cmp $b} keys %{$DEL[$type]}) {
  print "$filepath\\\n";
 }
 print "\n";
}
