#include <BrainrotDisplay.hpp>

using namespace geode::prelude;

bool BrainrotDisplay::init() {
    if (!CCLayerColor::init()) return false;
    auto shownBrainrots = SceneManager::get()->getPersistedNodes();
    for (auto& node : shownBrainrots) {
        node->setVisible(false); // Should you be in the brainrot display, thou brainrot will not be visible.
    }
    this->setColor({129, 133, 137});
    this->setOpacity(255);
    auto winSize = CCDirector::get()->getWinSize();
    auto carpet = CCLayerColor::create({255, 105, 97, 255}, winSize.width / 2.5f, winSize.height);
    carpet->setID("carpet");
    carpet->ignoreAnchorPointForPosition(false);
    carpet->setPosition({winSize.width / 2, winSize.height / 2});
    carpet->setZOrder(1);

    auto behindCarpet = CCLayerColor::create({ 180, 180, 180, 255 }, carpet->getContentWidth() * 1.2f, carpet->getContentHeight());
    behindCarpet->setID("behind-carpet");
    behindCarpet->ignoreAnchorPointForPosition(false);
    behindCarpet->setPosition(carpet->getPosition());


    this->addChild(carpet);
    this->addChild(behindCarpet);
    return true;
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
    scene->addChild(BrainrotDisplay::create());
    return scene;
}