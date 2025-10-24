#include <SaveManager.hpp>

using namespace geode::prelude;

SaveManager* SaveManager::get() {
    static SaveManager* instance = nullptr;
    if (!instance) instance = new SaveManager();
    return instance;
}

void SaveManager::getCurrentSave() {
    this->uncommitted = Mod::get()->getSavedValue<ComplexMap>("brainrots-to-show");
    this->getCurrentSaveCalled = true;
}

void SaveManager::pushChanges(const std::string& levelID, const std::string& brainrotID, MapData map) {
    if (!this->getCurrentSaveCalled) return; // if i didnt have this here not calling getCurrentSave will just override the past changes
    this->uncommitted[levelID][brainrotID] = map;
}

void SaveManager::commitChanges() {
    Mod::get()->setSavedValue<ComplexMap>("brainrots-to-show", this->uncommitted);
    this->uncommitted.clear();
    this->getCurrentSaveCalled = false;
}

SaveManager::GamingComplexMap SaveManager::brainrotInLevel(const std::string& levelID) {
    if (!this->getCurrentSaveCalled) return {};
    if (!this->uncommitted.contains(levelID)) return {};

    return this->uncommitted[levelID];
}

void SaveManager::removeChange(const std::string& levelID, const std::string& brainrotID) {
    this->uncommitted.at(levelID).erase(brainrotID);
    if (this->uncommitted.at(levelID).empty()) this->uncommitted.erase(levelID);
}

// anything under this comment is about collected brainrots

void SaveManager::getCollectedBrainrots() {
    this->collectedUncommitted = Mod::get()->getSavedValue<GamingComplexMap>("collected-brainrots");
    this->getCollectedCalled = true;
}

void SaveManager::pushCollectedChanges(const std::string& brainrotID, SaveManager::MapData map) {
    if (!this->getCollectedCalled) return;
    this->collectedUncommitted[brainrotID] = map;
}

void SaveManager::commitCollectedChanges() {
    Mod::get()->setSavedValue<GamingComplexMap>("collected-brainrots", this->collectedUncommitted);
    this->getCollectedCalled = false;
}

SaveManager::GamingComplexMap SaveManager::getAllCollectedBrainrots() {
    return this->collectedUncommitted;
}

SaveManager::MapData SaveManager::getCollectedBrainrot(const std::string& brainrotID) {
    auto allBrainrots = this->getAllCollectedBrainrots();
    for (const auto [k, v] : allBrainrots) {
        if (k == brainrotID) return v;
    }
    return {}; // brainrot of that ID was not found.
}