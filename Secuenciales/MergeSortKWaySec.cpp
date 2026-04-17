#include <vector>
#include <limits>
#include <algorithm>

// Merge K-way complejidad total O(nk*logk(n))
void mergeK(std::vector<int>& A, int l, int r, int k) {
    int n = r - l + 1;
    std::vector<int> temp(n);
    
    // Calcular limites de subarreglos
    int partSize = n / k;
    // Elementos excedentes
    int extra = n % k;
    
    // Arreglos que guardan los indices de inicio y fin de cada subarreglo
    std::vector<int> starts(k);
    std::vector<int> ends(k);
    int currentStart = l;
    
    // Asignar los indices de inicio y fin para cada subarreglo
    for (int i = 0; i < k; i++) {
        starts[i] = currentStart;
        int currentEnd = currentStart + partSize - 1;
        if (i < extra) currentEnd++;
        ends[i] = currentEnd;
        currentStart = currentEnd + 1;
    }

    // Arreglo auxiliar que mantiene los indices de los subarreglos
    std::vector<int> idx = starts;

    // Merge kway lineal
    for (int j = 0; j < n; j++) {
        int minVal = std::numeric_limits<int>::max();
        int minIdx = -1;

        // Encontrar el mínimo entre los k subarreglos
        for (int i = 0; i < k; i++) {
            if (idx[i] <= ends[i]) {
                if (A[idx[i]] < minVal) {
                    minVal = A[idx[i]];
                    minIdx = i;
                }
            }
        }
        
        // Agregar el mínimo al arreglo temporal
        temp[j] = minVal;
        idx[minIdx]++;
    }

    // Copiar
    std::copy(temp.begin(), temp.end(), A.begin() + l);
}

void kWayMergeSort(std::vector<int>& A, int l, int r, int k) {

    // Definir largo, tamaño del subarreglo y elementos excedentes
    int n = r - l + 1;
    int size = n / k;
    int extra = n % k;
    int start = l;

    // Divide en k partes
    for (int i = 0; i < k; i++) {
        int end = start + size - 1;
        if (i < extra) end++;

        if (start < end) {
            kWayMergeSort(A, start, end, k);
        }
        start = end + 1;
    }

    // Merge k-way
    mergeK(A, l, r, k);
}