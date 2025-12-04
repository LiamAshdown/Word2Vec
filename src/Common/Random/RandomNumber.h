#pragma once

#include <random>
#include <memory>
#include <type_traits>

class RandomNumber
{
public:
    RandomNumber() : _generator(std::random_device{}()) {}

    explicit RandomNumber(unsigned int seed) : _generator(seed) {}

    static RandomNumber *Instance()
    {
        static RandomNumber instance;
        return &instance;
    }

    template <typename T>
    typename std::enable_if<std::is_integral<T>::value, T>::type
    Number(T min, T max)
    {
        std::uniform_int_distribution<T> dist(min, max);
        return dist(_generator);
    }

    template <typename T>
    typename std::enable_if<std::is_floating_point<T>::value, T>::type
    Number(T min, T max)
    {
        std::uniform_real_distribution<T> dist(min, max);
        return dist(_generator);
    }

private:
    std::mt19937 _generator;
};