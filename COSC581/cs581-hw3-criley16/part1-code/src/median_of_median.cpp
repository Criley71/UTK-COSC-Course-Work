#include <algorithm>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <iomanip>
using namespace std;

void insertion_sort(vector<int> &list) {
    for (size_t i = 1; i < list.size(); i++) {
        int val = list[i];
        int j = i - 1;
        while (j >= 0 && list[j] > val) {
            list[j + 1] = list[j];
            j = j - 1;
        }
        list[j + 1] = val;
    }
}

int find_median(vector<int> list, int r) {
    if (static_cast<int>(list.size()) <= r) {
        insertion_sort(list);
        return list[(list.size() / 2)];
    }
    vector<int> medians;
    for (size_t i = 0; i < list.size(); i += r) {
        auto begin = list.begin() + i;
        auto end = list.begin() + min(i + r, list.size());
        vector<int> sub_list(begin, end);
        insertion_sort(sub_list);
        medians.push_back(sub_list[sub_list.size() / 2]);
    }
    return find_median(medians, r);
}

int select(vector<int> list, int k, int r) {
    int pivot = find_median(list, r);
    vector<int> low;
    vector<int> high;
    int equal_to_pivot = 0;

    for (size_t i = 0; i < list.size(); i++) {
        int x = list[i];
        if (x < pivot) {
            low.push_back(x);
        } else if (x > pivot) {
            high.push_back(x);
        } else {
            equal_to_pivot += 1;
        }
    }

    int low_size = low.size();
    if (k < low_size) {
        return select(low, k, r);
    } else if (k < low_size + equal_to_pivot) {
        return pivot;
    } else {
        int new_k = k - (low_size + equal_to_pivot);
        return select(high, new_k, r);
    }
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        cout << "Usage: ./mom <value_of_r> <name_of_file>\n";
        exit(1);
    }
    int r = atoi(argv[1]);
    string file = argv[2];
    ifstream fin(file);
    if (!fin.good()) {
        perror("Input File Error");
        exit(1);
    }
    string line;
    getline(fin, line);
    stringstream ss(line);
    int size;
    ss >> size;
    ss.clear();
    vector<int> list(size);
    int index = 0;
    while (getline(fin, line)) {
        list[index] = stoi(line);
        index += 1;
    }
    double final_median;
    if (size % 2 != 0) {
        int k = size / 2;
        final_median =  select(list, k, r);
    }else {

        int k_high = size / 2;
        int k_low = (size / 2) - 1;
    
        double high_val = select(list, k_high, r);
        double low_val = select(list, k_low, r);
    
        final_median = (high_val + low_val) / 2.0;
    }

    cout << fixed << setprecision(1) << final_median << "\n";
}