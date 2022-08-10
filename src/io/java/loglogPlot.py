#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Algorithms and Complexity                               August 08, 2022
IST4310
Prof. M Diaz-Maldonado

Synopsis:
Plots the average number of comparisons and the average elapsed time
as a function of the input size for the count duplicates algorithm.
The figure shows that the average time grows quadratically with the input
size in accordance with the expectations.


Copyright (c) 2022 Misael Diaz-Maldonado
This file is released under the GNU General Public License as published
by the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.


References:
[0] JJ McConnell, Analysis of Algorithms, 2nd edition
[1] TH Cormen, CE Leiserson, RL Rivest, C Stein, Introduction to
    Algorithms, 4th edition
[2] R Johansson, Numerical Python: Scientific Computing and Data
    Science Applications with NumPy, SciPy, and Matplotlib, 2nd edition
"""

# imports the needed methods from numerical python and matplotlib library
from numpy import loadtxt
from matplotlib import pyplot as plt

""" imports the experimental data """

# unpacks the input size, the number of comparisons, and the elapsed time
size, comps, etime = loadtxt("times.txt").T

""" visualization """

# closes all files and enables interactive plotting
plt.close('all')
plt.ion()
# creates a figure and a set of axes
fig, ax = plt.subplots()

# computes scaling constants to improve presentation
c1 = (size**2).mean() / comps.mean()
c2 = (size**2).mean() / etime.mean()
# plots the expected (theoretical) time as a function of size
ax.loglog(size, size**2,    color='black', linestyle='--',
          label='quadratic')
# plots the elapsed time as a function of size
ax.loglog(size, c2 * etime, color='black',  linestyle='', marker='o',
          markersize=12, label='elapsed-time')
# plots the number of comparisons as a function of size
ax.loglog(size, c1 * comps, color='red',   linestyle='', marker='*',
          markersize=12, label='comparisons')
ax.set_xlabel('input size, N')
ax.set_ylabel('time, t')
ax.set_title('average case')
ax.legend()

# exports the figure in PNG with a resolution of 600 DPI
fig.savefig("average.png", dpi=600)
