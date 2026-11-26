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
        if (!informedRunaway) {
            if (BrainrotRegistry::get()->brainrotsRanAway) {
                Notification::create("It appears one or more brainrots have ran away...", NotificationIcon::Warning)->show();
            }
            informedRunaway = true;
        }

        if (!Mod::get()->setSavedValue<bool>("informed-returning-brainrot-22091", true)) {
            static bool informedReturnedBrainrots = false;
            if (!informedReturnedBrainrots) {
                if (BrainrotRegistry::get()->returnBrainrotsRanAway) {
                    auto alert = FLAlertLayer::create(
                        "Brainrot Return",
                        "Due to update 2.2091,\n"
                        "you were unable to feed your brainrots.\n"
                        "Your collected brainrots have understood your reason and returned.\n"
                        "Brainrots that have ran away before the update will not return.\n"
                        "TikTok User brainrots have decided not to return.\n"
                        "You shall collect them again.",
                        utilities::_random::choice<std::string>({"are you fr rn", "OK", "yay!", "the", "d'accord"}).c_str()
                    );
                    alert->m_scene = this;
                    alert->show();
                }
                informedReturnedBrainrots = true;
            }
        }
		return true;
	}
};