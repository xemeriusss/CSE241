##############################
# Telco Customer Churn 
##############################

# Each row represents a customer, each column contains customer’s attributes described on the column Metadata.

# The data set includes information about (from Kaggle):
# Customers who left within the last month – the column is called Churn
# Services that each customer has signed up for – phone, multiple lines, internet, online security, online backup, device protection, tech support, and streaming TV and movies
# Customer account information – how long they’ve been a customer, contract, payment method, paperless billing, monthly charges, and total charges
# Demographic info about customers – gender, age range, and if they have partners and dependents

# The dataset contains 7043 rows (customers) and 21 columns (features).

# Features:

# CustomerID
# Gender
# SeniorCitizen: Whether the customer is a senior citizen or not (1, 0)
# Partner: Whether the customer has a partner or not (Yes, No)
# Dependents: Whether the customer has dependents or not (Yes, No) (Dependents means children or retired parents)
# Tenure: Number of months the customer has stayed with the company
# PhoneService: Whether the customer has a phone service or not (Yes, No)
# MultipleLines: Whether the customer has multiple lines or not (Yes, No, No phone service)
# InternetService: Customer’s internet service provider (DSL, Fiber optic, No)
# OnlineSecurity: Whether the customer has online security or not (Yes, No, No internet service)
# OnlineBackup: Whether the customer has online backup or not (Yes, No, No internet service)
# DeviceProtection: Whether the customer has device protection or not (Yes, No, No internet service)
# TechSupport: Whether the customer has tech support or not (Yes, No, No internet service)
# StreamingTV: Whether the customer has streaming TV or not (Yes, No, No internet service) (Like Netflix)
# StreamingMovies: Whether the customer has streaming movies or not (Yes, No, No internet service) (Like Netflix)
# Contract: The contract term of the customer (Month-to-month, One year, Two year)
# PaperlessBilling: Whether the customer has paperless billing or not (Yes, No)
# PaymentMethod: The customer’s payment method (Electronic check, Mailed check, Bank transfer (automatic), Credit card (automatic))
# MonthlyCharges: The amount charged to the customer monthly
# TotalCharges: The total amount charged to the customer
# Churn: Whether the customer churned or not (Yes or No) (Customers who left within the last month)

# Necessary libraries
from collections import Counter
from catboost import CatBoostClassifier
import numpy as np
import pandas as pd
import matplotlib.pyplot as plt
import warnings
import seaborn as sns
import math
from sklearn.model_selection import GridSearchCV, cross_validate, train_test_split
from sklearn.ensemble import RandomForestClassifier, GradientBoostingClassifier
#from sklearn.tree import DecisionTreeClassifier
from sklearn.metrics import accuracy_score, precision_score, recall_score, f1_score,roc_auc_score
from sklearn.tree import DecisionTreeClassifier
from sklearn.preprocessing import LabelEncoder
warnings.simplefilter(action="ignore")

# Setting the display options
pd.set_option('display.max_columns', None)
pd.set_option('display.width', 170)
pd.set_option('display.max_rows', None)
pd.set_option('display.float_format', lambda x: '%.3f' % x)

# Importing the dataset
df = pd.read_csv("Telco-Customer-Churn.csv")

############### Displaying general informations about dataset ###############
df.shape
df.dtypes

# Total charges should be numeric
df["TotalCharges"] = pd.to_numeric(df["TotalCharges"], errors='coerce')

# Encoding the churn into binary form: 1 if "Yes," 0 if "No".
df["Churn"] = df["Churn"].apply(lambda x : 1 if x == "Yes" else 0)

# df.dtypes
# df.head()
# df.tail()
# df.isnull().sum()

print(df.shape)
print(df.dtypes)
print(df.head())
print(df.tail())
print(df.isnull().sum())

################ Handling categorical and numerical variables #################

categorical_threshold = 10
cardinal_threshold = 20

# Categorical features:
cat_cols = [col for col in df.columns if df[col].dtypes == "O"]

# Binary numerical (1 or 0) features:
num_but_cat = [col for col in df.columns if df[col].nunique() < categorical_threshold and df[col].dtypes != "O"]

# Cardinal features:
cat_but_car = [col for col in df.columns if df[col].nunique() > cardinal_threshold and df[col].dtypes == "O"]

# New categorical features without cardinal and with binary features:
cat_cols = cat_cols + num_but_cat
cat_cols = [col for col in cat_cols if col not in cat_but_car]

# Numerical features:
num_cols = [col for col in df.columns if df[col].dtypes != "O"]

# Numerical without binary features:
num_cols = [col for col in num_cols if col not in num_but_cat]

print(f"Categorical features: {cat_cols}\n")
print(f"Numerical features: {num_cols}\n")
print(f"Binary numerical features: {num_but_cat}\n")


################ Analyzing numerical features #################


for col in num_cols:
    sns.boxplot(x=col, data=df)
    plt.show()


################ Analyzing categorical features with target #################

# Categorical features with target:
for col in cat_cols:
    print(col, ":", df[col].nunique())

    if col not in ["customerID", "Churn"]:
        sns.countplot(x=col, hue="Churn", data=df)
        plt.show()


# ################ Feature Engineering #################

# Outliers:
for col in num_cols:
    print(col, ":", df[col].nunique())

    # Calculate the first and third quartiles
    Q1 = df[col].quantile(0.25)
    Q3 = df[col].quantile(0.75)

    # Calculate the IQR (Interquartile Range)
    IQR = Q3 - Q1

    # Define the lower and upper bounds for potential outliers
    lower_bound = Q1 - 1.5 * IQR
    upper_bound = Q3 + 1.5 * IQR

    # Identify potential outliers
    outliers = df[(df[col] < lower_bound) | (df[col] > upper_bound)]

    # Print the potential outliers
    print("Potential outliers:")
    print(outliers)

    df = df[~((df[col] < lower_bound) | (df[col] > upper_bound))] # Removing outliers
    
    sns.boxplot(x=col, data=df)
    plt.show()



# Missing values:
def missing_values_table(dataframe, na_name=False):
    na_columns = [col for col in dataframe.columns if dataframe[col].isnull().sum() > 0]
    n_miss = dataframe[na_columns].isnull().sum().sort_values(ascending=False)
    ratio = (dataframe[na_columns].isnull().sum() / dataframe.shape[0] * 100).sort_values(ascending=False)
    missing_df = pd.concat([n_miss, np.round(ratio, 2)], axis=1, keys=['n_miss', 'ratio'])
    print(missing_df, end="\n")
    if na_name:
        return na_columns

na_columns = missing_values_table(df, na_name=True)

# Fills the missing values with median:
df["TotalCharges"].fillna(df["TotalCharges"].median(), inplace=True)

print(df.isnull().sum())


################ Correlation #################

# Correlation between numerical features:
corr_matrix = df[num_cols].corr()
sns.clustermap(corr_matrix, annot=True, fmt=".4f")
plt.title("Correlation Between Numerical Features")
plt.show()



################### FIRST MODEL ###################

dff = df.copy()
cat_cols = [col for col in cat_cols if col not in ["Churn"]]
cat_cols

def one_hot_encoder(dataframe, categorical_cols, drop_first=False):
    dataframe = pd.get_dummies(dataframe, columns=categorical_cols, drop_first=drop_first)
    return dataframe

dff = one_hot_encoder(dff, cat_cols, drop_first=True)
dff.head()

y = dff["Churn"]
X = dff.drop(["Churn","customerID"], axis=1)

X_train, X_test, y_train, y_test = train_test_split(X, y, test_size=0.30, random_state=1234)



gb_model = GradientBoostingClassifier(random_state=11).fit(X_train, y_train)
y_pred = gb_model.predict(X_test)


print(f"Accuracy: {round(accuracy_score(y_pred, y_test), 4)}")
print(f"Recall: {round(recall_score(y_pred,y_test),4)}")
print(f"Precision: {round(precision_score(y_pred,y_test), 4)}")
print(f"F1: {round(f1_score(y_pred,y_test), 4)}")
print(f"Auc: {round(roc_auc_score(y_pred,y_test), 4)}")

# ROC Curve
y_probs = gb_model.predict_proba(X_test)
y_probs = y_probs[:, 1]

from sklearn.metrics import roc_curve, roc_auc_score

fpr, tpr, thresholds = roc_curve(y_test, y_probs)

plt.plot([0, 1], [0, 1], linestyle="--")
plt.plot(fpr, tpr, marker=".", color="r")
plt.title("ROC Curve")
plt.xlabel("False Positive Rate")
plt.ylabel("True Positive Rate")
plt.show()

# Confusion Matrix
from sklearn.metrics import confusion_matrix

cm = confusion_matrix(y_test, y_pred)
sns.heatmap(cm, annot=True, fmt="d")
plt.title("Confusion Matrix")
plt.show()


################### Feature Importance ###################

# Feature Importance Analysis
feature_importances = gb_model.feature_importances_
feature_names = X_train.columns

feature_importance_df = pd.DataFrame({'Feature': feature_names, 'Importance': feature_importances})
feature_importance_df = feature_importance_df.sort_values(by='Importance', ascending=False)

# Plotting Feature Importance
plt.figure(figsize=(12, 8))
sns.barplot(x='Importance', y='Feature', data=feature_importance_df)
plt.title('Feature Importance Analysis')
plt.show()



################### Feature Selection ###################

# Set a threshold for feature importance
threshold = 0.001  

# Select features based on the threshold
selected_features = feature_importance_df[feature_importance_df['Importance'] >= threshold]['Feature'].tolist()

# Update the dataset with selected features
X_train_selected = X_train[selected_features]
X_test_selected = X_test[selected_features]

# Retrain the model on the new dataset
gb_model.fit(X_train_selected, y_train)

# Plotting Selected Features
plt.figure(figsize=(10, 6))
sns.barplot(x='Importance', y='Feature', data=feature_importance_df[feature_importance_df['Feature'].isin(selected_features)])
plt.title('Selected Features')
plt.show()

# Make predictions on the test set
y_pred_selected = gb_model.predict(X_test_selected)

# Print updated model performance metrics
print("\nModel Metrics after Feature Selection:")
print(f"Accuracy: {round(accuracy_score(y_pred_selected, y_test), 4)}")
print(f"Recall: {round(recall_score(y_pred_selected, y_test), 4)}")
print(f"Precision: {round(precision_score(y_pred_selected, y_test), 4)}")
print(f"F1: {round(f1_score(y_pred_selected, y_test), 4)}")
print(f"AUC: {round(roc_auc_score(y_pred_selected, y_test), 4)}")

##################################################################################################
# New Features

df["NEW_Age_Status"] = df["SeniorCitizen"].apply(lambda x: "Senior" if x == 1 else "Young")


df['NEW_TotalServices'] = (df[['PhoneService', 'InternetService', 'OnlineSecurity',
                                       'OnlineBackup', 'DeviceProtection', 'TechSupport',
                                       'StreamingTV', 'StreamingMovies']]== 'Yes').sum(axis=1)

#df["NEW_Contract_Status"] = df["Contract"].apply(lambda x: "Long" if x == "Two year" else "Short")
df["NEW_Engaged"] = df["Contract"].apply(lambda x: 1 if x in ["One year","Two year"] else 0)

df["NEW_Payment_Status"] = df["PaymentMethod"].apply(lambda x: "Automatic" if x == "Bank transfer (automatic)" or x == "Credit card (automatic)" else "Manual")

df["NEW_Tenure_Status"] = df["tenure"].apply(lambda x: "Long" if x > 60 else "Short")

df["NEW_MonthlyCharges_Status"] = df["MonthlyCharges"].apply(lambda x: "High" if x > 70 else "Low")

df["NEW_TotalCharges_Status"] = df["TotalCharges"].apply(lambda x: "High" if x > 2000 else "Low")

df["NEW_OnlineSecurity_OnlineBackup"] = df["OnlineSecurity"] + df["OnlineBackup"]

df["NEW_OnlineSecurity_OnlineBackup"] = df["NEW_OnlineSecurity_OnlineBackup"].apply(lambda x: "NoService" if x == 0 else "Service")     

df["NEW_StreamingTV_StreamingMovies"] = df["StreamingTV"] + df["StreamingMovies"]

df["NEW_StreamingTV_StreamingMovies"] = df["NEW_StreamingTV_StreamingMovies"].apply(lambda x: "NoService" if x == 0 else "Service")

# #df["NEW_AVG_Service_Fee"] = df["MonthlyCharges"] / (df['NEW_TotalServices'] + 1)

print(df.dtypes)



# Categorical features:
cat_cols = [col for col in df.columns if df[col].dtypes == "O"]

# Binary numerical (1 or 0) features:
num_but_cat = [col for col in df.columns if df[col].nunique() < categorical_threshold and df[col].dtypes != "O"]

# Cardinal features:
cat_but_car = [col for col in df.columns if df[col].nunique() > cardinal_threshold and df[col].dtypes == "O"]

# New categorical features without cardinal and with binary features:
cat_cols = cat_cols + num_but_cat
cat_cols = [col for col in cat_cols if col not in cat_but_car]

# Numerical features:
num_cols = [col for col in df.columns if df[col].dtypes != "O"]

# Numerical without binary features:
num_cols = [col for col in num_cols if col not in num_but_cat]

# Label Encoding
def label_encoder(dataframe, binary_col):
    labelencoder = LabelEncoder()
    dataframe[binary_col] = labelencoder.fit_transform(dataframe[binary_col])
    return dataframe

binary_cols = [col for col in df.columns if df[col].dtypes == "O" and df[col].nunique() == 2]
binary_cols

for col in binary_cols:
    df = label_encoder(df, col)

df.head()
cat_cols = [col for col in cat_cols if col not in binary_cols and col not in ["Churn"]]
cat_cols

def one_hot_encoder(dataframe, categorical_cols, drop_first=False):
    dataframe = pd.get_dummies(dataframe, columns=categorical_cols, drop_first=drop_first)
    return dataframe

df = one_hot_encoder(df, cat_cols, drop_first=True)



################### OTHER MODELS ###################

y = df["Churn"]
X = df.drop(["Churn","customerID"], axis=1)

print("SECOND MODEL")
print("Gradient Boosting Classifier")
rf_model = GradientBoostingClassifier(random_state=11)
rf_model.get_params()

cv_results = cross_validate(rf_model, X, y, cv=5, scoring=["accuracy", "f1", "roc_auc"])
print("Accuracy: ", cv_results['test_accuracy'].mean())
print("F1: ", cv_results['test_f1'].mean())
print("ROC_AUC: ", cv_results['test_roc_auc'].mean())

print("Random Forest Classifier")
rf_model = RandomForestClassifier(random_state=11)
rf_model.get_params()

cv_results = cross_validate(rf_model, X, y, cv=5, scoring=["accuracy", "f1", "roc_auc"])
print("Accuracy: ", cv_results['test_accuracy'].mean())
print("F1: ", cv_results['test_f1'].mean())
print("ROC_AUC: ", cv_results['test_roc_auc'].mean())

print("Decision Tree Classifier")
rf_model = DecisionTreeClassifier(random_state=11)
rf_model.get_params()

cv_results = cross_validate(rf_model, X, y, cv=5, scoring=["accuracy", "f1", "roc_auc"])
print("Accuracy: ", cv_results['test_accuracy'].mean())
print("F1: ", cv_results['test_f1'].mean())
print("ROC_AUC: ", cv_results['test_roc_auc'].mean())

print("CatBoost Classifier")
rf_model = CatBoostClassifier(random_state=11, verbose=False)
rf_model.get_params()

cv_results = cross_validate(rf_model, X, y, cv=5, scoring=["accuracy", "f1", "roc_auc"])
print("Accuracy: ", cv_results['test_accuracy'].mean())
print("F1: ", cv_results['test_f1'].mean())
print("ROC_AUC: ", cv_results['test_roc_auc'].mean())

# Feature Importance Analysis
# feature_importances = rf_model.feature_importances_
# feature_names = X_train.columns

# feature_importance_df = pd.DataFrame({'Feature': feature_names, 'Importance': feature_importances})
# feature_importance_df = feature_importance_df.sort_values(by='Importance', ascending=False)

# # Plotting Feature Importance
# plt.figure(figsize=(12, 8))
# sns.barplot(x='Importance', y='Feature', data=feature_importance_df)
# plt.title('Feature Importance Analysis')
# plt.show()


###################################################











    




