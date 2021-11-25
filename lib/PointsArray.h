#pragma once
#include <vector>
#include "shapes.h"



class PointsArray {
private:
    std::vector<point_t> points_;
public:
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
};