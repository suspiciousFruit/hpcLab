#include <iostream>
#include <unordered_set>
#include <omp.h>
#include "FixedArray.h"


class MutexSet {
private:
    std::unordered_set<point_t> set_;
    omp_lock_t lock_;
public:
    MutexSet() {
        omp_init_lock(&lock_);
    }

    void push(const point_t& p) {
        omp_set_lock(&lock_);
        set_.insert(p);
        omp_unset_lock(&lock_);
    }

    template <typename ArrType>
    void push(const ArrType& points) {
        omp_set_lock(&lock_);
        for (size_t i = 0; i < points.size(); ++i)
            set_.insert(points[i]);
        omp_unset_lock(&lock_);
    }

    friend std::ostream& operator<< (std::ostream& stream, const MutexSet& set) {
        for (const auto& s : set.set_) stream << s << '\n';
        return stream;
    }

    ~MutexSet() {
        omp_destroy_lock(&lock_);    
    }
};