#include <Brainrot.hpp> // i do <> for anything in the include folder
#include <Utils.hpp>
#include <numbers>

using namespace geode::prelude;

bool Brainrot::init(const std::string& brainrotID) {
    if (!CCSprite::init()) return false;

    auto texture = CCTextureCache::get()->addImage("tung_sahur.png"_spr, true);
    this->setTexture(texture);
    this->setTextureRect(CCRectMake(
        0, 0,
        texture->getContentSize().width,
        texture->getContentSize().height
    ));

    this->setScale(1.3f);
    this->setAnchorPoint({0.0f, 1.0f});

    speed = 100.0f;

    float angle = utilities::random::random(0.0f, 1.0f) * 2 * std::numbers::pi;
    velocity = ccp(std::cosf(angle) * speed, std::sinf(angle) * speed);

    this->scheduleUpdate();
    return true;
}

void Brainrot::update(float dt) {
    auto winSize = CCDirector::get()->getWinSize();
    
    auto newPos = this->getPosition() + ccpMult(velocity, dt);

    if (newPos.x < 0 || newPos.x > winSize.width)
        velocity.x = -velocity.x;
    if (newPos.y < 0 || newPos.y > winSize.height)
        velocity.y = -velocity.y;
    
    this->setPosition(this->getPosition() + ccpMult(velocity, dt));
}

void Brainrot::allowWander(bool toggle) {
    this->canWander = toggle;
}

Brainrot* Brainrot::create(const std::string& brainrotID) {
    auto ret = new Brainrot();
    if (ret && ret->init(brainrotID)) {
        ret->autorelease();
        return ret;
    }
    delete ret;
    ret = nullptr;
    return nullptr;
}