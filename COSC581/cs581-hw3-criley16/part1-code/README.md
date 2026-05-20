# Compile Methods
**`cd src` first** 
1. use `make` 
2. use if `make` doesn't work
``` 
g++ -Wall -g -c median_of_median.cpp -o median_of_median.o
g++ -Wall -g -o mom median_of_median.o
```
3. if needed `make clean` will delete the executable and `.o` files made from `make`

# Executing
The program expects 2 command line arguments in the format of:

`./med_of_med <value_of_r> <name_of_file>`

`<value_of_r>` is an odd number that determines the size of the the subarrays made in the median of median algorithms

`<name_of_file>` is a .txt file that holds a list of numbers to find the median of where the first line should indicate *n* number of elements followed by *n* lines of integers. Example input file of 4 integers:

```
4
100
35
48
-2
```
The program will return the median of the list

# Benchmarking
**NOTE:** If time.sh is not able to be executed use `chmod +x time.sh`

Using `./time.sh` it will run 5 input files of sizes $10^3$, $10^4$, $10^5$, $10^6$, and $10^7$ where r=3, 5, 7, 9 and 11 and measure the execution time using the input files in the `part4-analysis/inputs` folder. It will write the execution time to a txt file named `benchmark_kap.txt` where `benchmark.txt` are my results used in the analysis