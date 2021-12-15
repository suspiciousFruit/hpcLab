#include <iostream>
#include <vector>
#include <mpi.h>
#include "lib/shapes.h"
#include "mpilib/Split.h"
#include "mpilib/utils.h"
#include "mpilib/transmitting.h"




// int main() {
//     int pid = getPid();
//     int npids = getNpids();

//     if (pid == 0) {
//         const auto [circles, triangles] = read(file1, file2);
//         const auto tasks = splitToTasks(circles, triangles, npids);
//         // for (size_t i = 1; i < npids; ++i)
//         //     sendTask(tasks[i], i);

//         // const Solution solution = tasks[0].complete();
//         // for (size_t i = 1; i < npids; ++i) {
//         //     const auto s = recvSolution(any);
//         //     solution.merge(s);
//         // }

//         // write(filename, solution);
//     }
//     else {
//         // Task task = recvTask(pid);
//         // const auto sol = task.complete();
//         // sendSolution(0);
//     }
// }

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

std::vector<circle_t> circles = {
    {{2, 2}, 2.0},
    {{2, 2}, 2.0},
    {{2, 2}, 2.0}
};

std::vector<triangle_t> triangles = {
    {{0, 2}, {2, 0}, {4, 4}},
    {{0, 2}, {2, 0}, {4, 4}},
    {{0, 2}, {2, 0}, {4, 4}}
};


int main(int argc, char** argv) {
    //const auto ts = read<triangle_t>("triangles.txt");
	//const auto cs = read<circle_t>("circles.txt");
    MPI_Init(&argc, &argv);

    const int pid = getRank(MPI_COMM_WORLD);
    const int npids = getSize(MPI_COMM_WORLD);

    if (pid == 0) {
        const auto tasks = splitToTasks(circles, triangles, npids);
        for (size_t i = 1; i < npids; ++i) {
            std::cout << tasks[i] << '\n';
            sendTask(tasks[i], i, MPI_COMM_WORLD);
        }

        auto myCtask = tasks[0].complete();

        for (size_t i = 1; i < npids; ++i) {
            const auto ctask = recvCompleteTask(i, MPI_COMM_WORLD);
            myCtask.mergeWith(ctask);
        }
        std::cout << myCtask;
    }
    else {
        const auto task = recvTask(0, MPI_COMM_WORLD);
        const auto ctask = task.complete();
        sendCompleteTask(ctask, 0, MPI_COMM_WORLD);
    }

    MPI_Finalize();
}
