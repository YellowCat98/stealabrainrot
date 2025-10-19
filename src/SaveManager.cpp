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

void SaveManager::pushChanges(std::string levelID, MapData map) {
    if (!this->getCurrentSaveCalled) return; // if i didnt have this here not calling getCurrentSave will just override the past changes
    this->uncommitted[levelID].push_back(map);
}

void SaveManager::commitChanges() {
    Mod::get()->setSavedValue<ComplexMap>("brainrots-to-show", this->uncommitted);
    this->uncommitted.clear();
}