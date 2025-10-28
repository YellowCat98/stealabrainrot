#include <Geode/Geode.hpp>
#include <Geode/modify/MenuLayer.hpp>
#include <StealaBrainrot.hpp>

using namespace geode::prelude;


class $modify(MyMenuLayer, MenuLayer) {
	bool init() {
		if (!MenuLayer::init()) return false;

        auto menu = this->getChildByID(utilities::random::choice<std::string>({"bottom-menu", "side-menu", "right-side-menu"}));
        menu->addChild(CCMenuItemExt::createSpriteExtraWithFilename("player456_smile.png"_spr, 1.0f, [](CCMenuItemSpriteExtra*) {
            CCDirector::get()->pushScene(CCTransitionFade::create(0.5f, BrainrotDisplay::scene()));
        }));
        menu->updateLayout();

        static bool informedRunaway = false;
        log::info("{}", informedRunaway);
        log::info("{}", BrainrotRegistry::get()->brainrotsRanAway);
        if (!informedRunaway) {
            if (BrainrotRegistry::get()->brainrotsRanAway) {
                Notification::create("It appears one or more brainrots have ran away...", NotificationIcon::Warning)->show();
            }
            informedRunaway = true;
        }

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
                    auto brainrot = Brainrot::create(v.at("id"), k, utilities::stringToAge(v.at("age")), "");
                    brainrot->setID(fmt::format("{}{}-{}", ""_spr, v.at("id"), k));
                    SceneManager::get()->keepAcrossScenes(brainrot);
                }
                break;
            }
            case 1: {
                auto brainrots = SaveManager::get()->getAllCollectedBrainrots();
                if (brainrots.empty()) break;
                auto brainrotID = Mod::get()->getSavedValue<std::string>("equipped-brainrot");
                if (!brainrots.contains(brainrotID)) break;
                auto brainrot = Brainrot::create(brainrots[brainrotID].at("id"), brainrotID, utilities::stringToAge(SaveManager::get()->getCollectedBrainrot(brainrotID).at("age")), "");
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