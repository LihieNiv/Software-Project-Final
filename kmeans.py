# import math supposed to not need it
import sys

eps = 0.001
cent = []


def euc_dist(x, y):
    sm = 0
    for i in range(len(x)):
        sm += (x[i] - y[i]) ** 2
    return float(sm**0.5)


def closest(x):
    if len(cent) == 0:
        raise ValueError("An Error Has Occurred")
    mn = euc_dist(x, cent[0])
    index = 0
    for i in range(len(cent)):
        dst = euc_dist(x, cent[i])
        if dst < mn:
            mn = dst
            index = i
    return index


def avg_vector(arr):
    if len(arr) == 0:
        raise ValueError("An Error Has Occurred")
    vec = [0 for i in range(len(arr[0]))]
    for x in arr:
        for i in range(len(vec)):
            vec[i] += x[i] / len(arr)
    for i in range(len(vec)):
        vec[i] = float(vec[i])
    return vec


def kMeans(arr, K, iter=200):
    global cent
    cent = arr[0:K]
    while iter > 0:
        cent_arr = [[] for i in range(K)]
        for x in arr:
            cent_arr[closest(x)].append(x)
        cent_new = [avg_vector(arr) for arr in cent_arr]
        flag = False
        for i in range(K):
            if euc_dist(cent_new[i], cent[i]) >= eps:
                flag = True
        if not flag:
            break
        cent = cent_new
        iter -= 1
    if iter == 0:
        print("stopped bc of iter!")
    Str = ""
    for mu in cent:
        str = ""
        for num in mu:
            str += "%0.4f" % (num)
            str += ","
        print(str[0 : len(str) - 1])
        Str += str[0 : len(str) - 1] + "\n"
    return Str


def get_array(file_path):
    arr = []
    file = open(file_path, "r")
    for line in file.readlines():
        line = line.split(",")
        x = [float(num) for num in line]
        arr.append(x)
    return arr


args = sys.argv
K = int(args[1])
path = args[len(args) - 1]
if len(args) == 3:
    iter = 200
else:
    iter = int(args[2])
if len(path) < 4:
    raise AssertionError("An Error Has Occurred")
if path[len(path) - 4 : len(path)] != ".txt":
    raise AssertionError("invalid input file")
open(path)
arr = get_array(path)
N = len(arr)
assert (1 < K and K < N) and int(K) == K, "Invalid number of clusters!"
assert int(iter) == iter and (1 < iter and iter < 1000), "Invalid maximum iteration!"
kMeans(arr, K, iter)
