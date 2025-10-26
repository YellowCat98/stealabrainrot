#include <BrainrotDisplay.hpp>
#include <StealaBrainrot.hpp>

using namespace geode::prelude;

/*
bool BrainrotDisplay::init() {
    if (!CCLayer::init()) return false;

    this->setColor({129, 133, 137});
    this->setOpacity(255);
    auto winSize = CCDirector::get()->getWinSize();
    auto carpet = CCLayer::create({255, 105, 97, 255}, winSize.width / 2.5f, winSize.height);
    carpet->setID("carpet");
    carpet->ignoreAnchorPointForPosition(false);
    carpet->setPosition({winSize.width / 2, winSize.height / 2});
    carpet->setZOrder(1);

    auto behindCarpet = CCLayer::create({ 180, 180, 180, 255 }, carpet->getContentWidth() * 1.2f, carpet->getContentHeight());
    behindCarpet->setID("behind-carpet");
    behindCarpet->ignoreAnchorPointForPosition(false);
    behindCarpet->setPosition(carpet->getPosition());

    auto menu = CCMenu::create();
    menu->setID("brainrots-left");

    SaveManager::get()->getCollectedBrainrots();
    auto brainrots = SaveManager::get()->getAllCollectedBrainrots();

    for (const auto [k, v] : brainrots) {
        auto button = CCMenuItemExt::createSpriteExtraWithFilename(BrainrotRegistry::get()->brainrots[v.at("id")], 0.5f, [this](CCMenuItemSpriteExtra* btn) {
            return this->brainrotTouchCallback(btn);
        });
        button->setID(fmt::format("{}-{}", k, v.at("id")));
        menu->addChild(button);
    }
    menu->setLayout(geode::ColumnLayout::create()
        ->setAxisAlignment(AxisAlignment::Even)
        ->setGap(0.25f)
        //->setAutoScale(false)
    );

    menu->setPosition({carpet->getPositionX() - carpet->getContentWidth() / 2 - 50.0f, winSize.height / 2});

    this->addChild(menu);
    this->addChild(carpet);
    this->addChild(behindCarpet);
    return true;
}
*/

// i put a lot of care into low chances because this will ONLY be used for distinguishible duplicates
std::string toRoman(int num) {
    std::unordered_map<int, std::string> romans = {
        {1000, "M"}, {900, "CM"}, {500, "D"}, {400, "CD"},
        {100, "C"}, {90, "XC"}, {50, "L"}, {40, "XL"},
        {10, "X"}, {9, "IX"}, {5, "V"}, {4, "IV"}, {1, "I"}
    };

    std::string res;
    for (const auto [value, symbol] : romans) {
        while (num >= value) {
            res += symbol;
            num -= value;
        }
    }

    return res;
}

bool BrainrotDisplay::init() {
    if (!CCLayer::init()) return false;
    

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
                toRoman(numFromString<int>(v.at("dupe")).unwrap())
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

    this->brainrots = brainrots;
    this->addChild(brainrots);
    this->addChild(bg);
    this->addChildAtPosition(backMenu, Anchor::TopLeft, {12.0f, -25.0f}, false);
    return true;
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
    auto previousID = SaveManager::get()->getCollectedBrainrot(previousToken).at("id");
    
    auto previousBrainrot = static_cast<Brainrot*>(brainrots->getChildByID(fmt::format("{}-{}", previousID, previousToken)));

    previousBrainrot->setLabelColor({255, 255, 255});


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
            fmt::format("{} {}", BrainrotRegistry::get()->brainrotNames[brainrotData.at("id")], toRoman(numFromString<int>(brainrotData.at("dupe")).unwrap())),
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
                        BrainrotRegistry::get()->brainrotNames[brainrotData.at("id")], toRoman(numFromString<int>(brainrotData.at("dupe")).unwrap())
                    ),
                    utilities::random::choice<std::string>({"No!!", "i missclicked", "surely not", "i do not agree", "no, smh.."}).c_str(),
                    utilities::random::choice<std::string>({"OK", "Okay", "Duh??", "absolutely yes", "why would i click the button duh??", "Alright!", "k", "alr", fmt::format("{} says absolutely!", GJAccountManager::get()->m_username)}).c_str(),
                    [=](FLAlertLayer*, bool btn2) {
                        if (btn2) {
                            brainrot->setLabelColor({ 0, 255, 13 });
                            Mod::get()->setSavedValue<std::string>("equipped-brainrot", token);
                            Notification::create(fmt::format("Equipped {}!", BrainrotRegistry::get()->brainrotNames[id]))->show();
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