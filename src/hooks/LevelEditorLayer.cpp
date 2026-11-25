#include <Geode/Geode.hpp>
#include <Geode/modify/LevelEditorLayer.hpp>

using namespace geode::prelude;

class $modify(HideBrainrots, LevelEditorLayer) {
    // thanks undefined
    struct Fields {
        ~Fields() {
            auto scene = OverlayManager::get();
            auto baby = scene->getChildByID("brainrots"_spr);
            if (baby) {
                baby->setVisible(true);
            }
        }
    };
    bool init(GJGameLevel* p0, bool p1) {
        if (!LevelEditorLayer::init(p0, p1)) return false;
        m_fields.self();
        if (Mod::get()->getSettingValue<bool>("brainrots-in-editor")) {
            auto scene = OverlayManager::get();
            auto baby = scene->getChildByID("brainrots"_spr);
            if (baby) {
                baby->setVisible(false);
            }
        }
        return true;
    }
    
    void onBack() {

    }
};