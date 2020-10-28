import numpy as np
import matplotlib.pyplot as plt
plt.switch_backend('agg')

# example of how to make a 2D numpy array with 2 rows, 3 columns
#a = np.array([[4.2,-1,5.57],[0.1,-4,-10]])
#print(a[1,2])

filename = 'output.txt'
# documentation for loadtxt: https://numpy.org/doc/stable/reference/generated/numpy.loadtxt.html
data = np.loadtxt(filename, dtype=np.int, skiprows=1)
print("size of data: "+str(data.size))
print("shape of data: "+str(data.shape))

# grab each column of the data
arr = data[:,0]
sqr = data[:,1]

# make a plot
plt.plot(arr,sqr)
plt.title('squared consective integers',fontsize=16)
plt.xlabel('consecutive integers')
plt.ylabel('squared integers')
plt.savefig('squares.png')

