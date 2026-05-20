#include <chrono>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <vector>
using namespace std;

void sub_matrix_add(vector<vector<int>> &a, vector<vector<int>> &b, vector<vector<int>> &c, int n) {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            c[i][j] = a[i][j] + b[i][j];
        }
    }
}

void sub_matrix_sub(vector<vector<int>> &a, vector<vector<int>> &b, vector<vector<int>> &c, int n) {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            c[i][j] = a[i][j] - b[i][j];
        }
    }
}

void strassens(vector<vector<int>> &a, vector<vector<int>> &b, vector<vector<int>> &c, int n) {
    if (n <= 32) {
        for (int i = 0; i < n; i++) {
            for (size_t j = 0; j < n; j++) {
                int sum = 0;
                for (size_t k = 0; k < n; k++) {
                    sum += a[i][k] * b[k][j];
                }
                c[i][j] = sum;
            }
        }
        return;
    }
    int sub_size = n / 2;

    vector<vector<int>> a11(sub_size, vector<int>(sub_size, 0));
    vector<vector<int>> a12(sub_size, vector<int>(sub_size, 0));
    vector<vector<int>> a21(sub_size, vector<int>(sub_size, 0));
    vector<vector<int>> a22(sub_size, vector<int>(sub_size, 0));
    vector<vector<int>> b11(sub_size, vector<int>(sub_size, 0));
    vector<vector<int>> b12(sub_size, vector<int>(sub_size, 0));
    vector<vector<int>> b21(sub_size, vector<int>(sub_size, 0));
    vector<vector<int>> b22(sub_size, vector<int>(sub_size, 0));
    vector<vector<int>> c11(sub_size, vector<int>(sub_size, 0));
    vector<vector<int>> c12(sub_size, vector<int>(sub_size, 0));
    vector<vector<int>> c21(sub_size, vector<int>(sub_size, 0));
    vector<vector<int>> c22(sub_size, vector<int>(sub_size, 0));
    vector<vector<int>> s1(sub_size, vector<int>(sub_size, 0));
    vector<vector<int>> s2(sub_size, vector<int>(sub_size, 0));
    vector<vector<int>> s3(sub_size, vector<int>(sub_size, 0));
    vector<vector<int>> s4(sub_size, vector<int>(sub_size, 0));
    vector<vector<int>> s5(sub_size, vector<int>(sub_size, 0));
    vector<vector<int>> s6(sub_size, vector<int>(sub_size, 0));
    vector<vector<int>> s7(sub_size, vector<int>(sub_size, 0));
    vector<vector<int>> s8(sub_size, vector<int>(sub_size, 0));
    vector<vector<int>> s9(sub_size, vector<int>(sub_size, 0));
    vector<vector<int>> s10(sub_size, vector<int>(sub_size, 0));
    vector<vector<int>> p1(sub_size, vector<int>(sub_size, 0));
    vector<vector<int>> p2(sub_size, vector<int>(sub_size, 0));
    vector<vector<int>> p3(sub_size, vector<int>(sub_size, 0));
    vector<vector<int>> p4(sub_size, vector<int>(sub_size, 0));
    vector<vector<int>> p5(sub_size, vector<int>(sub_size, 0));
    vector<vector<int>> p6(sub_size, vector<int>(sub_size, 0));
    vector<vector<int>> p7(sub_size, vector<int>(sub_size, 0));
    vector<vector<int>> temp_a(sub_size, vector<int>(sub_size, 0));
    vector<vector<int>> temp_b(sub_size, vector<int>(sub_size, 0));
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
    sub_matrix_sub(b12, b22, s1, sub_size);
    sub_matrix_add(a11, a12, s2, sub_size);
    sub_matrix_add(a21, a22, s3, sub_size);
    sub_matrix_sub(b21, b11, s4, sub_size);
    sub_matrix_add(a11, a22, s5, sub_size);
    sub_matrix_add(b11, b22, s6, sub_size);
    sub_matrix_sub(a12, a22, s7, sub_size);
    sub_matrix_add(b21, b22, s8, sub_size);
    sub_matrix_sub(a11, a21, s9, sub_size);
    sub_matrix_add(b11, b12, s10, sub_size);

    strassens(a11, s1, p1, sub_size);
    strassens(s2, b22, p2, sub_size);
    strassens(s3, b11, p3, sub_size);
    strassens(a22, s4, p4, sub_size);
    strassens(s5, s6, p5, sub_size);
    strassens(s7, s8, p6, sub_size);
    strassens(s9, s10, p7, sub_size);

    sub_matrix_add(p5, p4, temp_a, sub_size);
    sub_matrix_add(temp_a, p6, temp_b, sub_size);
    sub_matrix_sub(temp_b, p2, c11, sub_size);

    sub_matrix_add(p1, p2, c12, sub_size);
    sub_matrix_add(p3, p4, c21, sub_size);

    sub_matrix_add(p5, p1, temp_a, sub_size);
    sub_matrix_sub(temp_a, p3, temp_b, sub_size);
    sub_matrix_sub(temp_b, p7, c22, sub_size);

    for (int i = 0; i < sub_size; i++) {
        for (int j = 0; j < sub_size; j++) {
            c[i][j] = c11[i][j];
            c[i][j + sub_size] = c12[i][j];
            c[i + sub_size][j] = c21[i][j];
            c[i + sub_size][j + sub_size] = c22[i][j];
        }
    }
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
    vector<vector<int>> c(n, vector<int>(n, 0));
    
    strassens(matrix_a, matrix_b, c, c.size());
    ofstream fout(argv[3]);
    fout << c.size() << " " << c[0].size() << "\n";
    for (size_t i = 0; i < c.size(); i++) {
        for (size_t j = 0; j < c[0].size(); j++) {
            fout << c[i][j] << " ";
        }
        fout << "\n";
    }
    fout.close();
}