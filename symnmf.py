import numpy as np
import pandas as pd
import sys
import symnmfmod as sym_mod
np.random.seed(1234)

def ddg(data, n) -> list[list]:
    return sym_mod.ddg(data, n)


def sym(data, n) -> list[list]:
    return sym_mod.sym(data, n)


def norm(data, n) -> list[list]:
    return sym_mod.norm(data, n)


def init_H(n, m, k, file_name):
    h = np.random.uniform(0, 2*((m/k)**0.5), n*k).reshape(n, k)
    print(h)


def update_H():
    return


def symnmf(data, k, n) -> list[list]:
    W = sym_mod.norm(data, n)
    m = np.average(W)
    H = np.random.uniform(0, 2*((m/k)**0.5), n*k).reshape(n, k).tolist()
    return sym_mod.symnmf(H, W, n, k)


def print_mat(mat) -> None:
    assert isinstance(mat, list)
    for row in mat:
        assert isinstance(row, list)
    for line in mat:
        new_line = ["%.4f" % item for item in line]
        print(",".join(new_line))


def main():
    args = sys.argv
    k = int(args[1])
    goal = args[2]
    file_name = args[3]
    data = pd.read_csv(file_name, header=None)
    #n = data.shape[0] #need to check if not empty?
    input = data.to_numpy().tolist()

    goals = {"symnmf": symnmf, "ddg": ddg, "norm": norm, "sym": sym}
    args = sys.argv
    k = int(args[1])
    goal = args[2]
    file_path = args[3]
    assert file_path[-4:] == ".txt", "bad file type, should be .txt"
    if goal not in goals:
        raise ValueError("Illegal goal value. Should be: symnmf/ddg/norm/sym.")
    func = goals[goal]
    data = []
    n = 0
    with open(file_path, "r") as file:
        line = file.readline()
        while line != "":
            n += 1
            line = line.split(",")
            try:
                line = [float(num) for num in line]
            except Exception:
                print("Bad file content.")
                exit(1)
            data.append(line)
            line = file.readline()
    if goal == "symnmf":
        res = func(data, k, n)
    else:
        res = func(data, n)
    print_mat(res)

    return

if __name__ == "__main__":
    main()
