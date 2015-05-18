### RBM pseudo-code
### Notes:
'''
Might be easiest to not use biases at first to make sure gradient
descent is working.  Then we can factor in biases later.

Also, K=5 for this problem.  I didn't want to use a magic number so it
is global at the top.  Similar with number of features.
'''
data = get_Data_fun()

# For each user there is a 5xm matrix, where m is the number of movies
# that user rated.  Call this matrix V.  The column vector v_i represents
# that movie i was rated k by this user.  v_i is 0 everywhere but 1 in the
# kth row.  So if this user rated the third movie as 2, then the third
# column would be (0 1 0 0 0)'.

# There are F latent features that will be the number of hidden units.
# So h_j with j = 1, ..., F

=======
NUM_RATINGS = 5
NUM_FACTORS = 100
NUM_MOVIES = 0

def sumOverFeatures(movie, rating, h, W):
    '''helper function for pCalcV'''
    tot = 0
    for hidden in range(NUM_FACTORS):s
        tot += h(hidden)*W(movie, hidden, rating)

# for pCalcV and pCalch, only iterate over populated columns of V.
# V will be 5x17770 and only rated movies by the user will have populated
# columns.

def pCalcV(V, h, W, B):
    '''V is an empty Kxm matrix, with K the number of ratings and m
     the number of movies.  h is a binary vector for the hidden features.
     W_i_j^k is the interaction parameter between feature j and rating k
     of movie i. B is the bias value matrix. B(i, k) is the bias for movie
     i with rating k.'''
    for movie in range(NUM_MOVIES):
        # Only iterate over populated columns of V.
        for rating in range(NUM_RATINGS):
            num = exp(B(movie, rating) +
                sumOverFeatures(movie, rating, h, W)
            den = 0
            for l in range(1, K):
                den += (exp(B(movie, l) +
                    sumOverFeatures(movie, l, h, W)))
            V(rating, movie) = num/den
    return V

def pCalch(V, h, W, b):
    '''All symbols are the same as above except here b is the
    bias vector for features.'''
    # Calculate probability of each h(j)
    for hidden in range(NUM_FACTORS):
        term = 0
        for movie in range(NUM_MOVIES):
            for rating in range(NUM_RATINGS):
                term += V(rating, movie)*W(movie, hidden, rating)
        # Assign p(h(hidden)=1 | V)
        h(hidden) = 1/(1+exp(-b(hidden) - term))

    return h

### May not need these next two.
### Just found them in the paper. I don't use them
### but there is definitely a way to use them.
def partition(movie, h, W, B):
    Z = 0
    for rating in range(NUM_RATINGS):
        Z += exp(B(movie, rating) +
            sumOverFeatures(movie, rating, h, W))
    return Z

def energy(V, h, W, B, b):
    term1 = 0
    term2 = 0
    term3 = 0
    term4 = 0
    for movie in range(NUM_MOVIES):
        # In one set of loops, get first part of energy term.
        for hidden in range(NUM_FACTORS):
            for rating in range(NUM_RATINGS):
                term1 += W(movie, hidden, rating)*
                    h(hidden)*V(rating, movie)
        # Within the outer loop, get 2nd part.
        term2 += log(partition(movie, h, W, B))
        # Get third part.
        for rating in range(NUM_RATINGS):
            term3 += V(rating, movie)*B(movie, rating)
    # Finally, get last term.
    for hidden in range(NUM_FACTORS):
        term4 += h(hidden)*b(hidden)
    # Combine 4 terms to get energy.
    E = -term1 + term2 - term3 - term4
    return E
#################################



# gradient = rate*(exp_data - exp_recon)

<<<<<<< HEAD

def update_h(V, W, b, last, threshold):
    '''For last epoch, we need to keep the probabilities in h to
    make predictions.  So last = 1 on last epoch.'''
    # Update hidden states:
    # Create an empty h vector 1xF
    h = array(1, NUM_FACTORS)
    h = pCalch(V, h, W, b)

    # If the probability for h(hidden) is higher than rand_num, set to 1.
    if last != 1:
        for hidden in range(NUM_FACTORS):
            if h(hidden) > threshold:
                h(hidden) = 1
            else:
                h(hidden) = 0

    return h

def update_W(W, B, b, minibatch, learn_rate):
    # minibatch is a subset of the data.
    users = size(minibatch)
    exp_data = array(NUM_MOVIES, NUM_FACTORS, NUM_RATINGS)
    exp_recon = array(NUM_MOVIES, NUM_FACTORS, NUM_RATINGS)
    for user in range(users):
        # Populate V:
        '''
        1) Pick a user
        2) Based on movies they have rated, fill in a 5x17770 matrix with
           their ratings.
        3) Columns for movies they haven't rated can remain
           empty (0s or null or whichever is fastest.)
        4) Columns for movies they have rated are 1 in the row of the rating
           and 0 elsewhere.
        '''
        # Get a random number between 0 and 1
        rand_num = rand(0, 1)

        # Update hidden states.
        h = update_h(V, W, b, 0, rand_num)

        # See how often states are on together.  Add to exp_data.
        for movie in range(NUM_MOVIES):
            # only iterate over populated columns of V.
            for feature in range(NUM_FACTORS):
                for rating in range(NUM_RATINGS):
                    # entry will be 1 if rating for that movie is on
                    # with feature. Will be 0 otherwise.
                    exp_data(movie, feature, rating) +=
                        V(rating, movie)*h(feature)

        # Reconstruct visible states:
        V = pCalcV(V, h, W, b)

        # If the probability for V(rating, movie) is higher than rand_num,
        # set to 1.
        for movie in range(NUM_MOVIES):
            # Only iterate over populated columns of V.
            for rating in range(NUM_RATINGS):
                if V(rating, movie) > rand_num:
                    V(rating, movie) = 1
                else:
                    V(rating, movie) = 0

        # Update hidden states again:
        h = update_h(V, W, b, 0, rand_num)

        for movie in range(NUM_MOVIES):
            for feature in range(NUM_FACTORS):
                for rating in range(NUM_RATINGS):
                    # entry will be 1 if rating for that movie is on with
                    # feature. Will be 0 otherwise.
                    exp_recon(movie, feature, rating) +=
                        V(rating, movie)*h(feature)

    # Repeat above for all users in a mini-batch (Size between 10 and 100).
    # Average the exp_data and exp_recon by dividing by number of users.
    W += (learn_rate/users)*(exp_data - exp_recon)
    return W
