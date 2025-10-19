#pragma once
#include <vector>
#include <Brainrot.hpp>

namespace utilities {
    namespace random {
        float random(float a, float b);
        int randint(int a, int b);
        
        // shitty c++ forces you to declare templates in headers otherwise it wont compile unless i specifically declare the types it supports
        // do i even need this function
        template<typename T>
        T choice(const std::vector<T>& vec) {
            int i = random::randint(0, (static_cast<int>(vec.size()) - 1));
            return vec[i];
        }

        template<typename K, typename V>
        std::pair<K, V> choiceMap(std::unordered_map<K, V> vec) {
            int i = random::randint(0, (static_cast<int>(vec.size()) - 1));
            auto it = vec.begin();
            std::advance(it, i);
            return *it;
        }
    }

    Brainrot::Age stringToAge(const std::string& age);
}