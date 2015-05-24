function [ expect ] = exp_calc( V, h )
%UNTITLED7 Summary of this function goes here
%   Detailed explanation goes here
[~, movies] = size(V);
[~, F] = size(h);
expect = zeros(movies, F, 5);
for i = 1:movies
    for j = 1:F
        for k = 1:5
            expect(i, j, k) = expect(i, j, k) + V(k, i)*h(j);
        end
    end
end

end

