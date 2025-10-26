#pragma once

#include <Geode/Geode.hpp>
#include <Brainrot.hpp>

class BrainrotDisplay : public cocos2d::CCLayer { // see your great brainrots.
private:
    bool init();
    void brainrotTouchCallback(Brainrot* brainrot);
    virtual bool ccTouchBegan(cocos2d::CCTouch* touch, cocos2d::CCEvent* event) override;

    void onEnter() override;
    void onExit() override;
public:
    static BrainrotDisplay* create();
    static cocos2d::CCScene* scene();
    cocos2d::CCMenu* brainrots;
};