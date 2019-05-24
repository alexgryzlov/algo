/*
  Дано число N и последовательность из N целых чисел.
  Найти вторую порядковую статистику на заданных диапазонах.
*/

#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>
#include <cassert>

using std::endl;
using std::cin;
using std::cout;
using std::vector;
using std::pair;
using std::make_pair;
using std::min;
using std::max;

struct Info {
    int min1_value, min2_value, min1_index, min2_index;
    Info(int a, int b, int c, int d) : min1_value(a), min1_index(b), min2_value(c), min2_index(d) {}
    Info() {}
};

std::ostream& operator<< (std::ostream &out, const Info& info) {
    out << "((" << info.min1_value << ", " << info.min1_index << "), (" << info.min2_value << ", " << info.min2_index << "))";
    return out;
}

class SparseTable {
public:
    SparseTable(const vector<int>& arr);
    int get_second_min(int left_bound, int right_bound) const;
    void print_table() const;
private:
    vector<vector<Info>> data;
};

SparseTable::SparseTable(const vector<int>& arr) {
    assert(arr.size() > 0);

    // Making our table the right size
    int max_power_of_2 = floor(log2(arr.size()));
    data.resize(max_power_of_2 + 1);
    for (int i = 0; i < max_power_of_2 + 1; ++i) {
        data[i].resize(arr.size() - pow(2, i) + 1);
    }

    // Initialize the first level
    for (int j = 0; j < data[0].size(); ++j) {
        data[0][j] = Info(arr[j], j, -1, -1);
    }

    // This case is special, because on the 0-th level there are no second min values
    // We want to avoid this check in our dynamical fill
    if (arr.size() > 1) {
        for (int j = 0; j < data[1].size(); ++j) {
            int min1_value, min2_value, min1_index, min2_index;
            if (data[0][j].min1_value < data[0][j + pow(2, 0)].min1_value) {
                min1_value = data[0][j].min1_value;
                min1_index = j;
                min2_value = data[0][j + pow(2, 0)].min1_value;
                min2_index = j + pow(2, 0);
            }
            else {
                min2_value = data[0][j].min1_value;
                min2_index = j;
                min1_value = data[0][j + pow(2, 0)].min1_value;
                min1_index = j + pow(2, 0);
            }
            data[1][j] = Info(min1_value, min1_index, min2_value, min2_index);
        }
    }
    // Dynamically fill the rest of the table
    for (size_t i = 2; i < data.size(); ++i) {
        for (size_t j = 0; j < data[i].size(); ++j) {
            int min1_value, min2_value, min1_index, min2_index;
            if (data[i - 1][j].min1_value < data[i - 1][j + pow(2, i - 1)].min1_value) {
                min1_value = data[i - 1][j].min1_value;
                min1_index = data[i - 1][j].min1_index;
                if (data[i - 1][j].min2_value < data[i - 1][j + pow(2, i - 1)].min1_value) {
                    min2_value = data[i - 1][j].min2_value;
                    min2_index = data[i - 1][j].min2_index;
                }
                else {
                    min2_value = data[i - 1][j + pow(2, i - 1)].min1_value;
                    min2_index = data[i - 1][j + pow(2, i - 1)].min1_index;
                }
            }
            else {
                min1_value = data[i - 1][j + pow(2, i - 1)].min1_value;
                min1_index = data[i - 1][j + pow(2, i - 1)].min1_index;
                if (data[i - 1][j + pow(2, i - 1)].min2_value < data[i - 1][j].min1_value) {
                    min2_value = data[i - 1][j + pow(2, i - 1)].min2_value;
                    min2_index = data[i - 1][j + pow(2, i - 1)].min2_index;
                }
                else {
                    min2_value = data[i - 1][j].min1_value;
                    min2_index = data[i - 1][j].min1_index;
                }
            }
            data[i][j] = Info(min1_value, min1_index, min2_value, min2_index);
        }
    }
}

int SparseTable::get_second_min(int left_bound, int right_bound) const {
    assert(right_bound >= left_bound);

    int n = floor(log2(right_bound - left_bound + 1));
    int k = (right_bound - left_bound + 1) - pow(2, n);
    if (k == 0) {
        return data[n][left_bound - 1].min2_value;
    }
    if (data[n][left_bound - 1].min1_value < data[n][left_bound - 1 + k].min1_value &&
        data[n][left_bound - 1].min1_index != data[n][left_bound - 1 + k].min1_index) {
        return min(data[n][left_bound - 1 + k].min1_value, data[n][left_bound - 1].min2_value);
    }
    if ((data[n][left_bound - 1].min1_value >= data[n][left_bound - 1 + k].min1_value &&
         data[n][left_bound - 1].min1_index != data[n][left_bound - 1 + k].min1_index)) {
        return min(data[n][left_bound - 1].min1_value, data[n][left_bound - 1 + k].min2_value);
    }
    if (data[n][left_bound - 1].min1_index == data[n][left_bound - 1 + k].min1_index) {
        return min(data[n][left_bound - 1 + k].min2_value, data[n][left_bound - 1].min2_value);
    }
}

void SparseTable::print_table() const {
    for (size_t i = 0; i < data.size(); ++i) {
        for (size_t j = 0; j < data[i].size(); ++j) {
            std::cout << data[i][j] << " ---- ";
        }
        std::cout << endl;
    }
}

int main() {
    int n, m; std::cin >> n >> m;
    vector<int> sequence(n);
    for (int i = 0; i < n; ++i) {
        int current_number; cin >> current_number;
        sequence[i] = current_number;
    }

    SparseTable sparse_table(sequence);

    for (int i = 0; i < m; ++i) {
        int left_bound, right_bound; cin >> left_bound >> right_bound;
        cout << sparse_table.get_second_min(left_bound, right_bound) << endl;
    }

    return 0;
}