function val = PrecisionRecallCurve(M, numpos, thr, color)
n = size(M,1);
nTh = size(thr,2);
val = zeros(nTh,2);
for i=1:nTh
    tp = 0;
    fp = 0;
    % calc RECALL
    for j=1:numpos
        if M(j,1) >= thr(1,i)
            tp = tp + 1;
        end
    end 
    val(i,1) = tp/numpos;
    % calc precision
	
    for j=numpos+1:n
        if M(j,1) >= thr(1,i)
            fp = fp + 1;
        end
    end
    val(i,2) = tp/(tp+fp);
end
plot(val(:,1),val(:,2),color);
%axis([0.9 1 0.7 1]);
end