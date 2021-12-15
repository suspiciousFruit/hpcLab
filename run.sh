#g++ -fopenmp -std=c++17 main_mpi.cpp
#run -c 2 ./a.out

mpic++ -fopenmp -std=c++17 main_mpi.cpp
srun -N 2 ./a.out