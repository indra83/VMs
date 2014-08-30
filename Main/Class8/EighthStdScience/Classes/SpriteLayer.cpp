#include "SpriteLayer.h"
#include <sstream>
#include <cstdlib>
#include <algorithm>

USING_NS_CC;

static const int SPRITE_ZINDEX = 0;
static const int LABEL_ZINDEX = 1;
static const Color3B BLACK(0, 0, 0);
static const float EPSILON=0.2;
static const int NUM_IMAGES=15;

// per person;
const float SpriteLayer::MAX_FORCE=100;

// TODO: base it on the crate size.. assuming the crate is 1m wide
static const int PTM_RATIO = 150;

class ValueArrow : public Sprite
{
    float _val;
    MenuItemLabel * _label;
    bool _showValues;

public :
    static ValueArrow * create(const std::string &filename, bool showValues=true)
    {
        ValueArrow *sprite = new ValueArrow();
        if (sprite && sprite->initWithFile(filename))
        {
            sprite->autorelease();
            sprite->_showValues = showValues;
            sprite->adjustSize();
            return sprite;
        }
        CC_SAFE_DELETE(sprite);
        return nullptr;
    }

    ValueArrow() : _val(0.0), _label(nullptr) {}

    void setValue(float f)
    {
        if( _val != f )
        {
            _val = f;
            adjustSize();
            if (_label)
                _label->setLabel(getLabel());
        }
    }

    float getValue() {return _val;}

    void adjustSize()
    {
        auto val = _val;
        if (_val < 0)
            val = -_val;
        setAnchorPoint(_val < 0.0 ? Vec2::ANCHOR_BOTTOM_RIGHT : Vec2::ANCHOR_BOTTOM_LEFT);

        if( _showValues && !_label) 
        {
            _label = MenuItemLabel::create(getLabel());
            _label->setColor(Color3B(255,255,255));
            _label->setAnchorPoint(Vec2(0.5, 0.0));
            _label->setPosition(Vec2(getContentSize().width /** getScaleX()/2 */, 65.0));
            // add the mass label
            addChild(_label);
        }

        if (_label)
        {
            _label->setAnchorPoint(Vec2(0.5, 0.0));
            _label->setPosition(Vec2(getContentSize().width * getScaleX()/2 , 65.0));
        }
        setFlippedX(_val < 0);

        // Stretches content proportional to newLevel
        float scale = val/SpriteLayer::MAX_FORCE;
        setScaleX(scale);
    }

    LabelTTF * getLabel()
    {
        std::stringstream sstr;
        float val = _val;
        if (val < 0)
            val = -val;
        sstr << (int)val << " N";
        auto labelTTF = LabelTTF::create(sstr.str().c_str(), "fonts/Maven Pro Black.otf", 30);
        labelTTF->setHorizontalAlignment(TextHAlignment::LEFT);
        return labelTTF;
    }

    void showValues(bool enable)
    {
        _showValues = enable;
    }
};

// on "init" you need to initialize your instance
bool SpriteLayer::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }

    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    //////////////////////////////
    // 2. add crate
    _crate = Layer::create();
    _crate->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
    _crate->setPosition(Vec2(visibleSize.width/2 + origin.x, visibleSize.height/3 + origin.y + 15));

    auto crate = Sprite::create("crate.png");
    crate->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
    crate->setPosition(Vec2::ZERO);
    _crate->setContentSize(crate->getContentSize());
    _crate->addChild(crate, SPRITE_ZINDEX);

    //////////////////////////////
    // 3. add mass label
    _massLabel = MenuItemLabel::create(getMassLabel());
    _massLabel->setColor(BLACK);
    _massLabel->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
    _massLabel->setPosition(Vec2(-crate->getContentSize().width/2, 0.0));
    _crate->addChild(_massLabel, LABEL_ZINDEX);

    this->addChild(_crate, SPRITE_ZINDEX);

    //////////////////////////////
    // 3. add the force arrows
    Vec2 curr(origin.x + visibleSize.width/2, origin.y + visibleSize.height/2);
    auto addArrow = [&](const std::string & fileName) -> ValueArrow *
    {
        auto arrow = ValueArrow::create("arrow.png");
        arrow->setScaleY(0.25);
        arrow->setPosition(curr); 
        curr = curr + Vec2(0, arrow->getContentSize().height / 4);
        this->addChild(arrow, LABEL_ZINDEX);
        return arrow;
    };

    _sumOfForces = addArrow("arrow.png");
    _forceFriction = addArrow("arrow-fr.png");
    _forceExternal = addArrow("arrow-force.png");

    //////////////////////////////
    // 3. add the speedLabel
    _speedLabel = MenuItemLabel::create(getSpeedLabel());
    _speedLabel->setColor(BLACK);
    _speedLabel->setAnchorPoint(Vec2::ANCHOR_BOTTOM_RIGHT);
    _speedLabel->setPosition(Vec2(visibleSize.width, visibleSize.height/3));
    addChild(_speedLabel);

    this->addPersonOfForce(0.0);
    this->scheduleUpdate();
    return true;
}

int getIndexFromForce(float force)
{
    static const int DELTA = SpriteLayer::MAX_FORCE/NUM_IMAGES;
    auto val = floor(force/DELTA);
    if (val > 14) val = 14; // for the exactly MAX_FORCE case
    return val;
}

void SpriteLayer::addPersonOfForce(float force)
{
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    auto beforeCrate = Vec2(visibleSize.width/2 + origin.x - _crate->getContentSize().width/2,
                    visibleSize.height/3 + 15 +origin.y);
    auto afterCrate = Vec2(visibleSize.width/2 + origin.x + _crate->getContentSize().width/2,
                    visibleSize.height/3 + 15 +origin.y);

    auto prevPosition = beforeCrate;
    if (_person)
    {
        prevPosition = _person->getPosition() + Vec2(_person->getContentSize().width, 0.0); 
        _personPushing = true;
        removeChild(_person);
    }

    bool reflect = false;
    std::stringstream sstr;
    if (force == 0)
        sstr << "pusher_straight_on.png";
    else
    {
        sstr << "pusher_" << getIndexFromForce(fabs(force)) << ".png";
        if (force < 0)
            reflect = true;
    }

    _person = Sprite::create(sstr.str());
    _person->setAnchorPoint(Vec2(0.0, 0.0));
    prevPosition -= Vec2(_person->getContentSize().width, 0.0);
    beforeCrate -= Vec2(_person->getContentSize().width, 0.0);
    if (force > 0)
        _person->setPosition(beforeCrate);
    else if (force < 0)
        _person->setPosition(afterCrate);
    else
        _person->setPosition(prevPosition);

    if (reflect)
        _person->setFlippedX(true);
    addChild(_person);
    if (force == 0)
        _personPushing = false;
}

float SpriteLayer::getFrictionalForce()
{
    float fric = 0.0;
    float gravity = 10.0;
    float max = _frictionCoefficient * _mass * gravity;
    if (fabs(_velocity) > 0.2)
        fric = _velocity > 0.0 ? -max : max;
    else if (fabs(_forceExternalValue) != 0.0)
    {
        if (fabs(_forceExternalValue) > max) 
            fric = _forceExternalValue > 0 ? -max : max;
        else
            fric = -_forceExternalValue;
    }
    return fric;
}

void SpriteLayer::readjustForces()
{
    _forceFrictionValue = getFrictionalForce();
    _forceFriction->setValue(_forceFrictionValue);
    _sumOfForcesValue = _forceExternalValue + _forceFrictionValue;
    _sumOfForces->setValue(_sumOfForcesValue);
}

void SpriteLayer::changeForceValue(float value)
{
    addPersonOfForce(value);

    _forceExternalValue = value;
    _forceExternal->setValue(_forceExternalValue);

    readjustForces();
}

void SpriteLayer::changeFrictionValue(float value)
{
    _frictionCoefficient = value;
}

LabelTTF * SpriteLayer::getMassLabel()
{
    std::stringstream sstr;
    sstr << _mass << " Kg";
    auto labelTTF = LabelTTF::create(sstr.str().c_str(), "fonts/Marker Felt.ttf", 30);
    labelTTF->setHorizontalAlignment(TextHAlignment::LEFT);
    return labelTTF;
}

LabelTTF * SpriteLayer::getSpeedLabel()
{
    std::stringstream sstr;
    sstr << "Speed - " << (int)_velocity << " m/sec";
    auto labelTTF = LabelTTF::create(sstr.str().c_str(), "fonts/Marker Felt.ttf", 30);
    labelTTF->setHorizontalAlignment(TextHAlignment::LEFT);
    return labelTTF;
}

void SpriteLayer::setMass(float mass)
{
    _mass = mass;
    _massLabel->setLabel(getMassLabel());
}

void SpriteLayer::update(float dt)
{
    readjustForces();

    float acc = _prevSumOfForcesValue / _mass;
    float dv = acc * dt;
    _velocity += dv;
    float dx = 0.0;
    if(fabs(_velocity) > EPSILON)
        dx = _velocity * dt * PTM_RATIO;
    _speedLabel->setLabel(getSpeedLabel());

    if(fabs(dx) > 0.0)
    {
        _moveCB(dx);
        if(!_personPushing)
            _person->runAction(Place::create(_person->getPosition() + Vec2(-dx, 0.0)));
    }

    _prevSumOfForcesValue = _sumOfForcesValue;
    Node::update(dt);
}

void SpriteLayer::addAnotherPerson()
{
}

SpriteLayer::~SpriteLayer()
{
    unscheduleUpdate();
}
