#include <iostream>
#include <fstream>
#include <vector>
#include <mpi.h>
#include "lib/shapes.h"
#include "mpilib/Split.h"
#include "mpilib/utils.h"
#include "mpilib/transmitting.h"
#include "lib/utility.h"

int getRank(MPI_Comm comm) {
    int rank;
    MPI_Comm_rank(comm, &rank);
    return rank;
}

int getSize(MPI_Comm comm) {
    int size;
    MPI_Comm_size(comm, &size);
    return size;
}

std::vector<circle_t> cs = {
    {{1, 2}, 2.0},
    {{2, 2}, 2.0}
};

std::vector<triangle_t> ts = {
    {{1, 2}, {2, 0}, {4, 4}},
    {{2, 2}, {2, 0}, {4, 4}}
};

void dual() {
    const auto circles = read<circle_t>("data/circles.txt");
    const auto triangles1 = read<triangle_t>("data/triangles0_111.txt");
    const auto triangles2 = read<triangle_t>("data/triangles111_221.txt");

    const auto r1 = PointsArray(intersect_omp(circles, triangles1));
    const auto r2 = PointsArray(intersect_omp(circles, triangles2));

    std::ofstream file1("data/res_2_1.txt"); file1 << r1;
    std::ofstream file2("data/res_2_2.txt"); file2 << r2;
}

template <typename ArrT, typename T>
bool has(const ArrT& arr, const T& el) {
    for (size_t i = 0; i < arr.size(); ++i)
        if (arr[i] == el) return true;
    return false;
}

template <typename A, typename B>
bool equal(const A& a, const B& b) {
    for (size_t i = 0; i < a.size(); ++i)
        if (!has(b, a[i])) return false;
    
    for (size_t i = 0; i < b.size(); ++i)
        if (!has(a, b[i])) return false;
    return true;
}

void test() {
    const auto triangles = read<triangle_t>("triangles.txt");
	const auto circles = read<circle_t>("circles.txt");
    auto tasks = splitToTasks(circles, triangles, 2);
    CompleteTask ctask;
    char* fnames[] = {"data/res_1_1.txt", "data/res_1_2.txt"};
    for (size_t i = 0; i < tasks.size(); ++i) {
        // ctask.mergeWith(t.complete());
        std::ofstream file(fnames[i]);
        file << tasks[i].complete();
    }
}

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);

    const int pid = getRank(MPI_COMM_WORLD);

    if (pid == 0) {
        const double start = MPI_Wtime();
        const auto triangles = read<triangle_t>("triangles.txt");
	    const auto circles = read<circle_t>("circles.txt");
        const int npids = getSize(MPI_COMM_WORLD);
        const auto tasks = splitToTasks(circles, triangles, npids);
        //printf("Using %d of %d process\n", tasks.size(), npids);
        
        for (size_t i = 1; i < npids; ++i)
            async::sendTask(tasks[i], i, MPI_COMM_WORLD);

        auto myCtask = tasks[0].complete();

        for (size_t i = 1; i < npids; ++i) {
            const auto ctask = recvCompleteTask(i, MPI_COMM_WORLD);
            myCtask.mergeWith(ctask);
        }

        const double end = MPI_Wtime();
        std::cout << end - start << std::endl;
        std::ofstream file("results_mpi.txt");
        file << myCtask;
    }
    else {
        const auto task = recvTask(0, MPI_COMM_WORLD);
        const auto ctask = task.complete();
        sendCompleteTask(ctask, 0, MPI_COMM_WORLD);
    }

    MPI_Finalize();
}
