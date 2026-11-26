#include <Geode/Geode.hpp>
#include <Geode/modify/MenuLayer.hpp>
#include <StealaBrainrot.hpp>

using namespace geode::prelude;

$on_mod(Loaded) {
	BrainrotRegistry::get()->brainrots = {
		{"tralalero_tralala", "tralalero.png"_spr},
		{"tung_tung_tung_tung_sahur", "tung_sahur.png"_spr},
		{"bombardino_crocodilo", "bombardino.png"_spr},
		{"chimpanzini_bananini", "chimpanzini.png"_spr},
		{"lirili_larila", "lirili.png"_spr},
		{"cappuccino_assassino", "cappuccino.png"_spr}, 
		{"ballerina_cappuccina", "ballerina.png"_spr},
		{"trippi_troppi", "trippi.png"_spr},
		{"ta_ta_ta_sahur", "ta_sahur.png"_spr},
		{"trulimero_trulichina", "trulimero.png"_spr}
	};

	BrainrotRegistry::get()->brainrotNames = {
		{"tralalero_tralala", "Tralalero Tralala"},
		{"tung_tung_tung_tung_sahur", "Tung Tung Tung Tung Sahur"},
		{"bombardino_crocodilo", "Bombardino Crocodilo"},
		{"chimpanzini_bananini", "Chimpanzini Bananini"},
		{"lirili_larila", "Lirili Larila"},
		{"cappuccino_assassino", "Cappuccino Assassino"}, 
		{"ballerina_cappuccina", "Ballerina Cappuccina"},
		{"trippi_troppi", "Trippi Troppi"},
		{"ta_ta_ta_sahur", "Ta Ta Ta Sahur"},
		{"trulimero_trulichina", "Trulimero Trulichina"}
	};


	// handling runaway brainrots
	std::vector<std::string> runawayTokens; // so that i dont iterate through getallcollectedbrainrots and remove cause apparently i shouldnt remove items while iterating
	SaveManager::get()->getCollectedBrainrots();
	SaveManager::get()->getCurrentSave();
	for (const auto [k, v] : SaveManager::get()->getAllCollectedBrainrots()) { // okay HOW many times have i iterated through all the collected brainrots in this mod what the fuck (nvm its only 3 times)
		auto lastFed = static_cast<std::time_t>(numFromString<int>(v.at("last-fed")).unwrap());
		auto now = std::chrono::system_clock::now();

		auto trueLastFed = std::chrono::system_clock::from_time_t(lastFed);

		std::tm start_tm = {};
		start_tm.tm_year = 2026 - 1900;
		start_tm.tm_mon = 9;
		start_tm.tm_mday = 17;
		std::time_t start_ts = std::mktime(&start_tm);

		std::tm end_tm = {};
		end_tm.tm_year = 2026 - 1900;
		end_tm.tm_mon = 10;
		end_tm.tm_mday = 24;
		std::time_t end_ts = std::mktime(&end_tm);

		if (lastFed >= start_ts && lastFed <= end_ts) {
			if (v.at("age") != "tiktok-user") {
				BrainrotRegistry::get()->returnBrainrotsRanAway = true;
				auto tmp = v;
				tmp["last-fed"] = utils::numToString<std::time_t>(std::chrono::system_clock::to_time_t(now));
				SaveManager::get()->pushCollectedChanges(k, tmp);
				continue;
			}
		}

		int hours = v.at("age") == "tiktok-user" ? 24 : 48;
		if (std::chrono::duration_cast<std::chrono::hours>(now - trueLastFed).count() >= hours) {
			log::warn("{} {} has run away due to starvation!", BrainrotRegistry::get()->brainrotNames[v.at("id")], utilities::toRoman(numFromString<int>(v.at("dupe")).unwrap()));
			SaveManager::get()->pushChanges(v.at("found-in"), v.at("id"), {
				{"token", k}, // this key will be used to identify in the playlayer hook if a brainrot is a runaway brainrot
				{"age", v.at("age")},
				{"dupe", v.at("dupe")},
				{"x", v.at("x")},
				{"y", v.at("y")},
				{"collected-at", v.at("collected-at")},
				{"found-in", v.at("found-in")},
				{"stars", v.at("stars")},
				{"last-fed", v.at("last-fed")}
			});
			runawayTokens.push_back(k);
		}
	}

	for (auto token : runawayTokens) {
		SaveManager::get()->removeCollectedChange(token); // im too lazy to add a removeCollectedChanges function
		// or maybe NOT i am not lazy to add a removeCollectedChanges function
	}
	bool found = false;
	for (const auto [k, v] : SaveManager::get()->uncommitted) {
		for (const auto [cock, vee] : SaveManager::get()->brainrotInLevel(k)) {
			if (vee.contains("token")) {
				BrainrotRegistry::get()->brainrotsRanAway = true;
				found = true;
				break;
			}
		}
		if (found) break;
	}

	SaveManager::get()->commitChanges();
	SaveManager::get()->commitCollectedChanges();
}

$on_game(TexturesLoaded) {
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
}