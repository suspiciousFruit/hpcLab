#include <iostream>
#include <omp.h>
#include <vector>
#include <iomanip>

#include "lib/intersect.h"
#include "lib/MutexSet.h"
#include "lib/FixedArray.h"
#include "lib/PointsArray.h"
#include "lib/utility.h"

// int main_not_working() {
//     MutexSet points_set;
//     const auto& ts = read<triangle_t>("triangles.txt");
// 	const auto& cs = read<circle_t>("circles.txt");
    
//     const double startTime = omp_get_wtime();

//     const size_t nthreads = omp_get_num_threads();
//     #pragma omp parallel
//     {
//         const size_t num = omp_get_thread_num();
//         #ifdef DYNAMIC
//         PointsArray points;
//         #else
//         FixedPointsArray points;
//         #endif

//         for (size_t i = num; i < cs.size(); i += nthreads) {
//             for (size_t j = 0; j < ts.size(); ++j) {

//                 #ifndef DYNAMIC
//                 if (!points.hasSpaceFor(6)) {
//                     points_set.push(points);
//                     points.clear();
//                 }
//                 #endif
//                 omp::intersect(ts[j], cs[i], points);

//             }
//         }

//         points_set.push(points);
//     }

//     const double endTime = omp_get_wtime();
//     std::cout << std::setprecision(10) << endTime - startTime << std::endl;
//     return 0;
// }

#define DYNAMIC

int main(int argc, const char* argv[]) {
    MutexSet points_set; // Синхронное множество (множество с мьютексом)
    const auto ts = read<triangle_t>("triangles.txt");
	const auto cs = read<circle_t>("circles.txt");
    const size_t nts = ts.size();
    const size_t ncs = cs.size();

    if (argc > 1) // Устанавливаем количество потоков
        omp_set_num_threads(atoi(argv[1]));

    const double startTime = omp_get_wtime();
    
    #pragma omp parallel for
    for (size_t i = 0; i < ncs; ++i) {
        #ifdef DYNAMIC // Использование динамического массива для точек (в каждом потоке)
        PointsArray points;
        #else // Использование массивов фиксированного размера для точек (в каждом потоке)
        FixedPointsArray points;
        #endif        
        for (size_t j = 0; j < nts; ++j) {

            #ifndef DYNAMIC
            if (!points.hasSpaceFor(6)) {
                points_set.push(points);
                points.clear();
            }
            #endif
            omp::intersect(ts[j], cs[i], points);

        }
        points_set.push(points);
    }

    const double endTime = omp_get_wtime();
    std::cout << std::setprecision(10) << endTime - startTime << std::endl;
    std::ofstream file("results.txt");
    file << points_set;
    return 0;
}