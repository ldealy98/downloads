---
title: "Writeup"
author: "Lauren Dealy"
output: html_document
---

## Writeup question #1

Parallel vectors are created and filled in this implementation by first calling allocate_Vector from vector.c which allocates space for the vector. Then it is initialized to all zeros by initialize_Vector. It computes the length and starting values of the local arrays by compute_subarray_size in vector.c which assigns extra entries to the beginning rows if the number of global entries does not divide evenly among the processes. Using the starting index and the number of local entries it then fills the arrays with linearly increasing values. After this it puts the data into the padded arrays and shares the ghost regions. If you had 13 entries and 4 processes the the 0th rank process would have 4 entries with the starting index being 0. It would have entries 0, 1, 2, 3. The local padded array size would be 2k + 4. The 1st rank process would have 3 entries with the starting index being 4. It would have entries 4, 5, 6. The local padded array size would be 2k + 3. The 2nd rank process would have 3 entries with the starting index being 7. It would have entries 7, 8, 9. The local padded array size would be 2k + 3. The 3rd and final process would have 3 entries with the starting index being 10. It would have entries 10, 11, 12. The local padded array size would be 2k + 3. 

## Writeup question #2
The initial setup with 41 entries on 2 processes would be that the 0th rank process would have 21 entries starting at index 0 and they would be 0-20. The 1st rank process would have 20 entries starting at index 20 and they would be 21-40. I would expect the result after averaging to be the same number for most numbers, but not the numbers towards the beginning and end because their averages would be messed up by the 0s in the padding. 