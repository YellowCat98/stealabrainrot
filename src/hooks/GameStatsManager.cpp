#include <Geode/Geode.hpp>
#include <Geode/modify/GameStatsManager.hpp>
#include <StealaBrainrot.hpp>

using namespace geode::prelude;

class $modify(RaiseBrainrot, GameStatsManager) {
    void incrementStat(char const* p0, int p1) {
        GameStatsManager::incrementStat(p0, p1);
        if (strcmp(p0, "28") == 0 || strcmp(p0, "6") == 0) {
            log::info("star {}", p1);
            auto equippedBrainrot = Mod::get()->getSavedValue<std::string>("equipped-brainrot");
            SaveManager::get()->getCollectedBrainrots();
            auto brainrotData = SaveManager::get()->getCollectedBrainrot(equippedBrainrot);
            if (brainrotData.empty()) return;
            if (brainrotData.contains("stars")) {
                auto starsAmount = numFromString<float>(brainrotData["stars"]);
                if (starsAmount.isErr()) {
                    log::error("Err: {}", starsAmount.err().value());
                    return;
                }
                brainrotData["stars"] = fmt::to_string(starsAmount.unwrap() += p1);
                SaveManager::get()->pushCollectedChanges(equippedBrainrot, brainrotData);
                log::info("{}", brainrotData);
                SaveManager::get()->commitCollectedChanges();
            }
        }
    }
};