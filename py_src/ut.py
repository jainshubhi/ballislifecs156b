import pandas as pd

data_in = 'data/um/all.dta'
data_out = 'data/ut/all.dta'

idx_in = 'data/um/all.idx'
idx_out = 'data/ut/all.idx'

data = pd.read_csv(data_in, sep=' ', names=['user', 'movie', 'date', 'rating'])
idx = pd.read_csv(idx_in, sep=' ', names=['idx'])

# join data and idx
df = pd.DataFrame()
df['user'] = data['user']
df['movie'] = data['movie']
df['date'] = data['date']
df['rating'] = data['rating']
df['idx'] = idx['idx']

# sort
df.sort(['user', 'date'], ascending=[True, True], inplace=True)

# separate again
new_data = pd.DataFrame()
new_data['user'] = df['user']
new_data['movie'] = df['movie']
new_data['date'] = df['date']
new_data['rating'] = df['rating']
new_idx = df['idx']

# put back in files
new_data.to_csv(data_out, sep=' ', header=False, index=False)
new_idx.to_csv(idx_out, sep=' ', header=False, index=False)
