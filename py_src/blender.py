import sys
import os

import numpy as np
from sklearn import linear_model
from sklearn.linear_model import SGDRegressor

BLEND_RATINGS = "gen/blend_only_ratings.txt"

def load_data(filename):
    f = open(filename)
    lst = []
    for line in f:
        lst.append(float(line))
    return lst

# check inputs
if len(sys.argv) < 5:
    print("need at least 2 models to blend.")
    exit(1)

# load data
print("getting data.")
qual_files, blend_files = [], []
qual_preds, blend_preds = [], []
blend_actuals = load_data(BLEND_RATINGS)

for i in range(1, len(sys.argv)):
    if i % 2 == 1:
        qual_files.append(sys.argv[i])
    else:
        blend_files.append(sys.argv[i])

# load data into preds
for f in qual_files:
    qual_preds.append(load_data(f))

for f in blend_files:
    blend_preds.append(load_data(f))

# need to transpose blend_preds
blend_preds = np.array(blend_preds).transpose()
blend_actuals = np.array(blend_actuals)

# train coefficients
print("training model.")
clf = linear_model.SGDRegressor(
    fit_intercept=False,
    penalty="elasticnet",
    n_iter=500,
    alpha=.01)
clf.fit(blend_preds, blend_actuals)
coeffs = list(clf.coef_)
sum_c = sum(coeffs)

print("model coefficients: " + str(coeffs))
print("sum of coefficients: " + str(sum_c))

# predict
f = open('out', 'w')
for i in range(len(qual_preds[0])):
    pred = 0
    for j in range(len(qual_preds)):
        pred += coeffs[j] * qual_preds[j][i]
    if pred > 5:
        pred = 5
    if pred < 1:
        pred = 1
    f.write(str(pred) + '\n')
f.close()

os.system("python3 blend.py out > out2")
