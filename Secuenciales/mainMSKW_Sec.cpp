#include <iostream>
#include <vector>
#include <fstream>
#include <chrono>
#include <cstdlib>
#include <ctime>

// Algoritmo
void kWayMergeSort(std::vector<int>& A, int l, int r, int k);

// Verificar orden
bool isSorted(const std::vector<int>& A) {
    for (int i = 1; i < (int)A.size(); i++) {
        if (A[i - 1] > A[i]) return false;
    }
    return true;
}

// Medir tiempo
long long runKWay(std::vector<int> A, int k, bool &ok) {

    auto start = std::chrono::high_resolution_clock::now();
    kWayMergeSort(A, 0, (int)A.size() - 1, k);
    auto end = std::chrono::high_resolution_clock::now();

    ok = isSorted(A);

    return std::chrono::duration_cast<std::chrono::milliseconds>(
        end - start
    ).count();
}

// Main
int main() {

    srand(time(NULL));

    std::ofstream file("resultadosKWay_Sec.csv");

    file << "n,k,repeticiones,tiempo_promedio_ms,proporcion_ordenado\n";

    std::vector<int> sizes = {
        1 << 20,
        1 << 22,
        1 << 24,
        1 << 26
    };

    std::vector<int> kValues = {
        4, 8, 16
    };

    int reps = 5;

    for (int n : sizes) {

        for (int k : kValues) {

            long long totalTime = 0;
            int correctCount = 0;

            for (int r = 0; r < reps; r++) {

                std::vector<int> A(n);

                for (int i = 0; i < n; i++) {
                    A[i] = rand();
                }

                bool ok = false;

                long long t = runKWay(A, k, ok);

                totalTime += t;

                if (ok) {
                    correctCount++;
                }
            }

            double avgTime = (double)totalTime / reps;
            double ratioSorted = (double)correctCount / reps;

            file << n << ","
                 << k << ","
                 << reps << ","
                 << avgTime << ","
                 << ratioSorted << "\n";

            std::cout << "n=" << n
                      << " k=" << k
                      << " avg=" << avgTime
                      << " ms sorted_ratio=" << ratioSorted
                      << "\n";
        }
    }

    file.close();

    return 0;
}