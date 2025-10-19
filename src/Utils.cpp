#include <Utils.hpp>
#include <random>
#include <Geode/Geode.hpp>

using namespace utilities;
using namespace geode::prelude;

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


Brainrot::Age utilities::stringToAge(const std::string& age) {
    if (age == "baby") return Brainrot::Age::Baby;
    if (age == "child") return Brainrot::Age::Child;
    if (age == "tiktok-user") return Brainrot::Age::TikTokUser;
    if (age == "teenager") return Brainrot::Age::Teenager;
    if (age == "adult") return Brainrot::Age::Adult;

    return Brainrot::Age::Baby; // return baby if the types dont match
}