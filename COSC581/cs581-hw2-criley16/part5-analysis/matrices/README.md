# Matrix maker
This generates 2 random matrices of a given size, writes them to seperate text files in the desired set
# Compile

`make`

# Run

It expects 2 command line arguments:

* `n` for the n x n matrix dimensions where n is a power of 2
* `set_number` a number 1-5 to save the 2 matrices to.

To execute:

`./mm n set_number`

Example:

`./mm 1024 3` this will make 2 1024x1024 matrices and save them in the set3 folder as `a.txt` and `b.txt`