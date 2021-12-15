#pragma once
#include <iostream>
#include <vector>
#include <cmath>
#include "Task.h"
#include "Range.h"
#include "utils.h"

size_t roundDiv(size_t num, size_t div) {
	size_t a = num / div;
	if (div * a != num) return a + 1;
	return a;
}

std::pair<size_t, size_t> getSplitCount(size_t npids) { // a * b == npids; (a - b) -> min, a * b - npids -> min
	size_t a = std::sqrt(npids);
	if (a == 1) return { a, npids };
	return { a, a };
}

std::vector<std::pair<Range, Range>> genSplit(size_t nc, size_t nt, size_t npids) {
	std::vector<std::pair<Range, Range>> res;
	const auto [ns_c, ns_t] = getSplitCount(npids);
	size_t cBlock = roundDiv(nc, ns_c);
	size_t tBlock = roundDiv(nt, ns_t);
	//printf("tBlock %d, cBlock %d\n", tBlock, cBlock);

	for (size_t i = 0; i < ns_c; ++i) {
		Range crange(i * cBlock, std::min(nc, (i + 1) * cBlock));
		for (size_t j = 0; j < ns_t; ++j) {
			Range trange(j * tBlock, std::min(nt, (j + 1) * tBlock));
			res.emplace_back(crange, trange);
		}
	}

	return res;
}

typedef Task<ViewArray<circle_t>, ViewArray<triangle_t>> ViewTask;

std::vector<ViewTask> splitToTasks(
	const std::vector<circle_t>& cs, const std::vector<triangle_t>& ts, size_t npids) {

	const auto ranges = genSplit(cs.size(), ts.size(), npids);
	std::vector<ViewTask> tasks;

	for (size_t i = 0; i < ranges.size(); ++i) {
		const auto [cr, tr] = ranges[i];
		tasks.emplace_back(makeViewArray(cs, cr), makeViewArray(ts, tr));
	}

	return tasks;
}