#include <iostream>

template <class T>
void Merge(T* a, int left, int mid, int right) {
    T* temp = new T[right - left];
    int pnt1 = left, pnt2 = mid;
    int cnt = 0;
    while (pnt1 < mid && pnt2 < right) {
        if (a[pnt1] <= a[pnt2]) {
            temp[pnt1 + pnt2 - left - mid] = a[pnt1];
            ++pnt1;
        }
        else {
            temp[pnt1 + pnt2 - left - mid] = a[pnt2];
            ++pnt2;
        }
    }
    if (pnt1 != mid) {
        for (int i = pnt1; i < mid; i++) {
            temp[pnt1 + pnt2 - left - mid] = a[pnt1];
            ++pnt1;
        }
    }

    if (pnt2 != right) {
        for (int i = pnt2; i < right; i++) {
            temp[pnt1 + pnt2 - left - mid] = a[pnt2];
            ++pnt2;
        }
    }

    for (int i = left; i < right; i++) {
        a[i] = temp[i - left];
    }

    delete[] temp;
}

template <class T>
void MergeSort(T* a, int start, int end) {
    if (start + 1 >= end) {
        return;
    }
    int middle = (start + end) / 2;
    MergeSort(a, start, middle);
    MergeSort(a, middle, end);
    Merge(a, start, middle, end);
}