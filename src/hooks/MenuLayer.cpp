#include <Geode/Geode.hpp>
#include <Geode/modify/MenuLayer.hpp>
#include <StealaBrainrot.hpp>

using namespace geode::prelude;


class $modify(MyMenuLayer, MenuLayer) {
	bool init() {
		if (!MenuLayer::init()) return false;

        auto menu = this->getChildByID("bottom-menu");
        menu->addChild(CCMenuItemExt::createSpriteExtraWithFilename("player456_smile.png"_spr, 1.0f, [](CCMenuItemSpriteExtra*) {
            CCDirector::get()->pushScene(CCTransitionFade::create(0.5f, BrainrotDisplay::scene()));
        }));
        menu->updateLayout();

		static bool addedBrainrots = false;
        if (addedBrainrots) return true;
        /*
		for (const auto [k, v] : BrainrotRegistry::get()->brainrots) {
            auto brainrot = Brainrot::create(k, Brainrot::Age::Baby);
            brainrot->allowWander(true);
            SceneManager::get()->keepAcrossScenes(brainrot);
        }
        addedBrainrots = true;
		*/
        SaveManager::get()->getCollectedBrainrots();
        switch (Mod::get()->getSettingValue<int>("brainrots-display")) { // you no longer have to do int64_t
            case 0: {
                auto brainrots = SaveManager::get()->getAllCollectedBrainrots();
                if (brainrots.empty()) break;
                for (const auto [k, v] : brainrots) {
                    auto brainrot = Brainrot::create(v.at("id"), utilities::stringToAge(v.at("age")), "");
                    brainrot->setID(fmt::format("{}{}-{}", ""_spr, v.at("id"), k));
                    SceneManager::get()->keepAcrossScenes(brainrot);
                }
                break;
            }
            case 1: {
                auto brainrots = SaveManager::get()->getAllCollectedBrainrots();
                if (brainrots.empty()) break;
                auto brainrotID = Mod::get()->getSavedValue<std::string>("equipped-brainrot");
                auto brainrot = Brainrot::create(brainrots[brainrotID].at("id"), utilities::stringToAge(SaveManager::get()->getCollectedBrainrot(brainrotID).at("age")), "");
                brainrot->setID(fmt::format("{}{}", ""_spr, brainrotID));
                SceneManager::get()->keepAcrossScenes(brainrot);
                break;
            }
            default: break; // anything after 1 will just be invisible!
        }

        addedBrainrots = true;
		return true;
	}
};