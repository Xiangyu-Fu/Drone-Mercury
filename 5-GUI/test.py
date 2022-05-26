from sklearn.datasets import make_regression
from sklearn.metrics import mean_squared_error
from mlxtend.evaluate import BootstrapOutOfBag
from sklearn.model_selection import train_test_split, KFold, LeaveOneOut, ShuffleSplit, cross_validate
import pandas as pd
import numpy as np
import seaborn as sns

def gen_data(n_samp, n_feat, noise_std, bias, outlier_strength):
    X, y, true_coef = make_regression(n_samples=n_samp, n_informative=n_feat-1, n_features=n_feat, random_state=43, noise=noise_std,bias=bias, coef=True)
    true_intercept=bias
    return X,y,true_coef, true_intercept

X, y, true_coef, true_intercept = gen_data(n_samp=300,n_feat=3,noise_std=3.3,bias=0.3, outlier_strength=0)

from sklearn.linear_model import LinearRegression
class FixedCoefLinearRegression(LinearRegression):
    def __init__(self, fixed_coef, fixed_intercept):
        super(FixedCoefLinearRegression, self).__init__()
        self.fixed_coef = fixed_coef
        self.fixed_intercept = fixed_intercept
    def fit(self, X, y, sample_weight=None):
        self.coef_ = self.fixed_coef
        self.intercept_ = self.fixed_intercept
lr_true = FixedCoefLinearRegression(fixed_coef=true_coef, fixed_intercept=true_intercept)

from sklearn.linear_model import LinearRegression
# Linear Regression
lr = LinearRegression()

def root_mean_squared_error(y, y_pred):
    # YOUR CODE HERE
    sz = len(y.shape)
    assert sz == 1, "the input data must in 1-dimension"
    rmse = np.sqrt(mean_squared_error(y, y_pred))
    return rmse

from sklearn.metrics import make_scorer
rmse_scorer = make_scorer(root_mean_squared_error)


k_max = 280 #second largest k (max for generating the grid)
ks = np.array([2,3,5,10])
ks = np.append(ks,np.arange(15,k_max, 20)) #grid of k values
ks = np.append(ks,X.shape[0]) #k=n_samples
print('k or number of splits for the following validation experiments:\n ', ks)

re_cf = pd.DataFrame()
re_rmse = pd.DataFrame()
for kk in ks:
    # YOUR CODE HERE
    cv = ShuffleSplit(n_splits=kk, test_size=.37, random_state=350)
    c_v = cross_validate(lr, X, y, cv=cv, scoring=rmse_scorer, return_train_score=True)

    # The given code
    re_rmse = re_rmse.append(pd.DataFrame({'splits': kk*np.ones(len(c_v['test_score'])),'set': len(c_v['test_score'])*['test'],'model': len(c_v['test_score'])*['lr'],'type': len(c_v['test_score'])*['resampling'] , 'RMSE': c_v['test_score']}),ignore_index=True)
    re_rmse = re_rmse.append(pd.DataFrame({'splits': kk*np.ones(len(c_v['train_score'])),'set': len(c_v['train_score'])*['train'], 'model': len(c_v['train_score'])*['lr'],'type': len(c_v['train_score'])*['resampling'] , 'RMSE': c_v['train_score']}),ignore_index=True)
    tmp = pd.DataFrame([true_coef-est.coef_ for est in c_v['estimator']])
    tmp['diff_intercept'] = [true_intercept-est.intercept_ for est in c_v['estimator']]
    tmp['splits'] = kk*np.ones((len(tmp)))
    tmp['type'] = 'resampling'
    re_cf = re_cf.append(tmp)
re_cf.columns = ['diff_coef_' + str(i) for i in range(len(true_coef))] + ['diff_intercept'] + ['splits'] + ['type']