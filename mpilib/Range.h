#pragma once
#include <iostream>

class Range {
public:
	size_t from_;
	size_t to_;
public:
	Range(size_t a, size_t b) : from_(a), to_(b) { }

	size_t size() const {
		return to_ - from_;
	}

    size_t from() const {
        return from_;
    }

    size_t to() const {
        return to_;
    }

	friend std::ostream& operator << (std::ostream& s, const Range& r) {
		s << "(" << r.from_ << ", " << r.to_ << ")";
		return s;
	}
};