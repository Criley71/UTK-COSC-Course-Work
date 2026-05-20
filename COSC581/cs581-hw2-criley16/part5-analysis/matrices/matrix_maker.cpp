#include <fstream>
#include <iostream>
#include <random>
#include <vector>
#include <filesystem>
using namespace std;
int main(int argc, char *argv[]) {
    srand(time(nullptr));
    int n;
    if (argc != 3) {
        perror("Usage: ./mm size_n_of_nxn_matrix set_number");
        exit(1);
    } else {
        n = stoi(argv[1]);
        if ((n & (n - 1)) != 0) {
            perror("n must be a power of 2");
            exit(1);
        }
    }
    string set = "set"+string(argv[2]);
    string file_a = set + "/a.txt"; 
    ofstream fout(file_a);

    fout << n << " " << n << "\n";
    vector<vector<int>> matrix(n, vector<int>(n, 0));
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            matrix[i][j] = (rand() % 101);
        }
    }
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            fout << matrix[i][j] << " ";
        }
        fout << "\n";
    }
    fout.close();
    string file_b = set + "/b.txt"; 
    ofstream fout2(file_b);

    fout2 << n << " " << n << "\n";
    //vector<vector<int>> matrix(n, vector<int>(n, 0));
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            matrix[i][j] = (rand() % 101);
        }
    }
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            fout2 << matrix[i][j] << " ";
        }
        fout2 << "\n";
    }
}