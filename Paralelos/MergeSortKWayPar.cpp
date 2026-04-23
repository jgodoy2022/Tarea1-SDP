#include <vector>
#include <limits>
#include <algorithm>
#include <omp.h>
#include <cmath>
// Merge K-way Secuencial
void mergeK_Sec(std::vector<int>& A, int l, int r, int k) {
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

//TODO: Arreglar correctitud (no funciona :( )
/**
 * @brief Combina subarreglos ordenados de A usando paralelismo
 * @param A Puntero al arreglo original
 * @param stars Lista de comienzos de subarreglos
 * @param ends  Lista de finales de subarreglos (starts y ends deben tener mismo tamaño)
 * 
 */
void mergeK_Par(std::vector<int>& A, std::vector<int> starts, std::vector<int> ends){
    int k = starts.size();
    int n = ends[k-1] - starts[0] + 1;
    //guardara respuesta
    std::vector<int> temp(n);
    //escoge el intervalo mas grande para subdividir
    std::pair<int,int> largest_interval = {starts[0],ends[0]};
    for(int i=0;i<k;i++){
        if(ends[i]-starts[i]>largest_interval.second-largest_interval.first){
            largest_interval = {starts[i], ends[i]};
        }
    }
    
    int interval_length = largest_interval.second - largest_interval.first + 1;
    int partition_size = interval_length/(std::max(1, (int)(std::log(interval_length))));

    std::vector<int> pivots;
    for(int i=partition_size; i<interval_length; i+=partition_size){
        pivots.push_back(A[largest_interval.first+i]);
    }

    std::vector<std::vector<int>> ranks(pivots.size(), std::vector<int>(k));
    std::vector<int> rank_sums(pivots.size(),0);
    #pragma omp parallel for
    for(int i = 0; i < pivots.size(); i++){
        int sum = 0;
        for(int ii = 0; ii < k; ii++){
            ranks[i][ii] = std::lower_bound(A.begin() + starts[ii], A.begin() + ends[ii] + 1, pivots[i]) - (A.begin() + starts[ii]);
            sum += ranks[i][ii];
        }
        rank_sums[i]=sum;
    }

    #pragma omp parallel for
    for(int i=0;i<pivots.size();i++){
        temp[rank_sums[i]] = pivots[i];
        //merge sequencial de intervalos
        //idx guarda comienzo de cada banda
        std::vector<int> idx(k);
        for(int ii = 0; ii<k;ii++){
            if(i>0)idx[ii] = ranks[i-1][ii];
            else idx[ii] = 0;
        }
        
        for (int ii = 0; ii < rank_sums[i] - (i > 0 ? rank_sums[i-1] : 0); ii++) {
            int minVal = std::numeric_limits<int>::max();
            int minIdx = -1;

            // Encontrar el mínimo entre los k subarreglos
            for (int iii = 0; iii < k; iii++) {
                if (idx[iii] <= ranks[i][iii])if(A[starts[iii]+idx[iii]] < minVal){
                    minIdx = iii;
                    minVal = A[starts[iii]+idx[iii]];
                }
            }
            
            // Agregar el mínimo al arreglo temporal
            if(i==0){
                temp[ii] = minVal;
            }else{
                temp[rank_sums[i-1]+ii] = minVal;
            }
            idx[minIdx]++;
        }
    }
    //añadir la ultima banda (entre el ultimo rango y fin the los intervalos)
    std::vector<int> idx(k);
    for(int i = 0; i < k; i++){
        idx[i] = ranks[pivots.size()-1][i];
    }

    int tail_start = rank_sums[pivots.size()-1];
    int tail_size = n - tail_start;
    for( int i=0;i<tail_size;i++){
        int minVal = std::numeric_limits<int>::max();
        int minIdx = -1;
        for(int ii=0;ii<k;ii++){
            if(idx[ii]<=ends[ii])if(A[starts[ii]+idx[ii]] < minVal){
                minVal= A[starts[ii]+idx[ii]];
                minIdx = ii;
            }
        }
        temp[tail_start+i] = minVal;
        idx[minIdx]++;
    }

    // Copiar
    std::copy(temp.begin(), temp.end(), A.begin() + starts[0]);
}
void mergeK_Par(std::vector<int>& A, int l, int r, int k){
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
    mergeK_Par(A,starts,ends);
}

void KWayMergeSort_Par(std::vector<int>& A, int l, int r, int k, bool useParallelMerge) {
    if (l >= r) return;

    // Definir largo, tamaño del subarreglo y elementos excedentes
    int n = r - l + 1;
    int size = n / k;
    int extra = n % k;

    // Arreglos que guardan los indices de inicio y fin de cada subarreglo
    std::vector<int> starts(k);
    std::vector<int> ends(k);

    int current = l;

    // Asignar los indices de inicio y fin para cada subarreglo
    for (int i = 0; i < k; i++) {
        starts[i] = current;
        int end = current + size - 1;
        if (i < extra) end++;
        ends[i] = end;
        current = end + 1;
    }

    // Umbral para decidir cuándo paralelizar
    int threshold = 1000; 

    // Paralelo
    #pragma omp parallel for if(n > threshold)
    for (int i = 0; i < k; i++) {
        if (starts[i] < ends[i]) {
            KWayMergeSort_Par(A, starts[i], ends[i], k, useParallelMerge);
        }
    }
    // Merge k-way
    if(n>threshold && useParallelMerge){
        mergeK_Par(A, l, r, k); // Version 3.4
    }else{
        mergeK_Sec(A, l, r, k); // Version 3.2
    }
}