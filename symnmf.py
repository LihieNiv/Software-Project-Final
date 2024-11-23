import numpy as np
import sys
import symnmf_mod as sym_mod

np.random.seed(1234)


def ddg(data, n) -> list[list]:
    return sym_mod.py_ddg(data, n)


def sym(data, n) -> list[list]:
    return sym_mod.py_sym(data, n)


def norm(data, n) -> list[list]:
    return sym_mod.py_norm(data, n)


def init_H(n, m, k, file_name):
    h = np.random.uniform(0, 2 * ((m / k) ** 0.5), n * k).reshape(n, k)
    print(h)


def update_H():
    return


def symnmf(data, k, n) -> list[list]:
    W = sym_mod.py_norm(data, n)
    mean_w = np.average(W)
    H = np.random.uniform(low=0, high=2 * np.sqrt(mean_w / k), shape=(n, k))
    # Check if H can be a ndarray or not, API wise
    return sym_mod.py_symnmf(H, W, n, k)


def print_mat(mat) -> None:
    assert isinstance(mat, list[list])
    for line in mat:
        new_line = ["%.4f" % item for item in line]
        print(",".join(new_line))


def main():
    args = sys.argv
    k = int(args[1])
    goal = args[2]
    file_name = args[3]
    data = pd.read_csv(file_name, header=None)
    input = data.to_numpy().tolist()

    n = len(input[0])  # need to check if input not empty?

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

    init_H(10, 3, 4, "a")
    return


if __name__ == "__main__":
    main()
