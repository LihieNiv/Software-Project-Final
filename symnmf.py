import numpy as np
import pandas as pd
import sys
#import symnmf_name as symnmf #change to name
np.random.seed(1234)

def get_full_symnmf():
    return

def get_sym():
    return

def get_ddg():
    return


def get_norm():
    return


def init_H(n, m, k, file_name):
    H = np.random.uniform(0,2*((m/k)**0.5), n*k).reshape(n, k)
    print(H)


def update_H():
    return

def main():
    args = sys.argv
    k = int(args[1])
    goal = int(args[2])
    file_name = args[3]
    data = pd.read_csv(file_name, header=None)
    input = data.to_numpy().tolist()

    init_H(10, 3, 4, "a")
    return

main()
