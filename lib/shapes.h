#pragma once
#include <iostream>

struct point_t
{
	double x, y;

	point_t() : x(0), y(0)
	{ }

	point_t(double x, double y) : x(x), y(y)
	{ }

	friend point_t operator- (const point_t& p1, const point_t& p2)
	{
		return point_t(p1.x - p2.x, p1.y - p2.y);
	}

	friend point_t operator+ (const point_t& p1, const point_t& p2)
	{
		return point_t(p1.x + p2.x, p1.y + p2.y);
	}

	friend bool operator== (const point_t& p1, const point_t& p2)
	{
		return (p1.x == p2.x && p1.y == p2.y);
	}

	friend std::ostream& operator<< (std::ostream& s, const point_t& p)
	{
		s << p.x << ',' << p.y;
		return s;
	}

	friend std::istream& operator>>(std::istream& s, point_t& p)
	{
		char delimiter;
		s >> p.x >> delimiter >> p.y;
		return s;
	}
};

namespace std
{
	template <>
	struct hash<point_t>
	{
		size_t operator() (const point_t& p) const noexcept
		{ return p.x; }
	};
}

struct triangle_t
{
	point_t a, b, c;

	triangle_t()
	{ }

	triangle_t(point_t _a, point_t _b, point_t _c) :
		a(_a), b(_b), c(_c)
	{ }

	friend std::ostream& operator<< (std::ostream& s, const triangle_t& t)
	{
		s << t.a << ',' << t.b << ',' << t.c;
		return s;
	}

	friend std::istream& operator>> (std::istream& s, triangle_t& t)
	{
		char delimiter;
		s >> t.a >> delimiter >> t.b >> delimiter >> t.c;
		return s;
	}
};

struct circle_t
{
	point_t a;
	double r;

	circle_t() : r(0.0)
	{ }

	circle_t(point_t center, double radius) : a(center), r(radius)
	{ }

	friend std::ostream& operator<< (std::ostream& s, const circle_t& c)
	{
		s << c.a << ',' << c.r;
		return s;
	}

	friend std::istream& operator>> (std::istream& s, circle_t& c)
	{
		char delimiter;
		s >> c.a >> delimiter >> c.r;
		return s;
	}
};

struct line_t
{
	double a, b, c;

	line_t(const point_t& p1, const point_t& p2)
	{
		const double x1 = p1.x;
		const double x2 = p2.x;
		const double y1 = p1.y;
		const double y2 = p2.y;

		if (y1 - y2)
		{
			a = 1;
			b = - (x1 - x2) / (y1 - y2);
			c = - x1 - b * y1;
		}
		else
		{
			b = 1;
			a = - (y1 - y2) / (x1 - x2);
			c = - y1 - a * x1;
		}
	}
};