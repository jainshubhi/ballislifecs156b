# Get Data

data = get_Data_fun()

# For each user there is a 5xm matrix, where m is the number of movies
# that user rated.  Call this matrix V.  The column vector v_i represents
# that movie i was rated k by this user.  v_i is 0 everywhere but 1 in the
# kth row.  So if this user rated the third movie as 2, then the third
# column would be (0 1 0 0 0)'.

# There are F latent features that will be the number of hidden units.
# So h_j with j = 1, ..., F


NUM_RATINGS = 5
NUM_FACTORS = 100
NUM_MOVIES = 0

def sumOverFeatures(i, rating, h, W):
    '''helper function for pCalcV'''
    tot = 0
    for j in range(NUM_FACTORS):
        tot += h(j) * W(i, j, k)
    return tot


def pCalcV(V, h, W, B, F):
    '''V is an empty Kxm matrix, with K the number of ratings and m
     the number of movies.  h is a binary vector for the hidden features.
     W_i_j^k is the interaction parameter between feature j and rating k
     of movie i. B is the bias value matrix. b_i^k is the bias for movie i
     with rating k.  F is the number of hidden features.'''
    for i in range(NUM_MOVIES):
        for k in range(NUM_RATINGS):
            num = exp(B(i, k) + sumOverFeatures(i, k, h, W))
            den = 0
            for l in range(1, K):
                den += (exp(B(i, l) + sumOverFeatures(i, l, h, W)))

            V(k, i) = num/den

    return V

def pCalch(V, h, W, b):
    '''All symbols are the same as above except here b is the
    bias vector for features.'''
    # h is a 1xF vector.  Not sure how to code this fact.
    h = array(1, NUM_FACTORS)
    # Calculate probability of each h(j)
    for j in range(NUM_FACTORS):
        term = 0
        for i in range(NUM_MOVIES):
            for k in range(1, NUM_RATINGS):
                term += V(k, i)*W(i, j, k)
        # Assign p(h(j)=1 | V)
        h(j) = 1/(1+exp(-b(j) - term))

    return h

def partition(i, h, W, B):
    Z = 0
    for l in range(NUM_RATINGS):
        Z += exp(B(i, l) + sumOverFeatures(i, l, h, W))
    return Z


def energy(V, h, W, B, b):
    term1 = 0
    term2 = 0
    term3 = 0
    term4 = 0
    for i in range(NUM_MOVIES):
        # In one set of loops, get first part of energy term.
        for j in range(NUM_FACTORS):
            for k in range(NUM_RATINGS):
                term1 += W(i, j, k) *h(j) * V(k, i)
        # Within the outer loop, get 2nd part.
        term2 += log(partition(i, F, h, W, B))
        # Get third part.
        for k in range(NUM_RATINGS):
            term3 += V(k, i) * B(i, k)
    # Finally, get last term.
    for j in range(F):
        term4 += h(j) * b(j)
    # Combine 4 terms to get energy.
    E = -term1 + term2 - term3 - term4
    return E

# Need to do Contrastive Divergence and find predictions.

# gradient = rate*(exp_data - exp_recon)

# Update hidden states:
H = pCalch(V, h, W, b, F)

# Get a random number between 0 and 1
rand_num = rand(0, 1)

# If the probability for H(j) is higher than rand_num, set to 1.
for j in range(F):
    if H(j) > rand_num:
        H(j) = 1
    else:
        H(J) = 0
# This happens for every update of H except the last one.
# Use probabilities for the last update.

# Update visible states:
V = pCalcV(V, h, W, B, F)
def exp_data(V, h, W, B, b, F):
    '''Finds the expectation '''
