#pragma once

#include <Geode/Geode.hpp>



class Brainrot : public cocos2d::CCSprite {
private:
    std::string brainrotID;
    bool init(const std::string& brainrotID);
    bool canWander = false;
    void update(float dt);

    cocos2d::CCPoint velocity;
    float speed;

public:
    static Brainrot* create(const std::string& brainrotID);
    void allowWander(bool toggle); // brainrots can wander around the screen if this is enabled, if not, they remain in a random position
};