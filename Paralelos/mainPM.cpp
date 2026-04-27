#include <omp.h>
#include <ctime>
#include <vector>
#include <chrono>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <algorithm>

std::vector<int> ParallelMerge(std::vector<int>& A, std::vector<int>& B);

bool isSorted(const std::vector<int>& A) {
    for (int i = 1; i < (int)A.size(); i++) {
        if (A[i - 1] > A[i]) return false;
    }
    return true;
}

long long runPM(std::vector<int>& A, std::vector<int>& B, int p, bool &ok) {
    omp_set_num_threads(p);

    auto start = std::chrono::high_resolution_clock::now();
    std::vector<int> C = ParallelMerge(A, B);
    auto end = std::chrono::high_resolution_clock::now();
    ok = isSorted(C);

    return std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
}

int main() {
    srand(time(NULL));
    std::ofstream file("resultadosParallelMerge.csv");

    file << "n,dummy,p,repeticiones,tiempo_promedio_ms,proporcion_ordenado\n";

    std::vector<int> sizes = { 1 << 20, 1 << 22, 1 << 24, 1 << 26 };
    std::vector<int> pValues = { 1, 2, 4, 8 };
    int reps = 5;

    for (int n : sizes) {
        for (int d = 0; d < 6; d++) {
            for (int p : pValues) {
                long long totalTime = 0;
                int correctCount = 0;

                std::cout << "Iniciando: n=" << n << " d=" << d << " p=" << p << std::endl;

                for (int r = 0; r < reps; r++) {
                    std::vector<int> A(n), B(n);
                    for (int i = 0; i < n; i++) {
                        A[i] = rand();
                        B[i] = rand();
                    }

                    std::sort(A.begin(), A.end());
                    std::sort(B.begin(), B.end());
                    bool ok = false;
                    totalTime += runPM(A, B, p, ok);
                    if (ok) correctCount++;
                }

                double avgTime = (double)totalTime / reps;
                double ratioSorted = (double)correctCount / reps;

                file << n << "," << d << "," << p << "," << reps << ","
                     << avgTime << "," << ratioSorted << "\n";
            }
        }
    }

    file.close();
    return 0;
}