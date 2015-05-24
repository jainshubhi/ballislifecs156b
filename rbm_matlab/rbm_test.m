%% Create dummy dataset:
% Specify number of users and movies
users = 100;
movies = 7;
dummy = cell(users, 1);
% Populate data
for i = 1:users
    dummy{i} = zeros(movies, 2);
    for j = 1:movies
        dummy{i}(j, 1) = j;
        dummy{i}(j, 2) = randi([1, 5], 1);
    end
end
%% Parameter Initialization
% Choose number of hidden features
F = 2;
% create W
W = (1/100)*rand(movies+1, F+1, 5);
% Choose Learning rate.
L = 0.01;
% Create empty reconstruction data set.
recon = cell(users, 1);
%% LEARN W
tic;
% Loop through all users
for epoch = 1:50000
    grad = 0;
    % For each user, find reconstruction error
    for i = 1:users
        % populate V
        V = create_v(dummy{i});
        % Create h
        h = h_calc(V, W);
        % Copy h to assign binary values for exp calculations.
        copy_h = h;
        for j = 1:F
            if h(j) > rand()
                copy_h(j) = 1;
            else
                copy_h(j) = 0;
            end
        end
        % Find exp_data
        exp_data = exp_calc(V, copy_h);
        % Reconstruct V
        new_V = v_calc(h, W);
        % Reconstruct h
        new_h = h_calc(new_V, W);
        % Similar to above.
        copy_new_h = h;
        for j = 1:F
            if h(j) > rand()
                copy_new_h(j) = 1;
            else
                copy_new_h(j) = 0;
            end
        end
        % Find exp_recon
        exp_recon = exp_calc(new_V, copy_new_h);
        % Calculate the gradient for that user. Add to total and average.
        grad = (grad*(i-1) + (exp_data - exp_recon))/i;
    end
    % Take a gradient step with learn rate and averaged over all users.
    W = W - L*grad;  
    if isnan(W(1,1,1))
        epoch
        break
    end
    % If you want to print out rmse at each step, uncomment below code. 
    %{
    for i = 1:users
        recon{i} = predict(dummy, i, W);
    end
    rmse(dummy, recon)
    %}
end
toc;
%% Reconstruct and Find RMSE
% Use W to recreate user preferences.
recon = cell(users, 1);
for i = 1:users
    recon{i} = predict(dummy, i, W);
end
% Print out rmse
rmse(dummy, recon)