#include <chrono>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <vector>
using namespace std;

vector<vector<int>> sub_matrix_add(const vector<vector<int>> &a, const vector<vector<int>> &b, int n) {
    vector<vector<int>> result(n, vector<int>(n));
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            result[i][j] = a[i][j] + b[i][j];
        }
    }
    return result;
}

vector<vector<int>> matrix_multiply(vector<vector<int>> &a, vector<vector<int>> &b, int n) {

    if (n <= 32) {
        vector<vector<int>> c(n, vector<int>(n));
        for (int i = 0; i < n; i++) {
            for (size_t j = 0; j < n; j++) {
                int sum = 0;
                for (size_t k = 0; k < n; k++) {
                    sum += a[i][k] * b[k][j];
                }
                c[i][j] = sum;
            }
        }
        return c;
    }

    int sub_size = n / 2;
    vector<vector<int>> a11(sub_size, vector<int>(sub_size));
    vector<vector<int>> a12(sub_size, vector<int>(sub_size));
    vector<vector<int>> a21(sub_size, vector<int>(sub_size));
    vector<vector<int>> a22(sub_size, vector<int>(sub_size));
    vector<vector<int>> b11(sub_size, vector<int>(sub_size));
    vector<vector<int>> b12(sub_size, vector<int>(sub_size));
    vector<vector<int>> b21(sub_size, vector<int>(sub_size));
    vector<vector<int>> b22(sub_size, vector<int>(sub_size));

    for (int i = 0; i < sub_size; i++) {
        for (int j = 0; j < sub_size; j++) {
            a11[i][j] = a[i][j];
            a12[i][j] = a[i][j + sub_size];
            a21[i][j] = a[i + sub_size][j];
            a22[i][j] = a[i + sub_size][j + sub_size];

            b11[i][j] = b[i][j];
            b12[i][j] = b[i][j + sub_size];
            b21[i][j] = b[i + sub_size][j];
            b22[i][j] = b[i + sub_size][j + sub_size];
        }
    }

    vector<vector<int>> c11 = sub_matrix_add(matrix_multiply(a11, b11, sub_size), matrix_multiply(a12, b21, sub_size), sub_size);
    vector<vector<int>> c12 = sub_matrix_add(matrix_multiply(a11, b12, sub_size), matrix_multiply(a12, b22, sub_size), sub_size);
    vector<vector<int>> c21 = sub_matrix_add(matrix_multiply(a21, b11, sub_size), matrix_multiply(a22, b21, sub_size), sub_size);
    vector<vector<int>> c22 = sub_matrix_add(matrix_multiply(a21, b12, sub_size), matrix_multiply(a22, b22, sub_size), sub_size);

    vector<vector<int>> c(n, vector<int>(n));
    for (int i = 0; i < sub_size; i++) {
        for (int j = 0; j < sub_size; j++) {
            c[i][j] = c11[i][j];
            c[i][j + sub_size] = c12[i][j];
            c[i + sub_size][j] = c21[i][j];
            c[i + sub_size][j + sub_size] = c22[i][j];
        }
    }
    return c;
}

int main(int argc, char *argv[]) {
    if (argc != 4) {
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
    //int p = matrix_b[0].size();
    int q = matrix_b.size();
    if (m != q) {
        cout << "Matrix product couldn't be performed\n";
        exit(1);
    }
  
    vector<vector<int>> result = matrix_multiply(matrix_a, matrix_b, n);
   
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