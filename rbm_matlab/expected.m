function [ rating ] = expected( probs )
%UNTITLED10 Summary of this function goes here
%   Detailed explanation goes here
rating = 0;
for i = 1:5
    rating = rating + i*probs(i);
end

end

