#pragma once

#include <Geode/Geode.hpp>
#include <SaveManager.hpp>



class Brainrot : public cocos2d::CCSprite {

public:

    enum class Age {
        Baby, // every brainrot starts off with this
        Child, // requires 5 stars/moons
        TikTokUser, // requires 10 stars/moons
        Teenager, // requires 15 stars/moons
        Adult // requires 25 stars/moons
    };
    static Brainrot* create(const std::string& brainrotID, const std::string& token, Age age, const std::string& nameAboveHead);
    void setLabelColor(const cocos2d::ccColor3B& col);
    std::string brainrotID;
    std::string brainrotToken;
private:
    bool init(const std::string& brainrotID, const std::string& token, Age age, const std::string& nameAboveHead);
    void update(float dt);

    cocos2d::CCPoint velocity;
    float speed;
    cocos2d::CCLabelBMFont* label;
};