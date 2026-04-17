#include <iostream>
#include <vector>


// Merge Sort secuencial complejidad O(n*logn)
// Funcion para fusionar dos subarreglos
void merge(std::vector<int>& A, int l, int mid, int r) {
    int n = r - l + 1;
    std::vector<int> temp(n);
    
    int i = l;
    int j = mid + 1;
    int k = 0;

    while (i <= mid && j <= r) {
        if (A[i] <= A[j]) {
            temp[k++] = A[i++];
        } else {
            temp[k++] = A[j++];
        }
    }

    while (i <= mid) {
        temp[k++] = A[i++];
    }

    while (j <= r) {
        temp[k++] = A[j++];
    }

    for (int m = 0; m < temp.size(); m++) {
        A[l + m] = temp[m];
    }
}

void mergeSort(std::vector<int>& A, int l, int r) {
    if (l < r) {
        int mid = l + (r - l) / 2;
        mergeSort(A, l, mid);
        mergeSort(A, mid + 1, r);
        merge(A, l, mid, r);
    }
}
