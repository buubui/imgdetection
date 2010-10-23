function M = Libsvm2Sparse(str, r,c)
M = zeros(r,c);
X = textread(str,'%s');
[rX, cX] = size(X);
index =0;
for i=1:rX
    if strcmp('-1',X(i,1))==1 || strcmp('+1',X(i,1))==1  || strcmp('1',X(i,1))==1 || strcmp('0',X(i,1))==1
        index = index +1;
        continue;
    end
    
    temp = regexp(char(X(i,1)),'\:','split');
    K = str2double(temp);
    M(index,K(1,1)) = K(1,2);
end
end