#include <iostream>

template <class T>
T& GetMedian(T& a, T& b, T& c) {
    if (a <= b) {
        if (b <= c) {
            return b;
        }
        else {
            if (a <= c) {
                return c;
            }
            else {
                return a;
            }
        }
    }
    else {
        if (b <= c) {
            if (a <= c) {
                return a;
            }
            else {
                return c;
            }
        }
        else {
            return b;
        }
    }
}

template <class T>
void swap(T &a, T &b) {
    T temp;
    temp = b;
    b = a;
    a = temp;
}

template <class T>
int Partition(T* a, int start, int end) {
    if (start == end) {
        return start;
    }
    swap(GetMedian(a[start], a[end], a[(start + end) / 2]), a[end]);
    int i = start, j = start;
    while (j < end) {
        if (a[j] <= a[end]) {
            swap(a[i], a[j]);
            ++j; ++i;
        }
        else {
            ++j;
        }
    }
    swap(a[i], a[end]);
    return i;
}
template <class T>
void InsertionSort(T* a, int start, int end) {
    for (int i = start; i < end + 1; i++) {
        T tmp = a[i];
        int j = i - 1;
        for (; tmp < a[j] && j >= start; j--) {
            a[j + 1] = a[j];
        }
        a[j + 1] = tmp;
    }
}

template <class T>
void QuickSort(T* a, int start, int end) {
    if (end - start <= 1000) {
        InsertionSort(a, start, end);
        return;
    }
    if (start < end) {
        int mid = Partition(a, start, end);
        QuickSort(a, start, mid - 1);
        QuickSort(a, mid + 1, end);
    }
}