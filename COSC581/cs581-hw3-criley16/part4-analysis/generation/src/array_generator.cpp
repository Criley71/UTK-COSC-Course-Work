#include <fstream>
#include <iostream>
#include <random>
#include <math.h>
#include <limits>
using namespace std;

int main(int argc, char* argv[]){
    random_device rd;
    mt19937_64 gen(rd());
    const int min_val = -pow(10, 9);
    const int max_val = pow(10, 9);

    if(argc > 3 || argc == 1){
        cout << "Usage: ./array_gen n_magitude_of_10 output_file";
        exit(1);
    }
    string out_file =  "array.txt";
    if(argc == 3){
        out_file = argv[2];
    }else{
        cout << "No outfile given, will output to 'array.txt'\n";
    }
    int n = atoi(argv[1]);
    int size = pow(10, n);
    
    ofstream fout(out_file);
    fout << size << "\n";

    for (int i = 0; i < size; i++){
        uniform_int_distribution<int> dist(min_val, max_val);
        fout << dist(gen) << "\n";
    }
    fout.close();
    return 0;
}