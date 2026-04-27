#include <omp.h>
#include <cmath>
#include <vector>
#include <algorithm>

/*rank con búsqueda binaria*/
int rank(int x, const std::vector<int>& A) {
    int inicio = 0;
    int fin = A.size();

    while (inicio < fin) {
        int mitad = (inicio + fin)/2;

        if (A[mitad] < x) {
            inicio = mitad + 1;
        }
        else {
            fin = mitad;
        }
    }
    return inicio;
}

std::vector<int> merge(std::vector<int>& A, std::vector<int>& B) {
    int i = 0, j = 0;
    std::vector<int> C;

    while (i < A.size() && j < B.size()) {
        if (A[i] < B[j]) {
            C.push_back(A[i++]);
        }
        else {
            C.push_back(B[j++]);
        }
    }
    while (i < A.size()) {
        C.push_back(A[i++]);
    }
    while (j < B.size()) {
        C.push_back(B[j++]);
    }
    return C;
}

std::vector<int> ParallelMerge(std::vector<int>& A, std::vector<int>& B) {
    if (A.empty()) {
        return B;
    }
    if (B.empty()) {
        return A;
    }

    int n = A.size() + B.size();
    int logn = std::max(1, (int)log2(n));

    if (A.size() <= logn) {
        return merge(A, B);
    }

    int TB = (B.size() + logn - 1)/logn;
    std::vector<int> j(TB + 1);
    j[0] = 0;

    //Pivotes
    for (int i = 1; i < TB; i++) {
        int idx = i*logn;

        if (B.size() <= idx) {
            idx = B.size() - 1;
        }

        j[i] = rank(B[idx], A);
    }

    j[TB] = A.size();
    std::vector<std::vector<int>> resultados(TB);

    #pragma omp parallel for
    for (int i = 0; i < TB; i++) {
        int a_start = j[i];
        int a_end = j[i + 1];
        int b_start = i*logn;
        int b_end = std::min((i + 1)*logn, (int)B.size());
        std::vector<int> Ai(A.begin() + a_start, A.begin() + a_end);
        std::vector<int> Bi(B.begin() + b_start, B.begin() + b_end);

        if (Ai.size() <= logn) {
            resultados[i] = merge(Ai, Bi);
        }
        else {
            resultados[i] = ParallelMerge(Bi, Ai);
        }
    }

    //Concatenar
    std::vector<int> C;

    for (int i = 0; i < resultados.size(); i++) {
        std::vector<int>& r = resultados[i];
        C.insert(C.end(), r.begin(), r.end());
    }
    return C;
}