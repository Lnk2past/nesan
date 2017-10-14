# plots the raw, uncertain, and filtered track data on top of eachother.
# expects Nx2 tables, each row containing a single x-,y-coordinate

import numpy as np
import matplotlib.pyplot as plt

raw = np.genfromtxt('raw_state.dat', unpack=True)
unc = np.genfromtxt('uncertain_state.dat', unpack=True)
fil = np.genfromtxt('filtered_state.dat', unpack=True)

plt.plot(raw[0], raw[1], 'r-', label='Raw')
plt.plot(unc[0], unc[1], 'g-', label='Uncertain')
plt.plot(fil[0], fil[1], 'b-', label='Filtered')

plt.grid()
plt.legend()

plt.axis('equal')

plt.show()