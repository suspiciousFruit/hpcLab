#pragma once
#include <iostream>
#include <omp.h>
#include <vector>
#include "shapes.h"

class MutexPointsArray;

class PointsArray {
private:
    std::vector<point_t> points_;
public:
    PointsArray() { }
    PointsArray(size_t size) : points_(size) { }

    PointsArray(std::vector<point_t>&& vec)
    : points_(std::move(vec)) { }

    void push(const point_t& p) {
        points_.push_back(p);
    }

    void push(const PointsArray& arr) {
        for (size_t i = 0; i < arr.size(); ++i)
            pushUnique(arr[i]);
    }

    void pushUnique(const point_t& p) {    
        for (size_t i = 0; i < points_.size(); ++i)
            if (points_[i] == p) return;
        push(p);
    }

    friend std::ostream& operator<< (std::ostream& stream, const PointsArray& arr) {
        for (size_t i = 0; i < arr.size(); ++i)
            stream << arr[i] << ' ';
        return stream;
    }

    point_t& operator[] (size_t index) {
        return points_[index];
    }

    const point_t& operator[] (size_t index) const {
        return points_[index];
    }

    size_t size() const {
        return points_.size();
    }

    const point_t* data() const {
        return points_.data();
    }
};

class MutexPointsArray {
private:
    PointsArray points_;
    omp_lock_t lock_;
public:
    MutexPointsArray() {
        omp_init_lock(&lock_);
    }

    void pushUnique(const point_t& p) {
        omp_set_lock(&lock_);
        points_.pushUnique(p);
        omp_unset_lock(&lock_);
    }

    template <typename ArrType>
    void push(const ArrType& points) {
        omp_set_lock(&lock_);
        points_.push(points);
        omp_unset_lock(&lock_);
    }

    PointsArray&& getPointsArray() {
        return std::move(points_);
    }

    ~MutexPointsArray() {
        omp_destroy_lock(&lock_);    
    }
};