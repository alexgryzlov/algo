/* Дан массив из целых чисел a1, a2, ..., an (индексация с 1!).
 * Для каждого запроса [left, right] найдите такой подотрезок al, al+1, ..., ar этого массива,
 * что сумма чисел al + al+1 + ... + ar является максимально возможной. Требуемое время ответа на запрос - O(log n).
*/
#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <stdlib.h>

using std::ceil;
using std::cin;
using std::cout;
using std::vector;
using std::endl;
using std::max;

int find_max_sum(const vector<int>& arr, int l, int r) {
    vector<int> max_sum;
    max_sum.push_back(0);
    int current_sum = 0;
    for (int i = l; i < r + 1; ++i) {
        current_sum = arr[i] + max_sum.back();
        if (current_sum < 0) current_sum = 0;
        max_sum.push_back(current_sum);
    }
    auto result = max_sum[0];
    for (auto i : max_sum) {
        if (i > result) {
            result = i;
        }
    }
    return result;
}

struct Node {
    int sum, prefix_sum, suffix_sum, sub_sum;
    Node() {}
    Node(int sum, int prefix_sum, int suffix_sum, int sub_sum) : sum(sum), prefix_sum(prefix_sum),
                                                                 suffix_sum(suffix_sum), sub_sum(sub_sum) {}
};

class SegmentTree {
public:
    SegmentTree(vector<int>& arr);
    int find_max_subarray_sum(int l, int r);
private:
    vector<Node> data;
    void build_tree(vector<int>& arr, int i, int l, int r);
    Node send_query(int i, int query_l, int query_r, int node_l, int node_r);
};


int main() {
    while (!cin.eof()) {
        int n, m; cin >> n >> m;
        vector<int> arr(pow(2, ceil(log2(n))), 0);
        for (int i = 0; i < n; ++i) {
            int current_number; cin >> current_number;
            arr[i] = current_number;
        }

        SegmentTree segment_tree(arr);

        for (int i = 0; i < m; ++i) {
            int left_bound, right_bound; cin >> left_bound >> right_bound;
            cout << segment_tree.find_max_subarray_sum(left_bound - 1, right_bound - 1) << endl;
            cout.flush();
        }
    }
    return 0;
}

SegmentTree::SegmentTree(vector<int>& arr) {
    data.resize(arr.size() * 2 - 1);
    build_tree(arr, 0, 0, arr.size());
}

void SegmentTree::build_tree(vector<int>& arr, int i, int l, int r) {
    if (l == r) {
        return;
    }
    if (r - l == 1) {
        data[i] = Node(arr[l], arr[l], arr[l], arr[l]);
    }
    else {
        int m = (l + r) / 2;
        build_tree(arr, 2 * i + 1, l, m);
        build_tree(arr, 2 * i + 2, m, r);
        Node& left_son = data[2 * i + 1];
        Node& right_son = data[2 * i + 2];
        data[i] = Node(left_son.sum + right_son.sum, // total sum
                       max(left_son.sum + right_son.prefix_sum, left_son.prefix_sum), // prefix
                       max(right_son.sum + left_son.suffix_sum, right_son.suffix_sum), // suffix
                       max(right_son.sub_sum, max(left_son.sub_sum, right_son.prefix_sum + left_son.suffix_sum))); // answer
    }
}

int SegmentTree::find_max_subarray_sum(int l, int r) {
    auto a = send_query(0, l, r, 0, (data.size() + 1) / 2 - 1);
    return a.sub_sum;
}

Node SegmentTree::send_query(int i, int query_l, int query_r, int node_l, int node_r) {
    if (node_l == query_l && node_r == query_r) {
        return data[i];
    }
    int m = (node_l + node_r) / 2;
    if (query_r <= m) {
        return send_query(2 * i + 1, query_l, query_r, node_l, m);
    }
    if (query_l > m) {
        return send_query(2 * i + 2, query_l, query_r, m + 1, node_r);
    }
    Node left_son = send_query(2 * i + 1, query_l, m, node_l, m);
    Node right_son = send_query(2 * i + 2, m + 1, query_r, m + 1, node_r);
    Node result = Node(left_son.sum + right_son.sum, // total sum
                       max(left_son.sum + right_son.prefix_sum, left_son.prefix_sum), // prefix
                       max(right_son.sum + left_son.suffix_sum, right_son.suffix_sum), // suffix
                       max(right_son.sub_sum, max(left_son.sub_sum, right_son.prefix_sum + left_son.suffix_sum))); // answer
    return result;
}