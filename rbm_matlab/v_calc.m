function [ V ] = v_calc( h, W )
%UNTITLED8 Summary of this function goes here
%   Detailed explanation goes here
[movies, F, ~] = size(W);
V = zeros(5, movies);
for i = 1:movies
    for k = 1:5
        num = 0;
        den = 0;
        for j = 1:F
            num = num + h(j)*W(i, j, k);
        end
        for l = 1:5
            temp = 0;
            for j = 1:F
                temp = temp + h(j)*W(i, j, l);
            end
            den = den + exp(temp);
        end
        V(k, i) = exp(num)/den;
    end
end
end

