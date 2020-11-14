Lauren Dealy
Writeup

Writeup question # 1:

A main difference in the creation of the serial vectors is that in the initialization of serial vectors in the provided code for lab 6 pads the vector with zeroes if it is not evenly distributed amongst the processes. The creation of parallel vectors is pretty much identical from lab 6. 

Writeup question #2:
a. I used MPI_Reduce and MPI_Bcast
b. I used MPI_Reduce because in the directions, it said we had to calculate the total sum of the individual sums, so a reduce would be appropriate to sum up the smaller sums. The directions said to return this sum to the root process, so I knew to only use Reduce instead of an All Reduce. I then used a broadcast because the directions said to make the total sum a float and then send it to every other process so a broadcast would make sense. 
c. They need to be a little different, for instance I only called the broadcast on the root because it is the only process that i want sharing its totalNorm value to others. If all the processes did this broadcast it wouldn't be my desired output. 

Writeup question #4:
I tried to run the given tests but kept getting errors of "undefined reference to serialNorm". I got these for every function already defined. I checked to make sure the header files were included, and I didn't edit the tests at all and I could not figure out how to fix this. 