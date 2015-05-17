import pandas as pd

DATA_SIZE = 102416306
BLEND_SIZE = 1964391
BLEND_SET = 3

data_in = '../data/ut/all.dta'
idx_in = '../data/ut/all.idx'
blend_ratings = '../gen/blend_only_ratings.txt'

data = pd.read_csv(data_in, sep=' ', names=['user', 'movie', 'date', 'rating'])
idx = pd.read_csv(idx_in, sep=' ', names=['idx'])

blend = []

for i in range(DATA_SIZE):
    if idx['idx'][i] == BLEND_SET:
        blend.append(data['rating'][i])

if len(blend) != BLEND_SIZE:
    print("did not work.")

blend = pd.DataFrame(blend);
blend.to_csv(blend_ratings, sep=' ', header=False, index=False)
