#include <vector>
#include <cmath>
#include <unordered_set>
#include "shapes.h"
#include "FixedArray.h"




namespace omp {
	template <typename ArrType>
	size_t intersect(const line_t& line, double r, ArrType& res) {
		const double a = line.a;
		const double b = line.b;
		const double c = line.c;

		const double x0 = - a * c / (a * a + b * b);
		const double y0 = - b * c / (a * a + b * b);

		const double dist = r * r * (a * a + b * b);
		if (std::abs(c * c - dist) == 0) {
			res.pushUnique({x0, y0});
			return 1;
		}
		else if (c * c < dist) {
			const double d = r * r - c * c / (a * a + b * b);
			const double mult = std::sqrt(d / (a * a + b * b));
			const double ax = x0 + b * mult;
			const double bx = x0 - b * mult;
			const double ay = y0 - a * mult;
			const double by = y0 + a * mult;
			res.pushUnique({ax, ay});
			res.pushUnique({bx, by});
			return 2;
		}
		
		return 0;
	}

	// intersect triangle and circle in (0, 0) with raidus r
	template <typename ArrType>
	size_t intersect(const triangle_t& t, double r, ArrType& res) {
		return intersect(line_t(t.a, t.b), r, res) +
			intersect(line_t(t.a, t.c), r, res) +
			intersect(line_t(t.c, t.b), r, res);
	}

	template <typename ArrType>
	void intersect(const triangle_t& t, const circle_t& c, ArrType& res) {
		// transform coords
		const point_t& offset = c.a;
		const triangle_t offset_triange(t.a - offset, t.b - offset, t.c - offset);

		// intersect triangle and cirlce
		const size_t npoints = intersect(offset_triange, c.r, res);

		// transform coords back
		for (size_t i = res.size() - npoints; i < res.size(); ++i)
			res[i] = res[i] + offset;
	}
}


// intersect line and circle in (0, 0) with radius r
std::vector<point_t> intersect(const line_t& line, double r)
{
	std::vector<point_t> res;
	const double a = line.a;
	const double b = line.b;
	const double c = line.c;

	const double x0 = - a * c / (a * a + b * b);
	const double y0 = - b * c / (a * a + b * b);

	const double dist = r * r * (a * a + b * b);
	if (std::abs(c * c - dist) == 0)
	{
		res.emplace_back(x0, y0);
	}
	else if (c * c < dist)
	{
		double d = r * r - c * c / (a*a + b * b);
		double mult = std::sqrt(d / (a * a + b * b));
		double ax, ay, bx, by;
		ax = x0 + b * mult;
		bx = x0 - b * mult;
		ay = y0 - a * mult;
		by = y0 + a * mult;
		res.emplace_back(ax, ay);
		res.emplace_back(bx, by);
	}
	
	return res;
}

bool in(const point_t& p, std::vector<point_t>& ps)
{
	for (size_t i = 0; i < ps.size(); ++i)
		if (ps[i] == p) return true;
	return false;
}

// intersect triangle and circle in (0, 0) with raidus r
std::vector<point_t> intersect(const triangle_t& t, double r)
{
	std::vector<point_t> res;

	const auto r1 = intersect(line_t(t.a, t.b), r);
	const auto r2 = intersect(line_t(t.a, t.c), r);
	const auto r3 = intersect(line_t(t.c, t.b), r);

	const auto insert = [&res](const auto& arr) -> void
	{
		for (size_t i = 0; i < arr.size(); ++i)
			if (!in(arr[i], res)) res.push_back(arr[i]);
	};

	insert(r1);
	insert(r2);
	insert(r3);
	return res;
}

// intersect triangle and circle
std::vector<point_t> intersect(const triangle_t& t, const circle_t& c)
{
	// transform coords
	const point_t& offset = c.a;
	const triangle_t offset_triange(t.a - offset, t.b - offset, t.c - offset);

	// intersect triangle and cirlce
	std::vector<point_t> res = intersect(offset_triange, c.r);

	// transform coords back
	for (size_t i = 0; i < res.size(); ++i)
		res[i] = res[i] + offset;

	return res;
}

std::unordered_set<point_t> find(const std::vector<triangle_t> ts, const std::vector<circle_t> cs)
{
	std::unordered_set<point_t> points;

	for (const auto& t : ts)
	{
		for (const auto& c : cs)
		{
			const auto r = intersect(t, c);
			points.insert(r.begin(), r.end());
		}
	}

	return points;
}