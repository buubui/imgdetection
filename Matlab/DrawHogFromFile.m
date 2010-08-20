function DrawHogFromFile(F)
A = csvread(F);
B = size(A);
C = A(2:B(1),:);
D = A(1:1,1:2);
DrawHog(C,D(1,1),D(1,2),50);