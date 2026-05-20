import numpy as np
import sys

def add(a, b):
    n = len(a)
    c = np.zeros((n,n), dtype=int)
    for i in range(n):
        for j in range(n):
            c[i][j] = a[i][j] + b[i][j]
    return c
def sub(a, b):
    n = len(a)
    c = np.zeros((n,n), dtype=int)
    for i in range(n):
        for j in range(n):
            c[i][j] = a[i][j] - b[i][j]
    return c

def strassen(a, b):
    
    n = len(a)
    if(n <=32 ):
        res = [[0 for _ in range(n)] for _ in range(n)] 
        for i in range(n):
            for j in range(n):
                for k in range(n):
                    res[i][j] += a[i][k] * b[k][j]
        return res
    new_size = n // 2
    a11 = a[:new_size, :new_size]
    a12 = a[:new_size, new_size:]
    a21 = a[new_size:, :new_size]
    a22 = a[new_size:, new_size:]
    
    b11 = b[:new_size, :new_size]
    b12 = b[:new_size, new_size:]
    b21 = b[new_size:, :new_size]
    b22 = b[new_size:, new_size:]
    
    s1 = sub(b12, b22)
    s2 = add(a11, a12)
    s3 = add(a21, a22)
    s4 = sub(b21, b11)
    s5 = add(a11, a22)
    s6 = add(b11, b22)
    s7 = sub(a12, a22)
    s8 = add(b21, b22)
    s9 = sub(a11, a21)
    s10 = add(b11, b12)
    
    p1 = strassen(a11, s1)
    p2 = strassen(s2, b22)
    p3 = strassen(s3, b11)
    p4 = strassen(a22, s4)
    p5 = strassen(s5, s6)
    p6 = strassen(s7, s8)
    p7 = strassen(s9, s10)

    c = np.zeros((n,n), dtype=int)
    c[:new_size, :new_size] = add(sub(add(p5, p4),p2), p6)
    c[:new_size, new_size:] = add(p1, p2)
    c[new_size:, :new_size] = add(p3, p4)
    c[new_size:, new_size:] = sub(sub(add(p5, p1), p3), p7)
    
    return c
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
    
result = strassen(matrix_a, matrix_b)
with open(sys.argv[3], 'w') as f3:
    f3.write(f'{am} {bn}\n')
    np.savetxt(f3, np.array(result), fmt='%d', delimiter=' ')