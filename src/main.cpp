#include <Geode/Geode.hpp>
#include <Geode/modify/MenuLayer.hpp>

using namespace geode::prelude;

$on_mod(Loaded) {
	BrainrotRegistry::brainrots = {
		{"tralalelo_tralala", "tralalelo.png"_spr},
		{"tung_tung_sahur", "tung_sahur.png"_spr},
		{"bombardino_crocodilo", "bombardino.png"_spr},
		{"chimpanzini_bananini", "chimpanzini.png"_spr},
		{"lirili_larila", "lirili.png"_spr},
		{"cappuccino_assassino", "cappuccino.png"_spr},
		{"ballerina_cappuccina", "ballerina.png"_spr}
	};
}

class $modify(MyMenuLayer, MenuLayer) {
	bool init() {
		if (!MenuLayer::init()) {
			return false;
		}

		log::debug("Hello from my MenuLayer::init hook! This layer has {} children.", this->getChildrenCount());

		auto myButton = CCMenuItemSpriteExtra::create(
			CCSprite::createWithSpriteFrameName("GJ_likeBtn_001.png"),
			this,
			menu_selector(MyMenuLayer::onMyButton)
		);

		auto menu = this->getChildByID("bottom-menu");
		menu->addChild(myButton);

		myButton->setID("my-button"_spr);

		menu->updateLayout();

		return true;
	}

	void onMyButton(CCObject*) {
		FLAlertLayer::create("Geode", "Hello from my custom mod!", "OK")->show();
	}
};