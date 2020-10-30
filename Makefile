# ==============ADD YOUR RULES FOR YOUR UNIT TESTS HERE==================
# so that the clean rule below works, name your unit test executable(s) obj/unitTestSuite

oldTest:
	# ****your rules here to build and run all updated unit tests from part 1****

newTest:
	# ****your rules here to build and run your new unit tests*****

test:
	make oldTest
	make newTest

# =================END OF YOUR RULES FOR YOUR UNIT TESTS================


buildPointSim:
	gcc test/pointSim.c code/material.c code/checkPt.c code/simulation.c code/suite.c -o obj/pointSim -lm -fopenmp

# examples of how to run pointSim with 4 temp values and 3 boundary values for each scheduling mode
runPointSimStatic:
	./obj/pointSim 4 3 0

runPointSimDynamic:
	./obj/pointSim 4 3 1
	
runPointSimGuided:
	./obj/pointSim 4 3 2

# if you're having memory issues, use valgrind (load proper modules first)
valTest:
	gcc -g test/pointSim.c code/material.c code/checkPt.c code/simulation.c code/suite.c -o obj/pointSim -lm -fopenmp
	valgrind ./obj/pointSim 4 3 0


# run your visualization of output results that are a collection of time series at the same point under
# different initial conditions that all have 50 time steps
plotTimeSeries:
	python test/plotTimeSeries.py 0 50 
	python test/plotTimeSeries.py 1 50
	python test/plotTimeSeries.py 2 50

plotScalability:
	python test/plotScalability.py

// =======================ADD YOUR RULE FOR PLOTTING MAX/MEAN HERE========================

// ======================END OF YOUR RULE=================================================

clean:
	rm -f obj/unitTest* 
	rm -f obj/pointSim
	rm -f results/*.txt
	rm -f results/*.png
