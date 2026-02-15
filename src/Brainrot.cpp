#include <Brainrot.hpp> // i do <> for anything in the include folder
#include <Utils.hpp>
#include <numbers>
#include <BrainrotRegistry.hpp>

using namespace geode::prelude;

bool Brainrot::init(const std::string& brainrotID, const std::string& token, Brainrot::Age age, const std::string& nameAboveHead) {
    if (!CCSprite::init()) return false;
    this->brainrotID = brainrotID;
    this->brainrotToken = token;
    auto texture = CCTextureCache::get()->addImage(BrainrotRegistry::get()->brainrots[brainrotID].c_str(), true);
    this->setTexture(texture);
    this->setTextureRect(CCRectMake(
        0, 0,
        texture->getContentSize().width,
        texture->getContentSize().height
    ));

    float scale = 0.0f;
    switch (age) {
        case Brainrot::Age::Baby: scale = 0.55f; break;
        case Brainrot::Age::Child: scale = 0.7f; break;
        case Brainrot::Age::TikTokUser: scale = 0.85f; break;
        case Brainrot::Age::Teenager: scale = 0.93f; break;
        case Brainrot::Age::Adult: scale = 1.0f; break;
    }
    this->setScale(scale);

    this->setAnchorPoint({0.0f, 1.0f});

    speed = 100.0f;

    float angle = utilities::_random::random(0.0f, 1.0f) * 2 * std::numbers::pi;
    velocity = ccp(std::cosf(angle) * speed, std::sinf(angle) * speed);

    if (!nameAboveHead.empty()) {
        auto label = CCLabelBMFont::create(nameAboveHead.c_str(), "bigFont.fnt");
        label->setAnchorPoint({{0.5f, 0.0f}});
        label->setPosition({
            this->getContentWidth() / 2,
            this->getContentHeight()
        });
        this->label = label;
        this->addChild(label, 1);
    }

    this->scheduleUpdate();
    return true;
}

void Brainrot::setLabelColor(const ccColor3B& col) {
    this->label->setColor(col);
}

void Brainrot::update(float dt) {
    auto winSize = CCDirector::get()->getWinSize();
    

    float maxAngleChange = 15.0f * (std::numbers::pi / 180.0f);
    float angleChange = utilities::random::random(-maxAngleChange, maxAngleChange);

    float currentAngle = std::atan2f(velocity.y, velocity.x);
    currentAngle += angleChange;

    velocity = ccp(std::cosf(currentAngle) * speed, std::sinf(currentAngle) * speed);

    auto newPos = this->getPosition() + ccpMult(velocity, dt);

    if (newPos.x < 0 || newPos.x > winSize.width)
        velocity.x = -velocity.x;
    if (newPos.y < 0 || newPos.y > winSize.height)
        velocity.y = -velocity.y;
    
    this->setPosition(this->getPosition() + ccpMult(velocity, dt));
}

Brainrot* Brainrot::create(const std::string& brainrotID, const std::string& token, Brainrot::Age age, const std::string& nameAboveHead) {
    auto ret = new Brainrot();
    if (ret && ret->init(brainrotID, token, age, nameAboveHead)) {
        ret->autorelease();
        return ret;
    }
    delete ret;
    ret = nullptr;
    return nullptr;
}