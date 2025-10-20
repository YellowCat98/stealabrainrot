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
}