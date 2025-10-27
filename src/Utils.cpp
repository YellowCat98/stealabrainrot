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

std::string random::string(int length) {
    static const char charset[] =
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
        "abcdefghijklmnopqrstuvwxyz"
        "0123456789"; // now i COULD generate this using ascii stuff and iterate this but i dont really feel like it

        constexpr int charsetSize = sizeof(charset) - 1;

        std::string id;
        id.reserve(length);
        for (size_t i = 0; i < length; ++i) {
            id += charset[random::randint(0, charsetSize - 1)];
        }
        return id;
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

Brainrot::Age utilities::getBrainrotAge(unsigned int currentStars) {
    if (currentStars >= 20) return Brainrot::Age::Adult;
    if (currentStars >= 15) return Brainrot::Age::Teenager;
    if (currentStars >= 10) return Brainrot::Age::TikTokUser;
    if (currentStars >= 5)  return Brainrot::Age::Child;
    return Brainrot::Age::Baby;
}

// i put a lot of care into low chances because this will ONLY be used for distinguishible duplicates
std::string utilities::toRoman(int num) {
    std::unordered_map<int, std::string> romans = {
        {1000, "M"}, {900, "CM"}, {500, "D"}, {400, "CD"},
        {100, "C"}, {90, "XC"}, {50, "L"}, {40, "XL"},
        {10, "X"}, {9, "IX"}, {5, "V"}, {4, "IV"}, {1, "I"}
    };

    std::string res;
    for (const auto [value, symbol] : romans) {
        while (num >= value) {
            res += symbol;
            num -= value;
        }
    }

    return res;
}