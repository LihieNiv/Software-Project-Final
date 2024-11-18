import kmeans
import symnmf
from sklearn.metrics import silhouette_score as sil_score
import sys


def choose_cluster_symnmf(sym_H: list[list[float]], k: int):
    labels = [-1 for i in len(sym_H)]
    for i in range(len(sym_H)):
        mx = 0
        ind = 0
        for j in range(k):
            if sym_H[i][j] > mx:
                mx = sym_H[i][j]
                ind = j
        labels[i] = ind
    return labels


def euc_dist(a: list[float], b: list[float]):
    if len(a) != len(b):
        print("Centroid and data point dont have the same length")
        return -1
    return sum([(a[i] - b[i]) ** 2 for i in range(len(a))])


def find_cluster(point: list[float], centroids: list[list[float]]):
    mn = euc_dist(point, centroids[0])
    ind = 0
    for i in range(len(centroids)):
        dist = euc_dist(point, centroids[i])
        if dist < mn:
            ind = i
            mn = dist
    return ind


def choose_cluster_kmeans(centroids: list[list[float]], data: list[list[float]]):
    labels = [find_cluster(point, centroids) for point in data]
    return labels


if __name__ == "__main__":
    args = sys.argv
    k = args[1]
    path = args[2]
    (n, data) = symnmf.parse_data(path)
    sym_H = symnmf.symnmf(data, k, n)
    kmeans_cent = [item.split(",") for item in kmeans.kMeans(data, k).split("\n")]
    sym_labels = choose_cluster_symnmf(sym_H, k)
    kmeans_labels = choose_cluster_kmeans(kmeans_cent, data)
    sym_score = sil_score(data, sym_labels)
    kmeans_score = sil_score(data, kmeans_labels)
    print("nmf: %.4f", sym_score)
    print("kmeans: %.4f", kmeans_score)
