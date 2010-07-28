function value = HisOfCell(Dm, Da, c, r, dc, dr, bins)
% Dm: Matrix weight
% Da: matrix of gradient
% c, r: column and row of a point
% d1, d2: lenght of cell
% bins: number of bin
[u,v] = size(Dm);
MatM = zeros(dr, dc);
MatA = zeros(dr, dc);
value = zeros(1,bins);
if (r+dr) <= u && (c+dc) <= v
    MatM = Dm(r:r+dr, c:c+dc);
    MatA = Da(r:r+dr, c:c+dc);
elseif (r+dr) > u && (c+dc) <= v
     MatM = Dm(end-dr:end , c:c+dc);
     MatA = Da(end-dr:end , c:c+dc);
elseif (r+dr) <= u && (c+dc) > v
    MatM = Dm(r:r+dr , end-dc: end);
    MatA = Da(r:r+dr , end-dc: end);
else
    MatM = Dm(end-dr:end , end-dc: end);
    MatA = Da(end-dr: end , end-dc: end);
end
Dtheta = 180/bins;
for i =1:bins
    for j =1:dr
        for k =1:dc
            if(MatA(j,k)< i*Dtheta && MatA(j,k)>=(i-1)*Dtheta)
                value(1,i) = value(1 , i) + MatM(j,k);
            end
        end
    end
end

 
end