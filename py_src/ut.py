import pandas as pd

data_in = 'data/um/all.dta'
data_out = 'data/ut/all.dta'

idx_in = 'data/um/all.idx'
idx_out = 'data/ut/all.idx'

data = pd.read_csv(data_in, sep=' ', names=['user', 'movie', 'date', 'rating'])
idx = pd.read_csv(idx_in, sep=' ', names=['idx'])

# join data and idx
df = pd.DataFrame()
df['user', 'movie', 'date', 'rating'] = data['user', 'movie', 'date', 'rating']
df['idx'] = idx['idx']

# sort
df.sort(['user', 'date'], ascending=[True, True], inplace=True)

# separate again
data = df['user', 'movie', 'date', 'rating']
idx = df['idx']

# put back in files
data.to_csv(data_out, sep=' ', header=False, index=False)
idx.to_csv(idx_out, sep=' ', header=False, index=False)

