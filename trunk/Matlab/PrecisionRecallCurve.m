function val = PrecisionRecallCurve(M, numpos, thr)
n = size(M,1);
nTh = size(thr,2);
val = zeros(nTh,2);
for i=1:nTh
    tp = 0;
    recPos = 0;
    % calc precision
    for j=1:numpos
        if M(j,1) >= thr(1,i)
            tp = tp + 1;
        end
    end
    val(i,1) = tp/numpos;
    % calc recall
    for j=1:n
        if M(j,1) >= thr(1,i)
            recPos = recPos + 1;
        end
    end
    val(i,2) = tp/recPos;
end
plot(val(:,2),val(:,1));
end