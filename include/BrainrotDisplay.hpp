#pragma once

#include <Geode/Geode.hpp>

class BrainrotDisplay : cocos2d::CCLayerColor { // see your great brainrots.
private:
    bool init();
    void brainrotTouchCallback(CCMenuItemSpriteExtra* sender);
public:
    static BrainrotDisplay* create();
    static cocos2d::CCScene* scene();
};