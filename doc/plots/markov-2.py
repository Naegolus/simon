#!/usr/bin/env python3

import numpy as np
import argparse

np.set_printoptions(formatter = {'float': '{:.3f}'.format})

p_slow = np.array([0.40, 0.20, 0.40])
p_fast = np.array([0.30, 0.50, 0.20])

nDef = 1

def matrix_power(n):

	P = np.column_stack([p_fast, np.zeros(3), np.zeros(3)])
	P = np.column_stack([p_slow, np.zeros(3), np.zeros(3)])
	P[1][1] = 1
	P[0][2] = 2

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

