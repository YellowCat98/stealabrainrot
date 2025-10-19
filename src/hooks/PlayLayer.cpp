#include <Geode/Geode.hpp>
#include <Geode/modify/PlayLayer.hpp>
#include <StealaBrainrot.hpp>

using namespace geode::prelude;

// here's how this works internally:
// basically, if the level has stars, at any random point while playing
// it stores a random position that you were just in inside your save
// when you play the second time, a BrainrotCollectible will appear
// brainrot collectibles can be collected when m_player1 or m_player2 passes through it (ONLY those 2, if i allow every playerobject to collect it then globed will inerfere i thinks)
class $modify(InsertBrainrot, PlayLayer) {
    struct Fields {
        float m_whenToPlace; // this determines where to place the brainrot.
        bool m_triggered;
        float m_elapsed;
    };

    bool init(GJGameLevel* level, bool useReplay, bool dontCreateObjects) {
        if (!PlayLayer::init(level, useReplay, dontCreateObjects)) return false;

        if (level->m_stars == 0) return true;
        
        int category = level->m_levelLength;
        float duration = 0.0f;
        log::info("category: {}", category);
        if (category == 0) return true; // brainrots cannot be collected on tiny or short levels.
        else if (category == 1) return true;
        else if (category == 2) duration = 30.0f;
        else if (category == 3) duration = 60.0f;
        else duration = 120.0f; // in platformer levels, brainrots can only spawn within the first 120 seconds.

        m_fields->m_whenToPlace = 10.0f + utilities::random::random(0.0f, 1.0f) * (duration - 10.0f);

        log::info("{}", m_fields->m_whenToPlace);

        return true;
    }

    void postUpdate(float dt) {
        PlayLayer::postUpdate(dt);
        
        if (m_fields->m_triggered) return;

        m_fields->m_elapsed += dt;

        if (m_fields->m_elapsed >= m_fields->m_whenToPlace) {
            m_fields->m_triggered = true;
            auto spr = CCSprite::create("GJ_button_01.png");
            auto pos = m_player1->getPosition();
            spr->setPosition(pos);
            this->m_objectLayer->addChild(spr);

            auto theChosenOne = utilities::random::choiceMap<std::string, std::string>(BrainrotRegistry::get()->brainrots).first;
            auto currentSavedValue = Mod::get()->getSavedValue<std::unordered_map<std::string, std::vector<std::map<std::string, std::string>>>>("brainrots-to-show");
            currentSavedValue[fmt::to_string(m_level->m_levelID.value())].push_back({
                {"id", theChosenOne},
                {"x", fmt::to_string(pos.x)},
                {"y", fmt::to_string(pos.y)}
            });
            //currentSavedValue.insert({fmt::to_string(m_level->m_levelID.value()), vec});
            Mod::get()->setSavedValue<std::unordered_map<std::string, std::vector<std::map<std::string, std::string>>>>("brainrots-to-show", currentSavedValue);
        }
    }
};