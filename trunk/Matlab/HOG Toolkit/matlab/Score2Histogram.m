function Hst = Score2Histogram(M, lower, upper)
n = (upper - lower)/4000;
Hst = hist(M,[lower:n:upper]);

end