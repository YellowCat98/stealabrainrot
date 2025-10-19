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
        bool m_assignBrainrot; // when this is true, the mod will create brainrots, if not it would be called collection mode, collects brainrots!
    };

    bool init(GJGameLevel* level, bool useReplay, bool dontCreateObjects) {
        if (!PlayLayer::init(level, useReplay, dontCreateObjects)) return false;

        if (level->m_stars == 0) return true;

        SaveManager::get()->getCurrentSave();
        log::info("{}", SaveManager::get()->uncommitted);
        auto brianrots = SaveManager::get()->brainrotInLevel(fmt::to_string(level->m_levelID.value()));
        log::info("{}", brianrots);
        
        if (brianrots.empty()) {
            m_fields->m_assignBrainrot = true;
            return brainrotAssignment();
        }

        m_fields->m_assignBrainrot = false;

        return brainrotCollection();
    }

    bool brainrotCollection() {
        return true;
    }

    bool brainrotAssignment() {
        int category = m_level->m_levelLength;
        float duration = 0.0f;
        if (category == 0) return true; // brainrots cannot be collected on tiny or short levels.
        else if (category == 1) return true;
        else if (category == 2) duration = 30.0f;
        else if (category == 3) duration = 60.0f;
        else duration = 120.0f; // in platformer levels, brainrots can only spawn within the first 120 seconds.

        m_fields->m_whenToPlace = 10.0f + utilities::random::random(0.0f, 1.0f) * (duration - 10.0f);

        return true;
    }

    void postUpdate(float dt) {
        PlayLayer::postUpdate(dt);
        if (m_fields->m_assignBrainrot) return brainrotAssignmentUpdate(dt);

        brainrotCollectionUpdate();
    }

    // this is the code that runs every frame if on assignment mode
    void brainrotAssignmentUpdate(float dt) {
        if (m_fields->m_triggered) return;

        m_fields->m_elapsed += dt;

        if (m_fields->m_elapsed >= m_fields->m_whenToPlace) {
            m_fields->m_triggered = true;
            auto spr = CCSprite::create("GJ_button_01.png");
            auto pos = m_player1->getPosition();
            spr->setPosition(pos);
            this->m_objectLayer->addChild(spr);

            auto theChosenOne = utilities::random::choiceMap<std::string, std::string>(BrainrotRegistry::get()->brainrots).first;

            SaveManager::get()->getCurrentSave(); // push current save into memory
            SaveManager::get()->pushChanges(fmt::to_string(m_level->m_levelID), {
                {"id", theChosenOne},
                {"x", fmt::to_string(pos.x)},
                {"y", fmt::to_string(pos.y)}
            });
            SaveManager::get()->commitChanges();
        }
    }

    // this is the code that runs every frame if on collection mode
    void brainrotCollectionUpdate() {
        log::info("toilette great");
    }
};