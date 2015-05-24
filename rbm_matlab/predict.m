function [ preds ] = predict( data, user, W )
%UNTITLED9 Summary of this function goes here
%   Detailed explanation goes here
    movies = size(W, 1) - 1;
    V = create_v(data{user});
    h = h_calc(V, W);
    new_V = v_calc(h, W);
    preds = zeros(movies, 2);
    for i = 1:movies
        preds(i, 1) = i;
        preds(i, 2) = expected(new_V(:,i));
    end
end

