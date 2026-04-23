#include <iostream>
#include <vector>
#include <fstream>
#include <chrono>
#include <cstdlib>
#include <ctime>
#include <omp.h> // Indispensable para controlar los hilos

void KWayMergeSort_Par(std::vector<int>& A, int l, int r, int k, bool useParallelMerge);

bool isSorted(const std::vector<int>& A) {
    for (int i = 1; i < (int)A.size(); i++) {
        if (A[i - 1] > A[i]) return false;
    }
    return true;
}

// RunKWay recibir p threads
long long runKWay(std::vector<int>& A, int k, int p, bool &ok) {
    // Configuramos el número de hilos para OpenMP
    omp_set_num_threads(p);
    
    auto start = std::chrono::high_resolution_clock::now();
    KWayMergeSort_Par(A, 0, (int)A.size() - 1, k, true); 
    auto end = std::chrono::high_resolution_clock::now();
    ok = isSorted(A);

    return std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
}

int main() {
    srand(time(NULL));
    std::ofstream file("resultadosKWay_Completo.csv");

    // Agregamos la columna 'p' (procesadores)
    file << "n,k,p,repeticiones,tiempo_promedio_ms,proporcion_ordenado\n";

    std::vector<int> sizes = { 1 << 20, 1 << 22, 1 << 24, 1 << 26 };
    std::vector<int> kValues = { 4, 8, 16, 32, 64, 128 };
    std::vector<int> pValues = { 1, 2, 4, 8 };
    int reps = 5;

    for (int n : sizes) {
        for (int k : kValues) {
            for (int p : pValues) {
                long long totalTime = 0;
                int correctCount = 0;

                std::cout << "Iniciando: n=" << n << " k=" << k << " p=" << p << "..." << std::endl;

                for (int r = 0; r < reps; r++) {
                    std::vector<int> A(n);
                    for (int i = 0; i < n; i++) A[i] = rand();

                    bool ok = false;
                    totalTime += runKWay(A, k, p, ok);
                    if (ok) correctCount++;
                }

                double avgTime = (double)totalTime / reps;
                double ratioSorted = (double)correctCount / reps;

                file << n << "," << k << "," << p << "," << reps << "," 
                     << avgTime << "," << ratioSorted << "\n";
            }
        }
    }

    file.close();
    return 0;
}