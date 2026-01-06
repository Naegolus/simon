#!/usr/bin/env python3

import argparse
import numpy as np
import matplotlib.pyplot as plt

np.set_printoptions(formatter = {'float': '{:.3f}'.format})

x1 = np.array([1.0, 0.0])
A = np.array([[0.14, 0.67],
              [0.86, 0.33]])

nDef = 1

def matrixPower(n):

	print(f"x1:")
	print(x1)

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

	xn = R @ x1
	print(f"xn = R * x1:")
	print(xn)

	return xn

def arrowPrint(pStart, pEnd, color, lbl):

	dpX = pEnd[0] - pStart[0]
	dpY = pEnd[1] - pStart[1]

	lenHead = 0.14
	lenArrow = np.sqrt(dpX**2 + dpY**2)

	if lenArrow < lenHead:
		return

	if lenArrow < lenHead:
		lenHead = 0.0

	kHead = (lenArrow - lenHead) / lenArrow

	plt.arrow(pStart[0], pStart[1], dpX * kHead, dpY * kHead,
		head_width = 0.5 * lenHead, head_length = lenHead,
		fc = 'white', ec = color, linewidth = 2, label = lbl)


def matrixPrint(n):
	cCntMesh = 20
	x = np.linspace(-3, 3, cCntMesh)
	y = np.linspace(-3, 3, cCntMesh)
	X, Y = np.meshgrid(x, y)

	R = np.linalg.matrix_power(A, n)

	U = R[0, 0] * X + R[0, 1] * Y
	V = R[1, 0] * X + R[1, 1] * Y

	dU = U - X
	dV = V - Y

	plt.figure(figsize=(10, 10))

	plt.quiver(X, Y, dU, dV, alpha = 0.6)

	plt.title('Vectorfield of R')
	plt.grid(True, alpha = 0.3)
	plt.xlabel('x')
	plt.ylabel('y')
	plt.axis('equal')
	plt.xlim(-3, 3)
	plt.ylim(-3, 3)

	pOrigin = (0, 0)

	eigenvalues, eigenvectors = np.linalg.eig(R)
	for i in range(2):
		ev = eigenvectors[:, i] * 2

		eigval = eigenvalues[i]
		ev = eigenvectors[:, i] * eigval

		arrowPrint(pOrigin, ev, 'green' if eigval > 0 else 'red', f'Eigenvector {i}')

		print(f'eigenvector[{i}] = {ev}')
		print(f'eigenvalue[{i}] = {eigval}')

	arrowPrint(pOrigin, x1, 'grey', 'Start');
	xn = matrixPower(n)
	arrowPrint(pOrigin, xn, 'black', 'End');
	arrowPrint(x1, xn, 'blue', 'Diff');

	plt.legend()
	plt.show()

if __name__ == "__main__":
	parser = argparse.ArgumentParser(description = 'Calculating A^n')
	parser.add_argument('n', type = int, nargs = '?', default = nDef, help = f"Exponent for A^n (default: {nDef})")

	args = parser.parse_args()

	matrixPrint(args.n)

