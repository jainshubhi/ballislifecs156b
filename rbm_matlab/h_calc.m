function [ h ] = h_calc( V, W )
%UNTITLED4 Summary of this function goes here
%   Detailed explanation goes here
[movies, F, ~] = size(W);
h = zeros(1, F);
for j = 1:F
    sum = 0;
    for i = 1:movies
        for k = 1:5
           sum = sum + V(k, i)*W(i,j,k);
        end
    end
    h(j) = 1/(1+exp(-sum));
end

