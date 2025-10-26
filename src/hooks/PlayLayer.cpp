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
        float m_whenToPlace; // this determines when to place the brainrot.
        bool m_triggered;
        float m_elapsed;
        bool m_assignBrainrot; // when this is true, the mod will create brainrots, if not it would be called collection mode, collects brainrots!
        std::map<std::string, CCSprite*> m_collectibles;
        std::map<std::string, CCSprite*> m_collected;
    };

    bool init(GJGameLevel* level, bool useReplay, bool dontCreateObjects) {
        if (!PlayLayer::init(level, useReplay, dontCreateObjects)) return false;

        if (level->m_stars == 0) return true;

        SaveManager::get()->getCurrentSave();
        auto brianrots = SaveManager::get()->brainrotInLevel(fmt::to_string(level->m_levelID.value()));
        log::info("{}", brianrots);
        if (brianrots.empty()) {
            m_fields->m_assignBrainrot = true;
            return brainrotAssignment();
        }

        m_fields->m_assignBrainrot = false;

        return brainrotCollection(brianrots);
    }

    bool brainrotCollection(SaveManager::GamingComplexMap brainrots) {
        for (const auto [k, v] : brainrots) {
            // so uh theres no BrainrotCollectible it'll just be a ccsprite and playlayer detects when m_player1 or m_player 2 touch it
            auto sprite = CCSprite::create(BrainrotRegistry::get()->brainrots[k].c_str());
            auto x = numFromString<float>(v.at("x"));
            auto y = numFromString<float>(v.at("y"));
            if (x.isErr()) {
                log::error("Err: {}", x.err().value());
                return true;
            } else if (y.isErr()) {
                log::error("Err: {}", y.err().value());
                return true;
            }
            auto point = ccp(x.unwrap(), y.unwrap());
            sprite->setPosition(point);
            sprite->setID(fmt::format("{}{}", ""_spr, k));
            m_objectLayer->addChild(sprite);
            
            m_fields->m_collectibles.insert({k, sprite});
        }
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

        return brainrotCollectionUpdate(dt);
    }

    // this is the code that runs every frame if on assignment mode
    void brainrotAssignmentUpdate(float dt) {
        if (m_fields->m_triggered) return;

        m_fields->m_elapsed += dt;

        if (m_fields->m_elapsed >= m_fields->m_whenToPlace) {
            m_fields->m_triggered = true;

            auto pos = m_player1->getPosition();
            //auto spr = CCSprite::create("GJ_button_01.png");
            //spr->setPosition(pos);
            //this->m_objectLayer->addChild(spr);
            SaveManager::get()->getCollectedBrainrots();
            
            std::function<std::string()> ensureNoDupes = [&]() {
                static int dupes = 0;
                auto theChosenOne = utilities::random::choiceMap<std::string, std::string>(BrainrotRegistry::get()->brainrots).first;

                bool duplicate = false;

                for (const auto [k, v] : SaveManager::get()->getAllCollectedBrainrots()) {
                    if (theChosenOne == v.at("id")) {
                        duplicate = true;
                        dupes += 1;
                        break;
                    }
                }
                if (duplicate) {
                    if (dupes >= BrainrotRegistry::get()->brainrots.size()) {
                        return theChosenOne;
                    }
                    return ensureNoDupes();
                }
                dupes = 0;
                return theChosenOne;
            };
            auto theChosenOne = ensureNoDupes();
            SaveManager::get()->getCurrentSave(); // push current save into memory
            SaveManager::get()->pushChanges(fmt::to_string(m_level->m_levelID), theChosenOne, {
                {"x", fmt::to_string(pos.x)},
                {"y", fmt::to_string(pos.y)}
            });
            SaveManager::get()->commitChanges();
            // BRAINROT lore: brainrots follow geometry dash players scent (for whatever reason...).
        }
    }

    // this is the code that runs every frame if on collection mode
    void brainrotCollectionUpdate(float dt) {
        
        for (const auto [k, v] : m_fields->m_collectibles) {
            if (m_fields->m_collected.contains(k)) continue;

            bool collisionPlayer1 = m_player1->boundingBox().intersectsRect(v->boundingBox());
            bool collisionPlayer2 = m_player1->boundingBox().intersectsRect(v->boundingBox());

            bool caughtBrainrot = collisionPlayer1 || collisionPlayer2;
            if (caughtBrainrot) {
                m_fields->m_collected.insert({k, v});
                log::info("SNATCHED BRAINROT: {} (you probably dont need this but the pointer address to the sprite of the brainrot is `{}`)", BrainrotRegistry::get()->brainrotNames[k], fmt::ptr(v));
                v->setVisible(false);
            }
        }
    }

    void resetLevel() {
        PlayLayer::resetLevel();
        for (const auto [k, v] : m_fields->m_collected) v->setVisible(true);
        
        m_fields->m_collected.clear(); // while yes brainrots can spawn even if you die, you always still have to beat the level in order for the brainrot to save!
    }

    void levelComplete() {
        SaveManager::get()->getCollectedBrainrots();
        SaveManager::get()->getCurrentSave();
        for (const auto [k, v] : m_fields->m_collected) {
            auto tokens = SaveManager::get()->getAllTokens();
            std::string token;
            do {
                token = utilities::random::string(8);
            } while (std::find(tokens.begin(), tokens.end(), token) != tokens.end());

            int dupeNumber = 1;

            for (const auto [cock, v] : SaveManager::get()->getAllCollectedBrainrots()) {
                if (k == cock) ++dupeNumber;
            }

            SaveManager::get()->pushCollectedChanges(token, {
                {"id", k},
                {"dupe", fmt::to_string(dupeNumber)}, // the dupe key determines what number duplicate the brainrot is so it's distinguishable in brainrot display
                {"age", "baby"},
                {"collected-at", fmt::to_string(std::chrono::system_clock::to_time_t(std::chrono::system_clock::now()))},
                {"found-in", fmt::to_string(m_level->m_levelID.value())},
                {"stars", fmt::to_string(utilities::random::randint(0, 2))} // PIECE OF LORE BEHIND BRAINROTS: the only reason you see them wandering in levels is because their previous owners neglected them, which is why they have little to no stars.
            });
            SaveManager::get()->removeChange(fmt::to_string(m_level->m_levelID.value()), k);
        }
        SaveManager::get()->commitCollectedChanges();
        SaveManager::get()->commitChanges();

        PlayLayer::levelComplete();
    }
};