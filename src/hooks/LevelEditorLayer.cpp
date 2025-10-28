#include <Geode/Geode.hpp>
#include <Geode/modify/LevelEditorLayer.hpp>

using namespace geode::prelude;

class $modify(HideBrainrots, LevelEditorLayer) {
    bool init(GJGameLevel* p0, bool p1) {
        if (!LevelEditorLayer::init(p0, p1)) return false;
        if (!Mod::get()->getSettingValue<bool>("brainrots-in-editor")) {
            auto scene = CCScene::get();
            if (scene) {
                auto baby = scene->getChildByID("brainrots"_spr);
                if (baby) {
                    baby->setVisible(false);
                }
            }
        }
        return true;
    }
};