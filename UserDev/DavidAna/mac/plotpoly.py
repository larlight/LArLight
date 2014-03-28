import numpy as np
import matplotlib.pyplot as plt
import re

fig, ax = plt.subplots()

f = open("polytmp.txt","r")

nlines = 0
for line in f:
    nlines += 1

poly1x = np.empty([nlines/2])
poly1y = np.empty([nlines/2])
poly2x = np.empty([nlines/2])
poly2y = np.empty([nlines/2])

counter  = 0
counter1 = 0
counter2 = 0

f2 = open("polytmp.txt","r")
for line2 in f2:
    values = re.split(r"\t+",line2)
    x = float(values[0])
    y = float(values[1])
    if (counter%2==0):
        poly1x[counter1] = x
        poly1y[counter1] = y
        counter1 += 1
    else:
        poly2x[counter2] = x
        poly2y[counter2] = y
        counter2 += 1
    counter += 1

plt.plot(poly1x,poly1y,"ro-")
plt.plot(poly2x,poly2y,"go-")
plt.show()
