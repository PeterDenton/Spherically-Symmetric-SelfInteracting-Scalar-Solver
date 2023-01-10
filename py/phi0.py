import matplotlib.pyplot as plt
import numpy as np
from scipy.optimize import curve_fit

colors = plt.rcParams["axes.prop_cycle"].by_key()["color"]
fs = 10 # fontsize

# set up the physics parameters
r_s = 1
gn = 1

# read in from file
name = "phi0"
fname = "data/" + name + ".txt"
dataf = open(fname, "r")
ms = [float(x) for x in dataf.readline().split()]
dataf.close()
dts = ["lambda"]
for m in ms:
	dts.append("phi0_%g" % m)
dt = [(d, "g") for d in dts]
data = np.loadtxt(fname, dtype = dt, skiprows = 1)

fig, axs = plt.subplots(2, sharex = True, gridspec_kw = {"height_ratios": [2, 1]})
fig.subplots_adjust(hspace = 0)

# the guess function for phi0, draws m in from global variable
def phi0_Guess(lambda_, alpha):
	global m
	if m == 0:
		low_lambda_limit = r_s ** 2 * gn * 0.5
	else:
		low_lambda_limit = gn * (1 - np.exp(-m * r_s) * (1 + m * r_s)) / m ** 2
	high_lambda_limit_coefficient = (6 * gn) ** (1. / 3)
	return -1. / (high_lambda_limit_coefficient ** -alpha * lambda_ ** (alpha / 3) + low_lambda_limit ** -alpha) ** (1. / alpha) # 1/(1/f1+1/f2)

# calculate the optimal value for alpha over the given range
popts = np.empty(len(ms))
pcovs = np.empty(len(ms))
for i in range(len(ms)):
	m = ms[i]
	popts[i], pcovs[i] = curve_fit(phi0_Guess, data["lambda"], data[dts[i + 1]])
	print("m=", ms[i], "optimal value for alpha=", popts[i])

#############
# top panel #
#############
for i in range(len(ms)):
	m = ms[i]
	d = dts[i + 1]
	c = colors[i % len(colors)]
	axs[0].plot(data["lambda"], abs(data[d]), label = r"$m=%g$" % m, c = c, ls = "-")
	axs[0].plot(data["lambda"], abs(phi0_Guess(data["lambda"], 2)), c = c, ls = "--")

axs[0].plot([1], [10], "k-", label = r"${\rm Exact\ (numerical)}$")
axs[0].plot([1], [10], "k--", label = r"${\rm Guess\ (eq.~38)}$")

axs[0].set_xscale("log")
axs[0].set_yscale("log")

v = list(axs[0].axis())
v[0] = data["lambda"][0]
v[1] = data["lambda"][-1]
v[2] = 3e-2
v[3] = 1e0
axs[0].axis(v)

axs[0].set_ylabel(r"$|\phi(0)|$")

s = r"$r_s=gn=1$"
s += "\n"
s += r"$\alpha=2$"
axs[0].text(0.01, 0.99, s, ha = "left", va = "top", transform = axs[0].transAxes, fontsize = fs)

s = r"$m$"
s = ""
axs[0].legend(loc = 1, title = s, fontsize = fs, title_fontsize = fs, ncol = 1, markerfirst = False)

################
# bottom panel #
################
alpha = 2.
for i in range(len(ms)):
	m = ms[i]
	d = dts[i + 1]
	c = colors[i % len(colors)]
	axs[1].plot(data["lambda"], abs(data[d] - phi0_Guess(data["lambda"], alpha)) / abs(data[d]), c = c, ls = "-", zorder = -10) # plot the fractional error at alpha=2
#	axs[1].plot(data["lambda"], abs(data[d] - phi0_Guess(data["lambda"], popts[i])) / abs(data[d]), c = c, ls = ":") # plot the fractional error with the given optimal value of alpha

axs[1].set_yscale("log")

v = list(axs[1].axis())
v[0] = data["lambda"][0]
v[1] = data["lambda"][-1]
v[2] = 1e-3
v[3] = 1e-1
axs[1].axis(v)

axs[1].set_xlabel(r"$\lambda$")
axs[1].set_ylabel(r"$|\Delta \phi(0)/\phi(0)|$")

axs[1].set_xticks(10. ** np.arange(int(np.log10(v[0])), int(np.log10(v[1])) + 1))

plt.savefig("fig/" + name + ".pdf")

