function MissRate_FPPW_Curve(Pos, Neg, thr, color)
numPos = size(Pos,1);
numNeg = size(Neg,1);
numThr = size(thr,2);
M = zeros(numThr,2);
for i=1:numThr
    tp = 0;
    fn = 0;
    fp = 0;
    % true positive, false negative
    for j=1:numPos
        if Pos(j,1) >= thr(1,i)
            tp = tp + 1;
        else
            fn = fn + 1;
        end
    end
    % false positive
    for j=1:numNeg
        if Neg(j,1) > thr(1,i)
            fp = fp + 1;
        end
    end
    M(i,1) = fp/(numPos+numNeg);
    M(i,2) = fn/(fn + tp);
end
plot(M(:,1),M(:,2),color);
end