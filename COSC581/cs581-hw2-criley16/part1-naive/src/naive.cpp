#include <chrono>
#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>
#include <iomanip>
using namespace std;

int main(int argc, char *argv[]) {
    if (argc != 4 ) {
        perror("Usage: ./naive matrix1.txt matrix2.txt out.txt");
        exit(1);
    }
    
    vector<vector<int>> matrix_a;
    ifstream fin(argv[1]);
    if (!fin.good()) {
        perror("matrix1 file error:");
        exit(1);
    }
    bool first_line = true;
    string line;
    while (getline(fin, line)) {
        vector<int> row = {};
        if (first_line) {

            first_line = false;
            continue;
        }
        stringstream ss(line);
        int num;
        while (ss >> num) {
            row.push_back(num);
        }
        matrix_a.push_back(row);
        ss.clear();
    }
    fin.close();
    vector<vector<int>> matrix_b;
    ifstream fin2(argv[2]);
    if (!fin2.good()) {
        perror("matrix2 file error:");
        exit(1);
    }
    first_line = true;
    while (getline(fin2, line)) {
        vector<int> row = {};
        if (first_line) {
            first_line = false;
            continue;
        }
        stringstream ss(line);
        int num;
        while (ss >> num) {
            row.push_back(num);
        }
        matrix_b.push_back(row);
        ss.clear();
    }
    int m = matrix_a[0].size();
    int n = matrix_a.size();
    int q = matrix_b.size();
    int p = matrix_b[0].size();
    if (m != q) {
        cout << "Matrix product couldn't be performed\n";
        exit(1);
    }

    vector<vector<int>> result(n, vector<int>(p, 0));
    for (int i = 0; i < n; i++) {
        for (size_t j = 0; j < p; j++) {
            for (size_t k = 0; k < m; k++) {
                result[i][j] += matrix_a[i][k] * matrix_b[k][j];
            }
        }
    }
 
    ofstream fout(argv[3]);
    fout << result.size() << " " << result[0].size() << "\n";
    for (size_t i = 0; i < result.size(); i++) {
        for (size_t j = 0; j < result[0].size(); j++) {
            fout << result[i][j] << " ";
        }
        fout << "\n";
    }
    fout.close();
}