#include <Geode/Geode.hpp>
#include <Geode/modify/MenuLayer.hpp>
#include <StealaBrainrot.hpp>

using namespace geode::prelude;


class $modify(MyMenuLayer, MenuLayer) {
	bool init() {
		if (!MenuLayer::init()) return false;
		
		static Brainrot* brainrot;
		if (!brainrot) {
			brainrot = Brainrot::create("h");
			brainrot->allowWander(true);
			SceneManager::get()->keepAcrossScenes(brainrot);
		}
		

		return true;
	}
};