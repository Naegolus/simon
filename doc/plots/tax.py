#!/usr/bin/env python3

import numpy as np
import matplotlib.pyplot as plt

def V(x, g, y, l):
	return np.sqrt(x) + np.sqrt(g) + np.sqrt(y) + np.sqrt(l)

# x .. Public goods
# g .. Private goods
# y .. Returns from economic activity
# l .. Leisure

N = 5

r = np.linspace(0.0, 1.0, 200)
l = 1.0 / (2.0 - r)

e = 1 - l
untaxed = 1 - r

y = untaxed * e
Vi = V(0.0, 0.0, y, l)

E = N * e
R = r * E

k = e + l

plt.plot(r, r,  label = "Tax r")
plt.plot(r, l,  label = "Leisure l(r)")
plt.plot(r, e,  label = "Effort e(r)")
plt.plot(r, k,  label = "Lifetime k(r)")
plt.plot(r, y,  label = "Returns y(r)")
plt.plot(r, R,  label = "Revenue R(r)")
plt.plot(r, Vi, label = "Utility Vi(r)")

plt.title("Tax influence")
plt.xlabel("Tax r")
plt.legend()
plt.grid(True)
plt.show()

