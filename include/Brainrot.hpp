#pragma once

#include <Geode/Geode.hpp>



class Brainrot : public cocos2d::CCSprite {
private:
    enum class Age : int; // since when the hell was "underlying type" a thing in enums
    std::string brainrotID;
    bool init(const std::string& brainrotID, Age age);
    bool canWander = false;
    void update(float dt);

    cocos2d::CCPoint velocity;
    float speed;

public:

    enum class Age {
        Baby, // every brainrot starts off with this
        Child, // requires 5 stars/moons
        TikTokUser, // requires 10 stars/moons
        Teenager, // requires 15 stars/moons
        Adult // requires 25 stars/moons
    };
    static Brainrot* create(const std::string& brainrotID, Age age);
    void allowWander(bool toggle); // brainrots can wander around the screen if this is enabled, if not, they remain in a random position
};