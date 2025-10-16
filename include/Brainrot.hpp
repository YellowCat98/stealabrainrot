#pragma once

#include <Geode/Geode.hpp>

struct BrainrotRegistry {
    static std::unordered_map<std::string, std::string> brainrots;
    static std::vector<Brainrot*> earnedBrainrots;
};

class Brainrot : public cocos2d::CCSprite {
private:
    std::string brainrotID;
    bool init(const std::string& brainrotID);
public:
    static Brainrot* create(const std::string& brainrotID);
    void allowWander(bool toggle); // brainrots can wander around the screen if this is enabled, if not, they remain in a random position
};