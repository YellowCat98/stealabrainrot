#include <Utils.hpp>
#include <random>

using namespace utilities;

float random::random(float a, float b) {
    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_real_distribution<float> dist(a, b);
    return dist(gen);
}

int random::randint(int a, int b) {
    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_int_distribution<int> dist(a, b);
    return dist(gen);
}