#include "Task.h"
#include "utils.h"


//-------------------------------------------------------------------------------------------------
//          SEND OPERATIONS
//-------------------------------------------------------------------------------------------------
template <typename TaskT>
void sendTask(const TaskT& task, int pid, MPI_Comm comm) {
    const auto [cs, csize] = task.getRawCircles();
    const auto [ts, tsize] = task.getRawTriangles();
    std::cout << cs << ' ' << ts << std::endl;
    MPI_Send((char*)cs, csize * sizeof(circle_t), MPI_CHAR, pid, 0, comm);
    MPI_Send((char*)ts, tsize * sizeof(triangle_t), MPI_CHAR, pid, 0, comm);
}

void sendCompleteTask(const CompleteTask& ctask, int pid, MPI_Comm comm) {
    const point_t* data = ctask.data();
    const size_t size = ctask.size() * sizeof(point_t);

    MPI_Send((char*)data, size, MPI_CHAR, pid, 0, comm);
}

//-------------------------------------------------------------------------------------------------
//          RECV OPERATIONS
//-------------------------------------------------------------------------------------------------
typedef Task<Array<circle_t>, Array<triangle_t> > AllocTask;

AllocTask recvTask(int pid, MPI_Comm comm) {
    MPI_Status status;
    int size;
    // Recieve circles
    MPI_Probe(pid, MPI_ANY_TAG, comm, &status);
    MPI_Get_count(&status, MPI_CHAR, &size);
    Array<circle_t> circles(size / sizeof(circle_t));
    MPI_Recv((void*)circles.data(), size, MPI_CHAR, pid, MPI_ANY_TAG, comm, &status);
    // Recieve triangles
    MPI_Probe(pid, MPI_ANY_TAG, comm, &status);
    MPI_Get_count(&status, MPI_CHAR, &size);
    Array<triangle_t> triangles(size / sizeof(triangle_t));
    MPI_Recv((void*)triangles.data(), size, MPI_CHAR, pid, MPI_ANY_TAG, comm, &status);

    AllocTask task(std::move(circles), std::move(triangles));
    return task;
}

CompleteTask recvCompleteTask(int pid, MPI_Comm comm) {
    MPI_Status status;
    int size;
    // Recieve points
    MPI_Probe(pid, MPI_ANY_TAG, comm, &status);
    MPI_Get_count(&status, MPI_CHAR, &size);
    PointsArray points(size / sizeof(point_t));
    MPI_Recv((void*)points.data(), size, MPI_CHAR, pid, MPI_ANY_TAG, comm, &status);
    return CompleteTask(std::move(points));
}