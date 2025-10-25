#pragma once
#include <Geode/Geode.hpp> // including geode includes a lot of things from std so i just do that

struct BrainrotRegistry {
    static BrainrotRegistry* get();
    std::unordered_map<std::string, std::string> brainrots;
    std::unordered_map<std::string, std::string> brainrotNames; // IDs are separated from the names, for example lirili_larila would be "Lirili Larila"
};