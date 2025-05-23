# -*- coding: utf-8 -*-
"""AD_DL_1.ipynb

Automatically generated by Colab.

Original file is located at
    https://colab.research.google.com/drive/1zABHqMgLKeOQwW02XmK1AMOwriMH40UP
"""

# Importing Libraries
import numpy as np
import pandas as pd
import matplotlib.pyplot as plt

from sklearn.datasets import fetch_openml
boston = fetch_openml(name='boston', version=1, as_frame=True)

data = boston.frame
data.columns = boston.feature_names + ['Price']

print("Data shape:", data.shape)

print("Feature names:", boston.feature_names)

print(data.head(10))

print(data.describe())

print(data.info())

from sklearn.model_selection import train_test_split
X = data[boston.feature_names].values
y = data['Price'].values

X_train, X_test, y_train, y_test = train_test_split(X, y, test_size=0.2, random_state=0)
print("X_train shape:", X_train.shape)
print("X_test shape:", X_test.shape)
print("y_train shape:", y_train.shape)
print("y_test shape:", y_test.shape)

from sklearn.linear_model import LinearRegression
regressor = LinearRegression()
regressor.fit(X_train, y_train)

# y_pred = regressor.predict(X_test.to_numpy())
y_pred = regressor.predict(X_test)

plt.scatter(y_test, y_pred, c='green')
plt.xlabel("Price: in $1000's")
plt.ylabel("Predicted value")
plt.title("True value vs predicted value: Linear Regression")
plt.show()

from sklearn.metrics import mean_squared_error, mean_absolute_error
mse = mean_squared_error(y_test, y_pred)
mae = mean_absolute_error(y_test, y_pred)
print("Mean Square Error:", mse)
print("Mean Absolute Error:", mae)