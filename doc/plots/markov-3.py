#!/usr/bin/env python3

import numpy as np
import argparse

np.set_printoptions(formatter = {'float': '{:.3f}'.format})

x = np.zeros((4, 1))

A = np.array([[0.00, 0.33, 0.50, 0.50],
              [1.00, 0.00, 0.00, 0.50],
              [0.00, 0.33, 0.00, 0.00],
              [0.00, 0.34, 0.50, 0.00]])

nDef = 1

def matrix_power(n):

	x[0] = 1.00
	print(f"x:")
	print(x)

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
	parser = argparse.ArgumentParser(description = 'Calculating P^n')
	parser.add_argument('n', type = int, nargs = '?', default = nDef, help = f"Exponent for P^n (default: {nDef})")

	args = parser.parse_args()
	matrix_power(args.n)

