#include <iostream>
#include <future>
#include <random>

int GetRandomIntInRange(const int min, const int max)
{
	static std::default_random_engine gen(
		static_cast<unsigned>(
			std::chrono::system_clock::now().time_since_epoch().count()
			)
	);
	std::uniform_int_distribution<int> distribution(min, max);
	return distribution(gen);
}

int GenerateValue(int value)
{
	return GetRandomIntInRange(0, 80) + value;
}

int main()
{
	std::future<int> Producer = std::async(GenerateValue, 10);
	std::cout << Producer.get();
}
