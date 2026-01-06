#!/usr/bin/env python3

import numpy as np
import argparse

np.set_printoptions(formatter = {'float': '{:.3f}'.format})

P = np.array([[0.14, 0.67],
              [0.86, 0.33]])

nDef = 1

def matrix_power(n):
	r = np.linalg.matrix_power(P, n)

	print(f"P:")
	print(P)

	print(f"R = P^{n}:")
	print(r)

	sums = np.sum(r, axis = 0)
	print(f"Sums:")
	print(f" {sums}")

	d = np.diag(r)
	print(f"d = diag(R): ")
	print(f" {d}")

	return r

if __name__ == "__main__":
	parser = argparse.ArgumentParser(description = 'Calculating P^n')
	parser.add_argument('n', type = int, nargs = '?', default = nDef, help = f"Exponent for P^n (default: {nDef})")

	args = parser.parse_args()
	matrix_power(args.n)

