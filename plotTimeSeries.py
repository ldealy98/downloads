import numpy as np
import matplotlib.pyplot as plt
plt.switch_backend('agg')
import sys

# You will write this script to read your results from 
# call this like python plotTimeSeries.py 0 nSims
# using 0 for static results, 1 for dynamic results, and 2 for guided results

parCode = int(sys.argv[1])
parNames = ['static','dynamic','guided']
thisParType = parNames[parCode]
# use this figName variable as the path where you will save the figure with your resulting time series
figName = 'results/'+thisParType+'_timeSeries.png'

# this variable has the number of simulations (so you know what file names to look at) and results are in results/sim_TESTNUMBER_PARCODE.txt
nSims = int(sys.argv[2])

# ===================YOUR CODE HERE======================


# ==================END YOUR CODE=======================
