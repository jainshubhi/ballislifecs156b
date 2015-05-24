function [ rmse ] = rmse( data, recon )
%UNTITLED12 Summary of this function goes here
%   Detailed explanation goes here
[users, ~] = size(data);
[movies, ~] = size(data{1});
rmse = 0;
for i = 1:users
    err = data{i}(:,2) - recon{i}(:,2);
    err = err.^2;
    rmse = rmse + sum(err);
end
rmse = rmse/(users*movies);
rmse = rmse^(1/2);
end

