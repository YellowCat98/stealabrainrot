#pragma once

#include <Geode/Geode.hpp>

struct BrainrotRegistry {
    static std::unordered_map<std::string, std::string> brainrots;
    static std::vector<Brainrot*> earnedBrainrots;
};

class Brainrot : public cocos2d::CCNode {
private:
    cocos2d::CCSprite* brainrotImage;
    std::string brainrotID;
    bool init(cocos2d::CCSprite* brainrotImage, const std::string& brainrotID);
public:
    static Brainrot* create(cocos2d::CCSprite* brainrotImage, const std::string& brainrotID);
    void allowWander(bool toggle); // brainrots can wander around the screen if this is enabled, if not, they remain in a random position
    cocos2d::CCSprite* getSprite(); // im too lazy to move brainrotImage to public
};