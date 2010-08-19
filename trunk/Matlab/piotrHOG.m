function V= piotrHOG(filepath)
I=imread(filepath);
I = double(rgb2gray(I));
  H=hog(I,8,9);
  V=hogDraw(H,35); 
  figure; im(V);
