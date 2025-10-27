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
		int hours = v.at("age") == "tiktok-user" ? 24 : 48;
		if (std::chrono::duration_cast<std::chrono::hours>(now - trueLastFed).count() >= 48) {
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
	}

	SaveManager::get()->commitChanges();
	SaveManager::get()->commitCollectedChanges();
}