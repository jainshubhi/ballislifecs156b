function [ expect ] = exp_calc( V, h )
%UNTITLED7 Summary of this function goes here
%   Detailed explanation goes here
movies = size(V, 2) - 1;
F = size(h, 2) - 1;
expect = zeros(movies+1, F+1, 5);
for i = 1:movies+1
    for j = 1:F+1
        for k = 1:5
            expect(i, j, k) = V(k, i)*h(j);
        end
    end
end

end

