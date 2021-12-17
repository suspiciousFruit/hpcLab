#pragma once
#include <iostream>
#include <vector>
#include "../lib/shapes.h"
#include "../lib/PointsArray.h"
#include "intersectApi.h"

class CompleteTask {
private:
    PointsArray points_;
public:
    CompleteTask(PointsArray&& parr) : points_(std::move(parr)) { }
    CompleteTask(std::vector<point_t>&& points) : points_(std::move(points)) {}
    CompleteTask() : points_(0) { }
    const point_t* data() const { return points_.data(); }
    size_t size() const { return points_.size(); }

    void mergeWith(const CompleteTask& otherTask) {
        points_.push(otherTask.points_);
    }

    friend std::ostream& operator<< (std::ostream& s, const CompleteTask& ctask) {
        for (size_t i = 0; i < ctask.size(); ++i)
            s << ctask.points_[i] << '\n';
        return s;
    }
};

template <typename ArrC, typename ArrT>
class Task {
private:
public:
	ArrC circles_;
	ArrT triangles_;
public:
    explicit Task(ArrC&& cs, ArrT&& ts)
    : triangles_(std::move(ts)), circles_(std::move(cs)) { }

    std::pair<const circle_t*, size_t> getRawCircles() const {
        return { circles_.data(), circles_.size() };
    }

    std::pair<const triangle_t*, size_t> getRawTriangles() const {
        return { triangles_.data(), triangles_.size() };
    }

    CompleteTask complete() const {
        auto res = intersect_omp(circles_, triangles_);
        return CompleteTask(std::move(res));
    }

    friend std::ostream& operator<< (std::ostream& s, const Task<ArrC, ArrT>& task) {
        const auto [cs, csize] = task.getRawCircles();
        const auto [ts, tsize] = task.getRawTriangles();
        s << "----------------------------------------------------------------------\n";
        for (size_t i = 0; i < csize; ++i)
            s << cs[i] << '\n';
        for (size_t i = 0; i < tsize; ++i)
            s << ts[i] << '\n';
        s << "----------------------------------------------------------------------";
        return s;
    }

};
