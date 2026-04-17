#include <vector>
#include <limits>
#include <algorithm>

// Merge K-way complejidad O(nk*logk(n))
void mergeK(std::vector<int>& A, int l, int r, int k) {
    int n = r - l + 1;
    std::vector<int> temp(n);
    
    // Calcular limites de subarreglos
    int partSize = n / k;
    int extra = n % k;
    
    std::vector<int> starts(k);
    std::vector<int> ends(k);
    int currentStart = l;
    
    for (int i = 0; i < k; i++) {
        starts[i] = currentStart;
        int currentEnd = currentStart + partSize - 1;
        if (i < extra) currentEnd++;
        ends[i] = currentEnd;
        currentStart = currentEnd + 1;
    }

    std::vector<int> idx = starts;

    // Merge kway lineal
    for (int j = 0; j < n; j++) {
        int minVal = std::numeric_limits<int>::max();
        int minIdx = -1;

        for (int i = 0; i < k; i++) {
            if (idx[i] <= ends[i]) {
                if (A[idx[i]] < minVal) {
                    minVal = A[idx[i]];
                    minIdx = i;
                }
            }
        }
        
        temp[j] = minVal;
        idx[minIdx]++;
    }

    // Copiar de vuelta
    std::copy(temp.begin(), temp.end(), A.begin() + l);
}

void kWayMergeSort(std::vector<int>& A, int l, int r, int k) {
    int n = r - l + 1;
    int size = n / k;
    int extra = n % k;
    int start = l;

    for (int i = 0; i < k; i++) {
        int end = start + size - 1;
        if (i < extra) end++;

        if (start < end) {
            kWayMergeSort(A, start, end, k);
        }
        start = end + 1;
    }

    mergeK(A, l, r, k);
}