#include <Geode/Geode.hpp>
#include <Geode/modify/EditLevelLayer.hpp>

using namespace geode::prelude;

class $modify(ShowBrainrots, EditLevelLayer) {
    bool init(GJGameLevel* p0) {
        if (!EditLevelLayer::init(p0)) return false;
        if (!Mod::get()->getSettingValue<bool>("brainrots-in-editor")) {
            queueInMainThread([]() {
            auto scene = CCScene::get();
            if (scene) {
                auto baby = scene->getChildByID("brainrots"_spr);
                if (baby) {
                    baby->setVisible(true);
                }
            }
            });
        } 
        return true;
    }
};