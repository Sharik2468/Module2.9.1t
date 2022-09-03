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

std::string Names("Anton Genadiy Lev Dmitriy Yuriy Arsen Nikita Gosha Rudolf Bob Alex Artem");
std::string SurNames("Sharabanov Popov Dmitriev Nagiev Stolov Polin");
std::string Patronymics("Alexandrovich Popovich Semenovich Antonovich Lvovich");
std::mutex g_lock;

void GetRandomIntInRangeForAge(const int min, const int max, Person* Actor, int switcher)
{
    g_lock.lock();
    static std::default_random_engine gen(static_cast<unsigned>(std::chrono::system_clock::now().time_since_epoch().count()));
    std::uniform_int_distribution<int> distribution(min, max);
    switch (switcher)
    {
        case (1): Actor->Age = distribution(gen);
        case (2): Actor->Performance = distribution(gen);
    }
    g_lock.unlock();
}

int GetRandomIntInRange(const int min, const int max)
{
    static std::default_random_engine gen(static_cast<unsigned>(std::chrono::system_clock::now().time_since_epoch().count()));
    std::uniform_int_distribution<int> distribution(min, max);
    return distribution(gen);
}

void GetName(Person* Actor)
{
    g_lock.lock();

    std::string Result;
    std::string CachedNames = Names;
    for (int i = 0; i < std::count(CachedNames.begin(), CachedNames.end(), ' ') + 1; i++)
    {
        if (GetRandomIntInRange(1, 4) % 2 != 0)
            CachedNames.erase(CachedNames.begin(), std::find(CachedNames.begin(), CachedNames.end(), ' ') + 1);
        else
            break;
    }
    std::string s2(CachedNames.begin(), std::find(CachedNames.begin(), CachedNames.end(), ' '));

    std::string CachedSurNames = SurNames;
    for (int i = 0; i < std::count(CachedSurNames.begin(), CachedSurNames.end(), ' ') + 1; i++)
    {
        if (GetRandomIntInRange(1, 4) % 2 != 0)
            CachedSurNames.erase(CachedSurNames.begin(), std::find(CachedSurNames.begin(), CachedSurNames.end(), ' ') + 1);
        else
            break;
    }
    std::string s1(CachedSurNames.begin(), std::find(CachedSurNames.begin(), CachedSurNames.end(), ' '));

    std::string CachedPatronymics = Patronymics;
    for (int i = 0; i < std::count(CachedPatronymics.begin(), CachedPatronymics.end(), ' ') + 1; i++)
    {
        if (GetRandomIntInRange(1, 4) % 2 != 0)
            CachedPatronymics.erase(CachedPatronymics.begin(), std::find(CachedPatronymics.begin(), CachedPatronymics.end(), ' ') + 1);
        else
            break;
    }
    std::string s0(CachedPatronymics.begin(), std::find(CachedPatronymics.begin(), CachedPatronymics.end(), ' '));
    Result = s1 + " " + s2 + " " + s0 + " ";

    Actor->Name = Result;

    g_lock.unlock();
}

void PrintMasses(std::vector<Person*> Actors, int Size)
{
    for (int it = 0; it != Size; it++)
    {
        std::cout << "\n" << Actors[it]->Name << " " << Actors[it]->Age << " " << Actors[it]->Performance;
    }
}

int main()
{
    int n = 4, debug = 0;
    std::vector<Person*> Consumer;

    for (int i = 0; i < n; i++)
    {
        Consumer.push_back(new Person());

        std::thread ProducerThreadName(GetName, Consumer[i]);
        std::thread ProducerThreadAge(GetRandomIntInRangeForAge, 18, 30, Consumer[i], 1);
        std::thread ProducerThreadPerformance(GetRandomIntInRangeForAge, 0, 100, Consumer[i], 2);

        ProducerThreadName.join();
        ProducerThreadAge.join();
        ProducerThreadPerformance.join();
    }

    std::sort(Consumer.begin(), Consumer.end(), [](Person* a, Person* b) { return a->Performance > b->Performance; });

    PrintMasses(Consumer, n);
}
