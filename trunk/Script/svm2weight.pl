#!perl
# Compute the weight vector of linear SVM based on the model file
# Author: Thorsten Joachims (thorsten@joachims.org)
# Call: perl svm2weight.pl model

open(M,$ARGV[0]) || die();

$l=<M>;
if(($l=<M>) != 0) { die("Not linear Kernel!\n"); }
$l=<M>;
$l=<M>;
$l=<M>;
$l=<M>;
$l=<M>;
$l=<M>; 
$l=<M>;
$l=<M>;
$l=<M>;
if($l !~ /threshold b/) { die("Parsing error!\n"); }
($num,$comm)=split(/#/,$l);
$num = sprintf("%.10f",$num);
#$num = -1*$num;
while($l=<M>) {
    ($features,$comments)=split(/#/,$l);
    ($alpha,@f)=split(/ /,$features);
    for $p (@f) {
	($a,$v)=split(/:/,$p);
	$w[$a]+=$alpha*$v;
    }
}
print "$#w\n";
for($i=1;$i<=$#w;$i++) {
    #print "$i:$w[$i]\n";
	print "$w[$i]\n";
}
print "$num\n";

