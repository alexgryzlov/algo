#include <iostream>

int abs(int value) {
    if (value >= 0)
        return value;
    else
        return -value;
}

class Heap {
public:
    Heap() {
        data = new int[4]; size = 4; current = -1, max_size = 0;
    }
    ~Heap() {
        delete[] data;
    }

    void add(int value);
    void del();
    int get_max_size() { return max_size; }
    int get_top() { return data[0]; }
    bool isEmpty() { if (current == -1) return true; else return false; }

private:
    void sift_up(int i);
    void sift_down(int i);
    int* data;
    int size;
    int current;
    int max_size;
};

void Heap::del() {
    if (current == -1) {
        return;
    }
    if (current + 1 > size / 4) {
        data[0] = data[current];
        sift_down(0);
        current--;
    }
    else {
        int *temp = new int[size / 2];
        for (int i = 0; i < current + 1; i++) {
            temp[i] = data[i];
        }
        delete[] data;
        data = temp;
        size /= 2;
        data[0] = data[current];
        sift_down(0);
        current--;
        return;
    }
}

void Heap::add(int value) {
    if (current + 1 < size) {
        data[++current] = value;
        if (current + 1 > max_size)
            max_size = current + 1;
        sift_up(current);
    }
    else {
        int *temp = new int[size * 2];
        for (int i = 0; i < size; i++) {
            temp[i] = data[i];
        }
        delete[] data;
        data = temp;
        data[++current] = value;
        size *= 2;
        if (current + 1 > max_size)
            max_size = current + 1;
        sift_up(current);
        return;
    }
}

void Heap::sift_up(int i) {
    while (i > 0) {
        if (abs(data[i]) > abs(data[(i - 1) / 2]))
            return;
        std::swap(data[i], data[(i - 1) / 2]);
        i = (i - 1) / 2;
    }
}

void Heap::sift_down(int i) {
    while (true) {
        int left = 2 * i + 1;
        int right = 2 * i + 2;
        int smallest = i;
        if (left <= current && abs(data[left]) <= abs(data[smallest]))
            smallest = left;
        if (right <= current && abs(data[right]) <= abs(data[smallest]))
            smallest = right;
        if (smallest == i)
            return;
        else {
            std::swap(data[i], data[smallest]);
            i = smallest;
        }
    }
}
