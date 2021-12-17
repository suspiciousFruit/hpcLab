#g++ -fopenmp -std=c++17 main_mpi.cpp
#run -c 2 ./a.out

mpic++ -fopenmp -std=c++17 -O3 main_mpi.cpp
srun -n 1 -c 1 ./a.out