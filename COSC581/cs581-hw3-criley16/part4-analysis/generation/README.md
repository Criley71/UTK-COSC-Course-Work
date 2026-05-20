# Compile Methods
**`cd src` first** 
1. use `make` 
2. use if `make` doesn't work
``` 
g++ -Wall -g -c array_generator.cpp -o array_generator.o
g++ -Wall -g -o array_gen array_generator.o
```
3. if needed `make clean` will delete the executable and `.o` files made from `make`

# Executing
The program expects 2 command line arguments in the format of:

`./array_gen <n_magnitude_of_10> <output_file.txt>`

`<n_magnitude_of_10>` is an integer that will be used to make the size of the list where the size is equal to $10^n$ so if n=3 then a list of 1000 integers will be made.

`<output_file.txt>` is the file you want the list to be written to.

This will return a file where the first line is the the size of the list and that many elements with one element per line