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
            utilities::stringToAge(v.at("age")), 
            fmt::format("{} {}", BrainrotRegistry::get()->brainrotNames[v.at("id")],
                toRoman(numFromString<int>(v.at("dupe")).unwrap())
            )
        );
        if (k == Mod::get()->getSavedValue<std::string>("equipped-brainrot")) brainrot->setLabelColor({ 0, 255, 13 });

        brainrot->setID(fmt::format("{}-{}", v.at("id"), k));

        brainrots->addChild(brainrot);
    }
    this->brainrots = brainrots;
    this->addChild(brainrots);
    this->addChild(bg);
    
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
    log::info("Touched {}! he {}.", brainrot->getID(), utilities::random::choice<std::string>({"was offended", "wants to kill you", "is yelling bad words at you"}));
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