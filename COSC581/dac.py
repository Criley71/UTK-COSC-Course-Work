import numpy as np
import sys

def square_matrix_multiply_recursive(a, b, n):
    if n == 1:
        c = np.ones((1,1), dtype=int)
        c[0][0] = a[0][0] * b[0][0]
        return c
    new_size = n // 2
    a11 = a[:new_size, :new_size]
    a12 = a[:new_size, new_size:]
    a21 = a[new_size:, :new_size]
    a22 = a[new_size:, new_size:]
    
    b11 = b[:new_size, :new_size]
    b12 = b[:new_size, new_size:]
    b21 = b[new_size:, :new_size]
    b22 = b[new_size:, new_size:]
    
    c11 = square_matrix_multiply_recursive(a11, b11, new_size) + square_matrix_multiply_recursive(a12, b21, new_size)
    c12 = square_matrix_multiply_recursive(a11, b12, new_size) + square_matrix_multiply_recursive(a12, b22, new_size)
    c21 = square_matrix_multiply_recursive(a21, b11, new_size) + square_matrix_multiply_recursive(a22, b21, new_size)
    c22 = square_matrix_multiply_recursive(a21, b12, new_size) + square_matrix_multiply_recursive(a22, b22, new_size)
    
    c = np.zeros((n, n), dtype=int)
    c[:new_size, :new_size] = c11
    c[:new_size, new_size:] = c12
    c[new_size:, :new_size] = c21
    c[new_size:, new_size:] = c22 
    
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
result = square_matrix_multiply_recursive(matrix_a,matrix_b,am)
with open(sys.argv[3], 'w') as f3:
    f3.write(f'{am} {bn}\n')
    np.savetxt(f3, np.array(result), fmt='%d', delimiter=' ')