#include <Geode/Geode.hpp>
#include <Geode/modify/LevelInfoLayer.hpp>
#include <StealaBrainrot.hpp>

using namespace geode::prelude;

class $modify(InformOfBrainrot, LevelInfoLayer) {
    bool init(GJGameLevel* level, bool challenge) {
        if (!LevelInfoLayer::init(level, challenge)) return false;
        SaveManager::get()->getCurrentSave();
        for (const auto [k, v] : SaveManager::get()->brainrotInLevel(fmt::to_string(level->m_levelID.value()))) {
            auto alert = FLAlertLayer::create(
                utilities::_random::choice<std::string>({"Hmm...", "What's this?", "Something is lurking..."}).c_str(),
                fmt::format(
                    "You sense a strange being lurking around in this level.\n{}"
                    "You are filled with {}.",
                    v.contains("token") ? "Could it be... a lost brainrot?\n" : "",
                    utilities::_random::choice<std::string>({"the power of catching brainrots.", "the need to become the brainrot master.", "the desire to learn piano."})
                ),
                "okay..?"
            );
            alert->m_scene = this;
            alert->show();
        }
        
        return true;
    }
};