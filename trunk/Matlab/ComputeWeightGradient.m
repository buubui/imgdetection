function [MatM, MatA] = ComputeWeightGradient(im, Dx, Dy)
%%% normalize gamma here
img = rgb2gray(im);
img = im2double(img);
[r, c] = size(img);
imgX = imfilter(img,Dx);
imgY = imfilter(img, Dy);
%%% compute weight
MatM = zeros(r, c);
for i =1:r
    for j = 1:c
        MatM(i,j) = (imgX(i,j).^2 + imgY(i,j).^2).^0.5;
    end
end
%%% compute 
MatA = zeros(r, c);
for i =1:r
    for j = 1:c
        MatA(i,j) = atand(imgY(i,j)/imgX(i,j));
    end
end
end