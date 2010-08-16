function value = ReshapeMatrix(M,r,c,h)
[y,x] = size(M);
value = zeros(r,c,h);
if y ~= r*c 
    return;
end
t=1;
for i=1:r
    for j=1:c
        for k=1:x
            value(i,j,k) = M(t,k);
        end
        t=t+1;
    end
end
end