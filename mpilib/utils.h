#pragma once
#include <vector>
#include "Range.h"

template <typename T>
class ViewArray {
private:
	const T* data_;
	size_t size_;
public:
    ViewArray(const T* data, size_t size)
    : data_(data), size_(size) { }

    const T* data() const {
        return data_;
    }

    size_t size() const {
        return size_;
    }

    const T& operator[] (size_t index) const {
        return data_[index];
    }
};

template <typename T>
ViewArray<T> makeViewArray(const std::vector<T>& vec, Range r) {//
    return ViewArray<T>(vec.data() + r.from(), r.size());
}



template <typename T>
class Array {
private:
    T* data_;
    size_t size_;
public:
    Array(size_t size) : size_(size) {
        if (size_ > 0) data_ = new T[size];
        else data_ = nullptr;
    }

    Array() : data_(nullptr), size_(0) { }

    Array(Array<T>&& other) : data_(other.data_), size_(other.size_) {
        other.data_ = nullptr;
        other.size_ = 0;
    }

    Array& operator= (Array&& other) {
        if (this != &other) {
            this->~Array();
            data_ = other.data_;
            size_ = other.size_;
            other.data_ = nullptr;
            other.size_ = 0;
        }
        return *this;
    }

    const T* data() const {
        return data_;
    }

    size_t size() const {
        return size_;
    }

    const T& operator[] (size_t index) const {
        return data_[index];
    }

    ~Array() {
        if (data_) delete data_;
        data_ = nullptr;
        size_ = 0;
    }
};