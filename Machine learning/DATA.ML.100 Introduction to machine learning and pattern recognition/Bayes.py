import pickle
import numpy as np
import matplotlib.pyplot as plt
from random import random
from skimage.transform import resize


def class_acc(pred,gt):
    P = 0
    for i in range(0, len(pred)):
        if pred[i] == gt[i]:
            P += 1
    return P / len(pred)


def cifar_10_naivebayes_learn(Xp,Y):
    output = [[], [], [], [], [], [], [], [], [], []]
    classified_class_list = [], [], [], [], [], [], [], [], [], []
    for i in range(len(Xp)):
        classified_class_list[Y[i]].append(Xp[i])
    for i in range(10):
        p = len(classified_class_list[i]) / len(Xp[:, 0])
        rgb = np.array(classified_class_list[i])
        mu_r = np.mean(rgb[:, 0])
        mu_g = np.mean(rgb[:, 1])
        mu_b = np.mean(rgb[:, 2])
        variance_r = np.var(rgb[:, 0])
        variance_g = np.var(rgb[:, 1])
        variance_b = np.var(rgb[:, 2])
        output[i] = [mu_r, mu_g, mu_b, variance_r, variance_g, variance_b, p]

    return output

def cifar10_color(X):
    x = resize(X,(1, 1))
    x = x[0][0]
    return x

def cifar10_nXn_color(X,n):
    x = np.zeros((X.shape[0], 3 * n ** 2))
    for i in range(0, X.shape[0]):
        img = X[i].reshape(3, 32, 32).transpose(1, 2, 0)
        resized = resize(img, (n, n))
        x[i] = resized.flatten()
    return x


def cifar10_classifier_naivebayes(x, mu, sigma2, p):
    class_probability = np.zeros(10)
    for i in range(0, 10):
        N_red = 1 / np.sqrt(2 * np.pi * sigma2[i, 0]) * np.exp(-(x[0] - mu[i, 0]) ** 2 / (2 * sigma2[i, 0]))
        N_green = 1 / np.sqrt(2 * np.pi * sigma2[i, 1]) * np.exp(-(x[1] - mu[i, 1]) ** 2 / (2 * sigma2[i, 1]))
        N_blue = 1 / np.sqrt(2 * np.pi * sigma2[i, 2]) * np.exp(-(x[2] - mu[i, 2]) ** 2 / (2 * sigma2[i, 2]))
        class_probability[i] = N_red * N_green * N_blue * p[i]

    return np.argmax(class_probability)


def cifar10_bayes_learn(Xf, Y):
    output = [[], [], [], [], [], [], [], [], [], []]
    cov = [[], [], [], [], [], [], [], [], [], []]
    classified_class_list = [], [], [], [], [], [], [], [], [], []
    p = [[], [], [], [], [], [], [], [], [], []]

    for i in range(Xf.shape[0]):
        classified_class_list[Y[i]].append(Xf[i])
    for i in range(10):

        p[i] = len(classified_class_list[i]) / len(Xf[0])
        rgb = np.array(classified_class_list[i])
        mu_temp = np.zeros(rgb.shape[1])
        for dimension in range(0, rgb.shape[1]):
            mu_temp[dimension] = np.mean(rgb[:, dimension])
        cov[i] = np.cov(rgb.T)
        output[i] = mu_temp

    return output, cov, p


def cifar10_classifier_bayes(x, mu, sigma, p):
    class_probability = np.zeros(10)

    for i in range(0, 10):
        N = 1 / (2 * np.pi * np.sqrt(np.linalg.det(sigma[i]))) * np.exp(-0.5 * (x - mu[i]).T @ np.linalg.pinv(sigma[i]) @ (x - mu[i]))
        class_probability[i] = N * p[i]

    return np.argmax(class_probability)


def matrixdist(a,b):
    d = np.linalg.norm(a - b)
    return d

def cifar10_classifier_1nn(x,trdata,trlabels):
    testdata = x["data"]
    testlabels = x["labels"]
    testlabels = np.array(testlabels)
    k = len(testdata)
    n = len(trdata)
    c = 11
    hits = 0
    for i in range(k):
        baseline = matrixdist(testdata[i], trdata[0])
        for j in range(n):
            dist = matrixdist(testdata[i],trdata[j])
            if dist <= baseline:
                baseline = dist
                c = trlabels[j]
        if class_acc(c,testlabels[i]):
            hits += 1
            c = 11
    return hits/k

def cifar10_classifier_random(x):
    rand = randint(0,9)
    if class_acc(rand,x):
        return True
    else:
        return False

def unpickle(file):
    with open(file, 'rb') as f:
        dict = pickle.load(f, encoding="latin1")
    return dict

datadict = unpickle('C:/Users/lauri/Desktop/Softat/PRMLkoodit/CIFAR-10/cifar-10-batches-py/data_batch_1')
testdict = unpickle('C:/Users/lauri/Desktop/Softat/PRMLkoodit/CIFAR-10/cifar-10-batches-py/test_batch')
X = datadict["data"]
Y = datadict["labels"]
D2C =unpickle('C:/Users/lauri/Desktop/Softat/PRMLkoodit/CIFAR-10/cifar-10-batches-py/data_batch_2')
D3C = unpickle('C:/Users/lauri/Desktop/Softat/PRMLkoodit/CIFAR-10/cifar-10-batches-py/data_batch_3')
D4C = unpickle('C:/Users/lauri/Desktop/Softat/PRMLkoodit/CIFAR-10/cifar-10-batches-py/data_batch_4')
D5C = unpickle('C:/Users/lauri/Desktop/Softat/PRMLkoodit/CIFAR-10/cifar-10-batches-py/data_batch_5')
D2 = D2C["data"]
D3 = D3C["data"]
D4 = D4C["data"]
D5 = D5C["data"]
D2L = D2C["labels"]
D3L = D3C["labels"]
D4L = D4C["labels"]
D5L = D5C["labels"]
labeldict = unpickle('C:/Users/lauri/Desktop/Softat/PRMLkoodit/CIFAR-10/cifar-10-batches-py/batches.meta')
label_names = labeldict["label_names"]
CIFAR_10_CLASS = Y
CIFAR_10 = X
CIFAR_10 = np.concatenate((CIFAR_10,D2))
CIFAR_10 = np.concatenate((CIFAR_10,D3))
CIFAR_10 = np.concatenate((CIFAR_10,D4))
CIFAR_10 = np.concatenate((CIFAR_10,D5))
CIFAR_10_CLASS = np.concatenate((CIFAR_10_CLASS,D2L))
CIFAR_10_CLASS = np.concatenate((CIFAR_10_CLASS,D3L))
CIFAR_10_CLASS = np.concatenate((CIFAR_10_CLASS,D4L))
CIFAR_10_CLASS = np.concatenate((CIFAR_10_CLASS,D5L))
Y = np.array(Y)
X = CIFAR_10
Y = CIFAR_10_CLASS
X = X.reshape(50000, 3, 32, 32).transpose(0,2,3,1).astype("uint8")



resized = np.zeros((X.shape[0], 3))

for i in range(len(X)):
    img = cifar10_color(X[i])
    resized[i] = img
    print(f'{(i/49999)*100}% done transforming images')

dist_param = cifar_10_naivebayes_learn(resized, Y)
dist_param = np.array(dist_param)

n=1000
#you can use n value 0-9999, i use 1000 to speed up the program and get a "good enough" evaluation

pred_naive = np.zeros(n)
pred_bayes = np.zeros(n)
pred_2x2 = np.zeros(n)
pred_4x4 = np.zeros(n)
pred_8x8 = np.zeros(n)




img_2x2 = cifar10_nXn_color(D5[:n],2)
img_4x4 = cifar10_nXn_color(D5[:n],4)
img_8x8 = cifar10_nXn_color(D5[:n],8)


Learned_values, B_cov, B_P = cifar10_bayes_learn(resized,Y)
Learned_values2, B_cov2, B_P2 = cifar10_bayes_learn(img_2x2,D5L)
Learned_values4, B_cov4, B_P4 = cifar10_bayes_learn(img_4x4,D5L)
Learned_values8, B_cov8, B_P8 = cifar10_bayes_learn(img_8x8,D5L)


for i in range(n):
    pred_naive[i] = cifar10_classifier_naivebayes(resized[i], dist_param[:, 0:3], dist_param[:, 3:6], dist_param[:, 6])
    pred_bayes[i] = cifar10_classifier_bayes(resized[i],Learned_values,B_cov,B_P)
    pred_2x2[i] = cifar10_classifier_bayes(img_2x2[i], Learned_values2,B_cov2,B_P2)
    pred_4x4[i] = cifar10_classifier_bayes(img_4x4[i], Learned_values4,B_cov4,B_P4)
    pred_8x8[i] = cifar10_classifier_bayes(img_8x8[i], Learned_values8,B_cov8,B_P8)
    #results dip noticeably at 8x8 and even more later due to limitations in numpy




print(f'The naive Bayesian classifier is {class_acc(pred_naive, Y)*100}% accurate')
print(f'The Bayesian classifier is {class_acc(pred_bayes, Y)*100}% accurate')
print(f'The 2x2 Bayesian classifier is {class_acc(pred_2x2, D5L)*100}% accurate')
print(f'The 4x4 Bayesian classifier is {class_acc(pred_4x4, D5L)*100}% accurate')
print(f'The 8x8 Bayesian classifier is {class_acc(pred_8x8, D5L)*100}% accurate')




data = {'naive':class_acc(pred_naive, Y)*100,'bayes 1x1':class_acc(pred_bayes, Y)*100,
        '2x2':class_acc(pred_2x2, D5L)*100, '4x4': class_acc(pred_4x4, D5L)*100,'8x8':class_acc(pred_8x8, D5L)*100,
        }
method = list(data.keys())
P = list(data.values())

fig = plt.figure(figsize=(10,5))

plt.bar(method,P, color = 'purple',
        width=0.5)
plt.xlabel("used method")
plt.ylabel("P")
plt.title("Cifar-10 dataset evaluation using Bayesian learning")
plt.show()