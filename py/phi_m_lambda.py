import numpy as np
import matplotlib.pyplot as plt

fs = 10

name = "phi_m_lambda"
fname = "data/" + name + ".txt"
dataf = open(fname, "r")
ms = [float(x) for x in dataf.readline().split()]
lambdas = [float(x) for x in dataf.readline().split()]
dataf.close()
dts = ["r"]
for m in ms:
	for lambda_ in lambdas:
		dts.append(r"$%g,%g$" % (m, lambda_))
dt = [(d, "f") for d in dts]
data = np.loadtxt(fname, dtype = dt, skiprows = 2)

for i in range(1, len(dts)):
	d = dts[i]
	plt.plot(data["r"], data[d] / abs(data[d][0]), label = d)

plt.xlabel(r"$r$")
plt.ylabel(r"$\phi/|\phi(0)|$")

v = list(plt.axis())
v[0] = data["r"][0]
v[1] = data["r"][-1]
v[3] = 0
plt.axis(v)

s = r"$r_s=gn=1$"
plt.text(0.01, 0.99, s, ha = "left", va = "top", transform = plt.gca().transAxes, fontsize = fs)

s = r"$m,\lambda$"
plt.legend(loc = 4, title = s, fontsize = fs, title_fontsize = fs)

plt.savefig("fig/" + name + ".pdf")
