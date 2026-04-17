#include <iostream>
#include <vector>


// Merge Sort secuencial complejidad O(n*logn)
// Funcion para fusionar dos subarreglos
void merge(std::vector<int>& A, int l, int mid, int r) {
    int n = r - l + 1;
    std::vector<int> temp(n);
    
    // Indices para los subarreglos
    int i = l;
    int j = mid + 1;
    int k = 0;

    // Fusionar los subarreglos en temp
    while (i <= mid && j <= r) {
        if (A[i] <= A[j]) {
            temp[k++] = A[i++];
        } else {
            temp[k++] = A[j++];
        }
    }

    // Copiar los elementos restantes si existen
    while (i <= mid) {
        temp[k++] = A[i++];
    }

    // Copiar los elementos restantes si existen
    while (j <= r) {
        temp[k++] = A[j++];
    }

    // Copiar el arreglo temporal de vuelta a A
    for (int m = 0; m < temp.size(); m++) {
        A[l + m] = temp[m];
    }
}

void mergeSort(std::vector<int>& A, int l, int r) {
    // Si el subarreglo tiene mas de un elemento dividirlo
    if (l < r) {
        int mid = l + (r - l) / 2;
        mergeSort(A, l, mid);
        mergeSort(A, mid + 1, r);
        merge(A, l, mid, r);
    }
}
