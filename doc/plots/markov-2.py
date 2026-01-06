#!/usr/bin/env python3

import numpy as np
import argparse

np.set_printoptions(formatter = {'float': '{:.3f}'.format})

x = np.zeros((3, 1))

A_slow = np.array([0.40, 0.20, 0.40])
A_fast = np.array([0.30, 0.50, 0.20])

nDef = 1

def matrix_power(n):

	x[0] = 1.00
	print(f"x:")
	print(x)

	A = np.column_stack([A_fast, np.zeros(3), np.zeros(3)])
	A = np.column_stack([A_slow, np.zeros(3), np.zeros(3)])
	A[1][1] = 1
	A[0][2] = 2

	print(f"A:")
	print(A)

	sums = np.sum(A, axis = 0)
	print(f"Sums A:")
	print(f" {sums}")

	R = np.linalg.matrix_power(A, n)
	print(f"R = A^{n}:")
	print(R)

	sums = np.sum(R, axis = 0)
	print(f"Sums R:")
	print(f" {sums}")

	xn = R @ x
	print(f"xn = R * x:")
	print(xn)

	return xn

if __name__ == "__main__":
	parser = argparse.ArgumentParser(description = 'Calculating A^n')
	parser.add_argument('n', type = int, nargs = '?', default = nDef, help = f"Exponent for A^n (default: {nDef})")

	args = parser.parse_args()
	matrix_power(args.n)

