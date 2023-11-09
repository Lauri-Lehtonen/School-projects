import pickle
import numpy as np
import matplotlib.pyplot as plt
from random import random,randint

def matrixdist(a,b):
    #uses numpys linear algebra norm to calculate distance, could implement it completely from scratch but this was better
    #optitimized
    d = np.linalg.norm(a - b)
    return d




def cifar10_classifier_1nn(x,trdata,trlabels):
    testdata = x["data"]
    testlabels = x["labels"]
    testlabels = np.array(testlabels)
    k = len(testdata)
    #parameter k will define how many iterations of test data the 1NN classifier will go through, len(testlabels) for the whole dataset
    n = len(trdata)
    #parameter n will define how many iterations of training data the 1NN clasifier will go through, len(trdata) for the whole dataset
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

def class_acc(pred,gt):
    if pred == gt:
        return True
    else:
        return False

def unpickle(file):
    with open(file, 'rb') as f:
        dict = pickle.load(f, encoding="latin1")
    return dict




datadict = unpickle('C:/Users/lauri/Desktop/Softat/PRMLkoodit/CIFAR-10/cifar-10-batches-py/data_batch_1')
testdict = unpickle('C:/Users/lauri/Desktop/Softat/PRMLkoodit/CIFAR-10/cifar-10-batches-py/test_batch')
D2C = unpickle('C:/Users/lauri/Desktop/Softat/PRMLkoodit/CIFAR-10/cifar-10-batches-py/data_batch_2')
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


X = datadict["data"]
Y = datadict["labels"]

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


N1 = 0
N2 = 0
N3 = cifar10_classifier_1nn(testdict,CIFAR_10,CIFAR_10_CLASS)
I = 0

loop = 0



labeldict = unpickle('C:/Users/lauri/Desktop/Softat/PRMLkoodit/CIFAR-10/cifar-10-batches-py/batches.meta')
label_names = labeldict["label_names"]


Y = np.array(Y)

for i in range(X.shape[0]):
    # Show some images randomly
    if loop < 10000:
        loop += 1
        I += 1
        if class_acc(Y[i], Y[i]):
            N1 += 1
        if cifar10_classifier_random(Y[i]):
            N2 += 1



Acc_1 = N1/I
Acc_rand = N2/I
print(f'the class predicted accuracy is {Acc_1}')
print(f'the class random accuracy is {Acc_rand}')
print(f'the class 1NN accuracy is {N3}')
