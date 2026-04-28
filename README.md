# Tarea1-SDP

Para compilar mainMS
g++ Secuenciales/mainMS_Sec.cpp Secuenciales/MergeSortSec.cpp -O2 -o ms

Para compilar mainMSKW
g++ Secuenciales/mainMSKW_Sec.cpp Secuenciales/MergeSortKWaySec.cpp -O2 -o mskw

Para compilar mainMSKW_Par
g++ Paralelos/mainMSKW_Par.cpp Paralelos/MergeSortKWayPar.cpp -O3 -fopenmp -o mskw_par

Para compilar mainPM
g++ Paralelos/mainPM.cpp Paralelos/ParallelMerge.cpp -O3 -fopenmp -o ParallelMerge

Para compilar mainMSKW_Completo
g++ Paralelos/mainMSKW_Completo.cpp Paralelos/MergeSortKWayPar.cpp -O3 -fopenmp -o mskw_completo

Para ejecutar cada uno respectivamente:
.\ms.exe
.\mskw.exe
.\mskw_par.exe
.\mskw_completo.exe