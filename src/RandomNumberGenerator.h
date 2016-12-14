#ifndef _RANDOM_NUMBER_GENERATOR_H_
#define _RANDOM_NUMBER_GENERATOR_H_

#include <random>
#include <ctime>

class RNG {
    std::default_random_engine generator;

public:
    RNG(unsigned seed): generator(seed) {}

    inline int intRange(int a, int b) {
        std::uniform_int_distribution<int> dist(a,b);
        return dist(generator);
    }

    inline float floatRange(float a, float b) {
        std::uniform_real_distribution<float> dist(a,b);
        return dist(generator);
    }

    inline bool bernoulli(float trueProbability) {
        std::bernoulli_distribution dist(trueProbability);
        return dist(generator);
    }
};

RNG global_rng(time(0));

#endif