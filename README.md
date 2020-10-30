In this folder, you'll copy over the provided unit tests from part 1 and modify them to 
test the functions again in part 2 (make sure you update interfaces and behaviors
accordingly). 

You'll create some (at least 4) of your own unit tests in a new file test/myUnitTests.c to 
check that the suite is working properly for a small case of your own design. 


You'll write your own python script in plotTimeSeries.py to read the results of those suites, 
and plot a collection of all the resulting time series in a figure you'll save (one
figure per scheduling scheme). All 3 scheduling schemes should give you the same results.

You'll create your own python script in plotMeanMax.py to read the mean and max 
values for the static scheduling scheme, then plot the results of each in
a histogram https://numpy.org/doc/stable/reference/generated/numpy.histogram.html.
Save the figures in results/meanDist.png and results/maxDist.png

You may (optionally) wish to create a python script to plot the timing results of your scalability
test. If so, save that in this folder under the name test/scalabilityPlots.py. 
But it is also acceptable to do this using Excel, Pages, or any other plotting program. 