function [ V ] = create_v( prefs )
%UNTITLED5 Summary of this function goes here
%   Detailed explanation goes here
[movies, ~] = size(prefs);
V = zeros(5, movies);
% Set the proper row of V to 1.
for i = 1:movies
    V(prefs(i, 2), i) = 1;
    
end
end

