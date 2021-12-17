#include <omp.h>
#include "../lib/intersect.h"
#include "../lib/PointsArray.h"

const size_t num_threads = 2;

template <typename ArrC, typename ArrT>
std::vector<point_t> intersect_omp(const ArrC& cs, const ArrT& ts) {
    std::vector<point_t> pvec;
    MutexPointsArrayWrap points(pvec); // Синхронное множество (множество с мьютексом)
    const size_t nts = ts.size();
    const size_t ncs = cs.size();

    // Устанавливаем количество потоков
    omp_set_num_threads(num_threads);

    #pragma omp parallel for
    for (size_t i = 0; i < ncs; ++i) {
        PointsArray localPoints;    
        for (size_t j = 0; j < nts; ++j)
            omp::intersect(ts[j], cs[i], localPoints);

        points.push(localPoints);
    }

    return std::move(pvec);
}