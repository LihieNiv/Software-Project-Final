import numpy as np
import sys
import symnmfmod as sym_mod

np.random.seed(1234)


def ddg(data, k, n) -> list[list]:
    return sym_mod.ddg(data, n)


def sym(data, k, n) -> list[list]:
    return sym_mod.sym(data, n)


def norm(data, k, n) -> list[list]:
    return sym_mod.norm(data, n)


def symnmf(data, k, n) -> list[list]:
    W = norm(data, n)
    mean_w = np.average(W)
    H = np.random.uniform(low=0, high=2 * np.sqrt(mean_w / k), shape=(n, k))
    # Check if H can be a ndarray or not, API wise
    return sym_mod.symnmf(H, W, n, k)


def print_mat(mat) -> None:
    assert isinstance(mat, list[list])
    for line in mat:
        new_line = ["%.4f" % item for item in line]
        print(",".join(new_line))


if __name__ == "__main__":
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
        n += 1
        while line != "":
            line = line.split(",")
            try:
                line = [float(num) for num in line]
            except Exception:
                print("Bad file content.")
                exit(1)
            data.append(line)
            line = file.readline()
    res = func(data, k, n)
    print_mat(res)
