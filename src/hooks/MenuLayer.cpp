#include <Geode/Geode.hpp>
#include <Geode/modify/MenuLayer.hpp>
#include <StealaBrainrot.hpp>

using namespace geode::prelude;


class $modify(MyMenuLayer, MenuLayer) {
	bool init() {
		if (!MenuLayer::init()) return false;

        auto menu = this->getChildByID(utilities::_random::choice<std::string>({"bottom-menu", "side-menu", "right-side-menu"}));
        auto btn = CCMenuItemExt::createSpriteExtraWithFilename("player456_smile.png"_spr, 1.0f, [](CCMenuItemSpriteExtra*) {
            CCDirector::get()->pushScene(CCTransitionFade::create(0.5f, BrainrotDisplay::scene()));
        });
        btn->setID("brainrot-lair"_spr);
        menu->addChild(btn);
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

        static bool informedReturnedBrainrots = false;
        if (!informedReturnedBrainrots) {
            if (BrainrotRegistry::get()->returnBrainrotsRanAway) {
                auto alert = FLAlertLayer::create(
                    "Brainrot Return",
                    "Due to update 2.2081,\n"
                    "you were unable to feed your brainrots.\n"
                    "Your collected brainrots have understood your reason and returned.\n"
                    "Brainrots that have ran away before the update will not return.\n"
                    "TikTok User brainrots have decided not to return.\n"
                    "You shall collect them again.",
                    "OK"
                );
                alert->m_scene = this;
                alert->show();
            }
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

        auto brainrotMenu = CCNode::create();
        brainrotMenu->setContentSize(CCDirector::get()->getWinSize());
        brainrotMenu->setID("brainrots"_spr);

        SaveManager::get()->getCollectedBrainrots();
        switch (Mod::get()->getSettingValue<int>("brainrots-display")) { // you no longer have to do int64_t
            case 0: {
                auto brainrots = SaveManager::get()->getAllCollectedBrainrots();
                if (brainrots.empty()) break;
                for (const auto [k, v] : brainrots) {
                    auto brainrot = Brainrot::create(v.at("id"), k, utilities::stringToAge(v.at("age")), "");
                    brainrot->setID(fmt::format("{}-{}", v.at("id"), k));
                    brainrotMenu->addChild(brainrot);
                }
                break;
            }
            case 1: {
                auto brainrots = SaveManager::get()->getAllCollectedBrainrots();
                if (brainrots.empty()) break;
                auto brainrotID = Mod::get()->getSavedValue<std::string>("equipped-brainrot");
                if (!brainrots.contains(brainrotID)) break;
                auto brainrot = Brainrot::create(brainrots[brainrotID].at("id"), brainrotID, utilities::stringToAge(SaveManager::get()->getCollectedBrainrot(brainrotID).at("age")), "");
                brainrot->setID(fmt::format("{}", brainrotID));
                brainrotMenu->addChild(brainrot);
                break;
            }
            default: break; // anything after 1 will just be invisible!
        }
        OverlayManager::get()->addChild(brainrotMenu);

        addedBrainrots = true;
		return true;
	}
};