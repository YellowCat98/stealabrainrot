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

std::string utilities::ageToString(Brainrot::Age age) {
    // i dont trust c++ so i initialize std::string directly (which is probably what i should be down but anyway)
    if (age == Brainrot::Age::Baby) return std::string("baby");
    if (age == Brainrot::Age::Child) return std::string("child");
    if (age == Brainrot::Age::TikTokUser) return std::string("tiktok-user");
    if (age == Brainrot::Age::Teenager) return std::string("teenager");
    if (age == Brainrot::Age::Adult) return std::string("adult");
    
    return std::string(""); // the compiler will probably complain if i dont do this
}

Brainrot::Age utilities::getBrainrotAge(int currentStars) {
    if (IS_BETWEEN(0, currentStars, 5)) return Brainrot::Age::Baby;
    if (IS_BETWEEN(5, currentStars, 10)) return Brainrot::Age::Child;
    if (IS_BETWEEN(10, currentStars, 15)) return Brainrot::Age::TikTokUser;
    if (IS_BETWEEN(15, currentStars, 20)) return Brainrot::Age::Teenager;
    return Brainrot::Age::Adult; // brainrots are an immortal species, after they become adults, they do not age.
}