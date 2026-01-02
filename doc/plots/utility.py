#!/usr/bin/env python3

import numpy as np
import matplotlib.pyplot as plt

def V(x, g, y, l):
	return np.sqrt(x) + np.sqrt(g) + np.sqrt(y) + np.sqrt(l)

# x .. Public goods
# g .. Private goods
# y .. Returns from economic activity
# l .. Leisure

r = 0.4
l = np.linspace(0.0, 1.0, 500)

e = 1 - l
untaxed = 1 - r

y = untaxed * e
Vi = V(0.0, 0.0, y, l)

l_star = 1.0 / (2.0 - r)
l_star = np.full(len(l), l_star)
Vi_star = V(0.0, 0.0, untaxed * (1 - l_star), l_star)

plt.plot(l, l,  label = "Leisure l(r)")
plt.plot(l, l_star,  label = "Leisure l*(r)")
plt.plot(l, e,  label = "Effort e(r)")
plt.plot(l, y,  label = "Returns y(r)")
plt.plot(l, Vi, label = "Utility Vi(r)")
plt.plot(l, Vi_star, label = "Utility Vi*(r)")

plt.title("Optimum leisure")
plt.xlabel("Leisure l")
plt.legend()
plt.grid(True)
plt.show()

