import numpy as np
import sys
def multiply(a, b, am, an, bn):
    res = [[0 for _ in range(bn)] for _ in range(am)] 
    for i in range(am):
        for j in range(bn):
            for k in range(an):
                res[i][j] += a[i][k] * b[k][j]
    return res

am = 0
an = 0
with open(sys.argv[1]) as f1:
    am, an = [int(x) for x in next(f1).split()]
matrix_a = np.loadtxt(sys.argv[1], skiprows=1, dtype=int)

bm = 0
bn = 0
with open(sys.argv[2]) as f2:
    bm, bn = [int(x) for x in next(f2).split()] 
matrix_b = np.loadtxt(sys.argv[2], skiprows=1, dtype=int)

if an != bm:
    print("Matrix product couldn't be performed")
    exit(1)

result = multiply(matrix_a, matrix_b, am, an, bn)
with open(sys.argv[3], 'w') as f3:
    f3.write(f'{am} {bn}\n')
    np.savetxt(f3, np.array(result), fmt='%d', delimiter=' ')
