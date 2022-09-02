#include <iostream>
#include <string>
#include <future>
#include <random>
#include <map>

struct Person
{
	std::string Name;
	int Age;
	int Performance;
};

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

template<typename Type>
void GetFutureValue(std::shared_future<Type> myFuture)
{
	//printf("\nFuture value = %d", myFuture.get());
	std::cout << "\nFutureValue = " << myFuture.get();
}

std::string GetName()
{
	std::string s("Anton");
	return s;
}

void PrintMass(Person* Actor)
{
	std::cout << "\n" << Actor->Name << " " << Actor->Age << " " << Actor->Performance;
}


int main()
{
	std::shared_future<std::string> FutureProducerName = std::async(GetName);
	std::shared_future<int> FutureProducerAge = std::async(GetRandomIntInRange, 20, 40);
	std::shared_future<int> FutureProducerPerformace = std::async(GetRandomIntInRange, 0, 100);
	std::thread ProducerThreadName(GetFutureValue<std::string>, std::ref(FutureProducerName));
	std::thread ProducerThreadAge(GetFutureValue<int>, std::ref(FutureProducerAge));
	std::thread ProducerThreadPerformance(GetFutureValue<int>, std::ref(FutureProducerPerformace));

	std::vector<Person*> ConsumerMap;

	ConsumerMap.push_back(new Person());
	for (auto n : ConsumerMap) {
		n->Name = FutureProducerName.get();
		n->Age = FutureProducerAge.get();
		n->Performance = FutureProducerPerformace.get();
		PrintMass(n);
	}

	ProducerThreadName.join();
	ProducerThreadAge.join();
	ProducerThreadPerformance.join();	

}
