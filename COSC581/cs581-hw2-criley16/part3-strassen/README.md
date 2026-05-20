# Compile
`cd src`

`make`

or if make fails

```
g++ -Wall -std=c++17 -g -Wno-sign-compare -c strassen.cpp -o strassen.o
g++ -Wall -std=c++17 -g -Wno-sign-compare -o strassen strassen.o
```

# Running the script
The program expects 3 command line arguments, 2 input matrices and 1 output file.

using `make run` will take a.txt, b.txt and c.txt as its 2 inputs and 1 output respectively. 

other wise use `./strassen matrix1.txt matrix2.txt out.txt`

due to the overhead required in allocating the memory required for each sub-matrix this is a hybrid Stressen algorithm that at a threshold of n <= 32 it invokes the naive method of multiplying matrices 