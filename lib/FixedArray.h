#pragma once
#include "shapes.h"

template <size_t N>
class FixedArray {
private:
    point_t points_[N];
    size_t size_;
public:
    FixedArray() : size_(0) { }

    void push(const point_t& p) {
        if (size_ < N) {
            points_[size_] = p;
            ++size_;
        }
    }

    void push(const FixedArray<N>& arr) {
        for (size_t i = 0; i < arr.size(); ++i)
            pushUnique(arr[i]);
    }

    void pushUnique(const point_t& p) {
        if (size_ < N) {    
            for (size_t i = 0; i < size_; ++i) {
                if (points_[i] == p)
                    return;
            }
        }
        push(p);
    }

    void clear() {
        size_ = 0;
    }

    template <size_t T>
    friend std::ostream& operator<< (std::ostream& stream, const FixedArray<T>& arr) {
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

    bool hasSpaceFor(size_t n) const {
        return (N - size_ >= n);
    }

    size_t size() const {
        return size_;
    }

    ~FixedArray() { }
};

typedef FixedArray<64> FixedPointsArray;