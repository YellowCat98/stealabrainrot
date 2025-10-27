#include <BrainrotDisplay.hpp>
#include <StealaBrainrot.hpp>

using namespace geode::prelude;



bool BrainrotDisplay::init() {
    if (!CCLayer::init()) return false;
    this->setKeypadEnabled(true);

    auto bg = MenuGameLayer::create();
    bg->setID("background");
    bg->setRotation(utilities::random::randint(0, 1) ? 180.0f : bg->getRotation()); // basically a 1/2 chance the background is rotated

    auto shownBrainrots = SceneManager::get()->getPersistedNodes();
    for (auto& node : shownBrainrots) {
        node->setVisible(false); // Should you be in the brainrot display, thou brainrot will not be visible.
    }

    auto brainrots = CCMenu::create();
    brainrots->setPosition({0.0f, 0.0f});
    brainrots->setID("brainrots");
    brainrots->setZOrder(1);

    SaveManager::get()->getCollectedBrainrots();
    for (const auto [k, v] : SaveManager::get()->getAllCollectedBrainrots()) {
        auto brainrot = Brainrot::create(v.at("id"),
            k,
            utilities::stringToAge(v.at("age")),
            fmt::format("{} {}", BrainrotRegistry::get()->brainrotNames[v.at("id")],
                utilities::toRoman(numFromString<int>(v.at("dupe")).unwrap())
            )
        );
        if (k == Mod::get()->getSavedValue<std::string>("equipped-brainrot")) brainrot->setLabelColor({ 0, 255, 13 });

        brainrot->setID(fmt::format("{}-{}", v.at("id"), k));

        brainrots->addChild(brainrot);
    }

    // i was too lazy so i took the back button code from my other mod
    // https://github.com/YellowCat98/SerpentLua/blob/9c782168cd141faed594baf647ba693d13265b9a/src/internal/ui/ScriptsLayer.cpp#L173C1-L191C81

    auto backMenu = CCMenu::create();
    backMenu->setID("back-menu");
    
    auto backBtn = CCMenuItemExt::createSpriteExtra(CCSprite::createWithSpriteFrameName(utilities::random::randint(0, 1) ? "GJ_arrow_01_001.png" : "GJ_arrow_02_001.png"), [this](CCObject*) {
        this->keyBackClicked();
    });
    backMenu->addChild(backBtn);

    // some parts of the quit button code is taken from geode's own source code
    // more specifically at
    // https://github.com/geode-sdk/geode/blob/0da07978549a898c98ff54efdeb08dda3dc55787/loader/src/ui/mods/ModsLayer.cpp#L371

    backMenu->setAnchorPoint({0.0f, 0.5f});

    backMenu->setLayout(SimpleRowLayout::create()
        ->setMainAxisAlignment(MainAxisAlignment::Start)
        ->setGap(5.0f)
    );

    auto infoMenu = CCMenu::create();
    infoMenu->setID("info-menu");
    infoMenu->setAnchorPoint({1.0f, 0.0f});

    auto infoBtn = CCMenuItemExt::createSpriteExtra(CCSprite::createWithSpriteFrameName("GJ_infoIcon_001.png"), [this](CCMenuItemSpriteExtra* self) {
        // i got a little bored so erm
        #ifdef GEODE_IS_WINDOWS
            bool runningOnLinux = (std::getenv("WINEPREFIX") || std::getenv("WINELOADER") || std::getenv("WINEDEBUG") || GetProcAddress(GetModuleHandleA("ntdll.dll"), "wine_get_version"));
        #elif GEODE_IS_ANDROID
            bool runningOnLinux = true;
        #else
            bool runningOnLinux = false;
        #endif
        FLAlertLayer::create(
            utilities::random::randint(0, 1) ? "Info" : "Information",
            "Here lies all your collected brainrots.\n"
            "Duplicate brainrots are distinguished via the roman numeral after their name.\n"
            "The brainrot whose name is green is the current equipped brainrot.\n"
            "Press on a brainrot to view information about it and equip it.\n",
            // ok you know i have to do the random thingy here too!
            utilities::random::choice<std::string>({"Understood!", "Wha..?", "i think i got it!", "OK", "Absolute OK!", (runningOnLinux ? "i use arch btw" : "i use NT btw"), "Pay 250 Robux to unlock VIP"}).c_str()
        )->show();
    });

    infoMenu->addChild(infoBtn);

    infoMenu->setLayout(SimpleRowLayout::create()
        ->setMainAxisAlignment(MainAxisAlignment::End)
        ->setCrossAxisAlignment(CrossAxisAlignment::End)
        ->setGap(5.0f)
    );

    this->brainrots = brainrots;
    this->addChild(brainrots);
    this->addChild(bg);
    this->addChildAtPosition(backMenu, Anchor::TopLeft, {12.0f, -25.0f}, false);
    this->addChildAtPosition(infoMenu, Anchor::BottomRight, {-10.0f, 10.0f}, false);
    return true;
}

void BrainrotDisplay::keyBackClicked() {
    CCDirector::get()->popSceneWithTransition(0.5f, kPopTransitionFade);
}

void BrainrotDisplay::onEnter() {
    CCLayer::onEnter();
    CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, 0, true);
}

void BrainrotDisplay::onExit() {
    CCDirector::sharedDirector()->getTouchDispatcher()->removeDelegate(this);
    CCLayer::onExit();
}

bool BrainrotDisplay::ccTouchBegan(CCTouch* touch, CCEvent* event) {
    auto location = touch->getLocation();
    CCObject* obj = nullptr;
    CCARRAY_FOREACH(brainrots->getChildren(), obj) {
        auto brainrot = static_cast<Brainrot*>(obj);
        if (!brainrot->isVisible()) continue;

        if (brainrot->boundingBox().containsPoint(location)) {
            this->brainrotTouchCallback(brainrot);
            return true;
        }
    }
    return true;
}

void BrainrotDisplay::brainrotTouchCallback(Brainrot* brainrot) {
    auto token = brainrot->brainrotToken;
    auto id = brainrot->brainrotID;

    auto brainrotData = SaveManager::get()->getCollectedBrainrot(token);

    auto previousToken = Mod::get()->getSavedValue<std::string>("equipped-brainrot");


    geode::createQuickPopup(
        "Brainrot Info",
        fmt::format(
            "Name: {}\n"
            "ID: {}\n"
            "Age: {}\n"
            "Caught at: {}\n"
            "Last fed: {}\n"
            "Stars/Moons fed: {}\n"
            "Status: {}",
            fmt::format("{} {}", BrainrotRegistry::get()->brainrotNames[brainrotData.at("id")], utilities::toRoman(numFromString<int>(brainrotData.at("dupe")).unwrap())),
            brainrotData.at("id"),
            brainrotData.at("age"),
            fmt::format("{:%Y-%m-%d %H:%M:%S}", std::chrono::floor<std::chrono::seconds>(std::chrono::system_clock::from_time_t(static_cast<std::time_t>(numFromString<int>(brainrotData.at("collected-at")).unwrap())))),
            fmt::format("{:%Y-%m-%d %H:%M:%S}", std::chrono::floor<std::chrono::seconds>(std::chrono::system_clock::from_time_t(static_cast<std::time_t>(numFromString<int>(brainrotData.at("last-fed")).unwrap())))),
            brainrotData.at("stars"),
            previousToken == token ? "Equipped" : "Not equipped"
        ),
        "OK", "EQUIP",
        [=](FLAlertLayer*, bool btn2) {
            if (btn2) {
                createQuickPopup(
                    "Confirmation",
                    fmt::format(
                        "Are you sure you want to equip {} {}?",
                        BrainrotRegistry::get()->brainrotNames[brainrotData.at("id")], utilities::toRoman(numFromString<int>(brainrotData.at("dupe")).unwrap())
                    ),
                    utilities::random::choice<std::string>({"No!!", "i missclicked", "surely not", "i do not agree", "no, smh.."}).c_str(),
                    utilities::random::choice<std::string>({"OK", "Okay", "Duh??", "absolutely yes", "why would i click the button duh??", "Alright!", "k", "alr", fmt::format("{} says absolutely!", GJAccountManager::get()->m_username)}).c_str(),
                    [=](FLAlertLayer*, bool btn2) {
                        if (btn2) {
                                if (SaveManager::get()->getAllCollectedBrainrots().contains(previousToken)) {
                                    auto previousID = SaveManager::get()->getCollectedBrainrot(previousToken).at("id");
        
                                    auto previousBrainrot = static_cast<Brainrot*>(brainrots->getChildByID(fmt::format("{}-{}", previousID, previousToken)));

                                    previousBrainrot->setLabelColor({255, 255, 255});
                                }
                            brainrot->setLabelColor({ 0, 255, 13 });
                            Mod::get()->setSavedValue<std::string>("equipped-brainrot", token);
                            Notification::create(fmt::format("Equipped {} {}!", BrainrotRegistry::get()->brainrotNames[id], utilities::toRoman(numFromString<int>(brainrotData.at("dupe")).unwrap())))->show();
                        }
                    }
                );
            }
        }
    );
}

BrainrotDisplay* BrainrotDisplay::create() {
    auto ret = new BrainrotDisplay();
    if (ret && ret->init()) {
        ret->autorelease();
        return ret;
    }
    delete ret;
    return nullptr;
}

CCScene* BrainrotDisplay::scene() {
    auto scene = CCScene::create();
    auto layer = BrainrotDisplay::create();
    layer->setTouchEnabled(true);
    scene->addChild(layer);
    return scene;
}