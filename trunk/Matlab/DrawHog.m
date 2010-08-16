function DrawHog(X,m,n,p)
Y = ReshapeMatrix(X,m,n,36);
V = hogDraw(Y,p);
figure;im(V);