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

    explicit PointsArray(std::vector<point_t>&& vec) : points_(std::move(vec)) { }

    // PointsArray& operator= (PointsArray&& other) {
    //     if (this != &other)
    //         points_ = std::move(other.points_);
    //     return *this;
    // }

    void push(const point_t& p) {
        points_.push_back(p);
    }

    void pushUnique(const point_t& p) {    
        for (size_t i = 0; i < points_.size(); ++i)
            if (points_[i] == p) return;
        push(p);
    }

    template <typename ArrT>
    void push(const ArrT& arr) {
        for (size_t i = 0; i < arr.size(); ++i)
            pushUnique(arr[i]);
    }

    friend std::ostream& operator<< (std::ostream& stream, const PointsArray& arr) {
        for (size_t i = 0; i < arr.size(); ++i)
            stream << arr[i] << '\n';
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

class MutexPointsArrayWrap {
private:
    std::vector<point_t>& points_;
    omp_lock_t lock_;
public:
    MutexPointsArrayWrap(std::vector<point_t>& vec) : points_(vec) {
        omp_init_lock(&lock_);
    }

    void pushUnique(const point_t& p) {
        omp_set_lock(&lock_);
        pushUnique__nolock(p);
        omp_unset_lock(&lock_);
    }

    template <typename ArrType>
    void push(const ArrType& points) {
        omp_set_lock(&lock_);
        for (size_t i = 0; i < points.size(); ++i)
            pushUnique__nolock(points[i]);
        omp_unset_lock(&lock_);
    }

    void push(const point_t& p) {
        omp_set_lock(&lock_);
        points_.push_back(p);
        omp_unset_lock(&lock_);
    }

    void pushUnique__nolock(const point_t& p) {    
        for (size_t i = 0; i < points_.size(); ++i)
            if (points_[i] == p) return;
        points_.push_back(p);
    }

    ~MutexPointsArrayWrap() {
        omp_destroy_lock(&lock_);    
    }
};