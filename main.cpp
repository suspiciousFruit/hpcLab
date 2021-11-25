#include <unordered_set>
#include "lib/utility.h"
#include "lib/intersect.h"


int main()
{
	const auto& ts = read<triangle_t>("triangles.txt");
	const auto& cs = read<circle_t>("circles.txt");

	const circle_t c({2, 2}, 2.0);
	const triangle_t t({0, 2}, {2, 0}, {4, 4});

	const auto res = find(ts, cs);
	write(res, "results.txt");
	print(res);

    return 0;
}