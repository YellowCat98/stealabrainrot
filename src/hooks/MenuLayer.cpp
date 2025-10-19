#include <Geode/Geode.hpp>
#include <Geode/modify/MenuLayer.hpp>
#include <StealaBrainrot.hpp>

using namespace geode::prelude;


class $modify(MyMenuLayer, MenuLayer) {
	bool init() {
		if (!MenuLayer::init()) return false;
		static bool addedBrainrots = false;
        if (addedBrainrots) return true;
		for (const auto [k, v] : BrainrotRegistry::get()->brainrots) {
            auto brainrot = Brainrot::create(k, Brainrot::Age::Baby);
            brainrot->allowWander(true);
            SceneManager::get()->keepAcrossScenes(brainrot);
        }
        addedBrainrots = true;
		

		return true;
	}
};