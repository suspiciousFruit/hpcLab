#include <vector>
#include <fstream>
#include <iostream>

template <typename T>
std::vector<T> read(const std::string& filename)
{
	std::vector<T> res;
	T value;
	std::ifstream file(filename);

	if (file.is_open())
	{
		while (true)
		{
			file >> value;
			res.emplace_back(value);

			if (!file.good() || file.eof()) break;
		}
	}
	else std::cout << "Error: can not open file " << filename << std::endl;
	return res;
}

template <typename IContainer>
void write(const IContainer& container, const std::string& filename)
{
	std::ofstream file(filename);

	if (file.is_open())
		for (const auto& elem : container)
			file << elem << std::endl;
}

template <typename T>
void print(const T& t) {
    for (const auto& a : t) std::cout << a << ' ';
    std::cout << '\n';
}