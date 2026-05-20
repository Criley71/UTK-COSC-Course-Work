# Compile
`cd src`

`make`

or if make fails

```
g++ -Wall -std=c++17 -g -Wno-sign-compare -c naive.cpp -o naive.o
g++ -Wall -std=c++17 -g -Wno-sign-compare -o naive naive.o
```

# Running the script
The program expects 3 command line arguments, 2 input matrices and 1 output file.

using `make run` will take a.txt, b.txt and c.txt as its 2 inputs and 1 output respectively. 

other wise use `./naive matrix1.txt matrix2.txt out.txt`