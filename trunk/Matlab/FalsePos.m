function [val,list] = FalsePos(M, numpos, thr)
n = size(M,1);
temp = zeros(n,1);
val = 0;
j=1;
for i=numpos+1:n
    if M(i,1) >= thr
        val = val + 1;
        temp(j,1) = i;
        j = j + 1;
    end
end
%list = zeros(val,1);
list = temp(1:val,:);
end

