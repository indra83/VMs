#include "SpriteLayer.h"
#include <sstream>
#include <cstdlib>
#include <algorithm>

USING_NS_CC;

class ValueArrow : public Sprite
{
    float _val;
    Point _basePos;
    MenuItemLabel * _label;

public :
    static ValueArrow * create(const std::string &filename)
    {
        ValueArrow *sprite = new ValueArrow();                                                                                                                                        
        if (sprite && sprite->initWithFile(filename))
        {
            sprite->autorelease();
            sprite->adjustSize();
            sprite->showValues();
            return sprite;
        }
        CC_SAFE_DELETE(sprite);
        return nullptr;
    }

    ValueArrow() : _val(0.0f), _label(nullptr) {}

    void setValue(float f)
    {
        _val = f; 
        adjustSize();
        if (_label)
            _label->setLabel(getLabel());
    }

    float getValue() {return _val;}

    void adjustSize()
    {
        auto val = _val;
        if (_val < 0)
            val = -_val;
        setAnchorPoint(Point(_val < 0.0 ? 1.0 : 0.0, 0.0)); 
        if (_label)
        {
            _label->setAnchorPoint(Point(0.5, 0.0));
            _label->setPosition(Point(getContentSize().width * getScaleX()/2 , 65.0));  
        }
        setFlippedX(_val < 0);

        // 150 == normal size
        //
        // Stretches content proportional to newLevel
        float scale = val/150;
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

    void showValues()
    {

		Size visibleSize = Director::getInstance()->getVisibleSize();
		Point origin = Director::getInstance()->getVisibleOrigin();

		_label = MenuItemLabel::create(getLabel());
		_label->setColor(Color3B(255,255,255));
		_label->setAnchorPoint(Point(0.5, 0.0));
		_label->setPosition(Point(getContentSize().width /** getScaleX()/2 */, 65.0));
		// add the mass label
		addChild(_label);
		_label->retain();

    }
};

class SpriteAlt : public Sprite
{
    std::vector< Node *> _altNodes;
    Point _phantomPos;
public :
    static SpriteAlt * create(const std::string &filename, const Point & pos) 
    {
        SpriteAlt *sprite = new SpriteAlt();                                                                                                                                        
        if (sprite && sprite->initWithFile(filename))
        {
            sprite->autorelease();
            sprite->Sprite::setPosition(pos);
            sprite->_phantomPos = pos;
            return sprite;
        }
        CC_SAFE_DELETE(sprite);
        return nullptr;
    }

    SpriteAlt() : _altNodes() {}

    void addAltNode( Node * node ) { _altNodes.push_back(node); } 

    void removeAltNode(Node * node) 
    { 
        auto f = std::find(_altNodes.begin(), _altNodes.end(), node); 
        if (f != _altNodes.end())
            _altNodes.erase(f); 
    }

    // the physics engine uses setPosition to update bodies.. override it
    void setPosition(const Point &point)
    {
        Point positionBefore = _phantomPos;
        if (fabs(positionBefore.x - point.x) > 0.0)  
        //if (positionBefore.x != point.x)  
        {
        //    CCLOG( "changing position from P( %f, %f) --> P( %f, %f)", positionBefore.x, positionBefore.y, point.x, point.y);  
            Point delta = point - positionBefore;
            for ( auto altNode : _altNodes )
                altNode->runAction(Place::create(Point((altNode->getPosition() - delta).x, altNode->getPosition().y)));
        }
        Sprite::setPosition(Point(getPosition().x, point.y));
        _phantomPos = point;
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
    Point origin = Director::getInstance()->getVisibleOrigin();

    _crate = SpriteAlt::create("crate.png", Point(visibleSize.width/2 + origin.x , visibleSize.height/2 + origin.y +15));

    auto body = PhysicsBody::create(); 
    // PhysicsMaterial - Density , Restitution , Friction
    _shape = PhysicsShapeBox::create(_crate->getContentSize(), PhysicsMaterial(1.0, 0.0, 0.5));
    body->addShape(_shape);
    body->setMass(100);
    _crate->setPhysicsBody(body);
    this->addChild(_crate, 1);

    auto node = Node::create();
    //float y = visibleSize.height + origin.y;
    node->setPhysicsBody(PhysicsBody::createEdgeBox(
                Size(1000*visibleSize.width, visibleSize.height)));

    node->setPosition(Point(visibleSize.width/2 + origin.x , 5*visibleSize.height/6 + 12 +origin.y));
    addChild(node);
    _crate->addAltNode(node);

    ///////////////////////////////////////////////////////////////////////////////////////

    // issue block to always set sum of forces to true
    _sumOfForces = ValueArrow::create("arrow.png");
	_sumOfForces->setScaleY(0.25);
	_sumOfForces->setPosition(Point(origin.x + visibleSize.width/2,
										origin.y + visibleSize.height/2 /*+ 5*_forceFriction->getContentSize().height / 4*/));
	_sumOfForces->retain();
	addChild(_sumOfForces, 0);

    // issue block to always set mass to true
    _massLabel = MenuItemLabel::create(getMassLabel());
	_massLabel->setColor(Color3B(0, 0, 0));
	_massLabel->setAnchorPoint(Point(0.0, 0.0));

	auto half = _crate->getContentSize()/2;
	_massLabel->setPosition(_crate->getPosition() - Point(half.width, half.height));
	_massLabel->retain();
	addChild(_massLabel, 1);

    // issue block to always set show friction values to true
    _forceFriction = ValueArrow::create("arrow-fr.png");
	_forceFriction->setAnchorPoint(Point(0.0f, 0.0f));
	_forceFriction->setScaleY(0.25);
	_forceFriction->setPosition(Point(origin.x + visibleSize.width/2,
										origin.y + visibleSize.height/2 + _forceFriction->getContentSize().height / 4));
	_forceFriction->retain();
	addChild(_forceFriction , 1);

	// issue block to always set show force values to true
	_forceExternal = ValueArrow::create("arrow-force.png"); // TODO: make it  blue
	_forceExternal->setAnchorPoint(Point(0.0f, 0.0f));
	_forceExternal->setScaleY(0.25);
	_forceExternal->setPosition(Point(origin.x + visibleSize.width/2,
										origin.y + visibleSize.height/2 + 3*_forceExternal->getContentSize().height / 4));
	_forceExternal->retain();
	addChild(_forceExternal, 2);

	// issue block to always set speed value to true
	_speedLabel = MenuItemLabel::create(getSpeedLabel());
	_speedLabel->setColor(Color3B(0, 0, 0));
	_speedLabel->setAnchorPoint(Point(1.0, 0.0));
	_speedLabel->setPosition(Point(visibleSize.width, visibleSize.height - 40));
	// add the mass label
	addChild(_speedLabel);
	_speedLabel->retain();


    this->addPersonOfForce(0.0);
    this->scheduleUpdate();
    return true;
}

int getIndexFromForce(float force)
{
    // 10 = 150/15;
    auto val = floor(force/10);
    if (val > 14) val = 14; 
    return val;
}

void SpriteLayer::addPersonOfForce(float force)
{

    Size visibleSize = Director::getInstance()->getVisibleSize();
    Point origin = Director::getInstance()->getVisibleOrigin();

    auto beforeCrate = Point(visibleSize.width/2 + origin.x - _crate->getContentSize().width/2, 
                    visibleSize.height/3 + 15 +origin.y);
    auto afterCrate = Point(visibleSize.width/2 + origin.x + _crate->getContentSize().width/2,
                    visibleSize.height/3 + 15 +origin.y);

    auto prevPosition = beforeCrate;
    if (_person) 
    {
        prevPosition = _person->getPosition() + Point(_person->getContentSize().width, 0.0); 
        _crate->removeAltNode(_person); 
        removeChild(_person);
    }

    bool reflect = false;
    std::stringstream sstr;
    if (force == 0)
        sstr << "pusher_straight_on.png";
    else
    {
        if (force > 0) 
            sstr << "pusher_" << getIndexFromForce(force) << ".png";
        else
        {
            sstr << "pusher_" << getIndexFromForce(-force) << ".png";
            reflect = true;
        }
    }

    _person = Sprite::create(sstr.str());
    _person->setAnchorPoint(Point(0.0, 0.0)); 
    prevPosition -= Point(_person->getContentSize().width, 0.0); 
    beforeCrate -= Point(_person->getContentSize().width, 0.0); 
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
        _crate->addAltNode(_person);
}

void SpriteLayer::setBackGroundLayer(Layer *layer)
{
    _crate->addAltNode(layer);
}

void SpriteLayer::changeForceValue(float value)
{
    _forceExternalValue = value;
    addPersonOfForce(value);

    if (_forceExternal)
        _forceExternal->setValue(value);
    if (_sumOfForces)
        _sumOfForces->setValue(value + _forceFriction->getValue());
}

void SpriteLayer::changeFrictionValue(float value)
{
    _shape->setFriction(value);
}

LabelTTF * SpriteLayer::getMassLabel()
{
    float mass = _crate->getPhysicsBody()->getMass();
    std::stringstream sstr;
    sstr << mass << " Kg";
    auto labelTTF = LabelTTF::create(sstr.str().c_str(), "fonts/Marker Felt.ttf", 30);
    labelTTF->setHorizontalAlignment(TextHAlignment::LEFT);
    return labelTTF;
}

// TODO : get it through the api
#define PTM_RATIO 100
LabelTTF * SpriteLayer::getSpeedLabel()
{
    float velocity = _crate->getPhysicsBody()->getVelocity().x;
    std::stringstream sstr;
    sstr << "Speed - " << (int)(velocity / PTM_RATIO) << " m/sec";
    auto labelTTF = LabelTTF::create(sstr.str().c_str(), "fonts/Marker Felt.ttf", 30);
    labelTTF->setHorizontalAlignment(TextHAlignment::LEFT);
    return labelTTF;
}

void SpriteLayer::update(float dt)
{
    if (_forceExternalValue != 0.0)
        _crate->getPhysicsBody()->applyImpulse(Vect(_forceExternalValue, 0.0));

    if (_speedLabel) 
        _speedLabel->setLabel(getSpeedLabel());
    float vel = _crate->getPhysicsBody()->getVelocity().x;

// TODO: show frictional force
// temp hack.. find out how to get this from the api
   
    if (_forceFriction)
    {
        float fric = 0.0;
        float max = (_shape->getFriction()) *
                    (_crate->getPhysicsBody()->getMass())*
                    (-_crate->getPhysicsBody()->getWorld()->getGravity().y / PTM_RATIO);
        if (fabs(vel) >= 0.2) 
            fric = vel > 0.0 ? -max : max; 
        else if (fabs(_forceExternal->getValue()) != 0.0)
        {
            if (fabs(_forceExternal->getValue()) > max) 
                fric = _forceExternal->getValue() > 0 ? -max : max; 
            else
                fric = -_forceExternal->getValue();
        }
        _forceFriction->setValue(fric); 
    }

    if (_sumOfForces)
    {
        float total = 0.0;
        if (_forceExternal)
            total += _forceExternal->getValue();
        if (_forceFriction)
            total += _forceFriction->getValue();
        _sumOfForces->setValue(total);
    }
    Node::update(dt);
}

SpriteLayer::~SpriteLayer()
{
    CC_SAFE_RELEASE(_massLabel);
    CC_SAFE_RELEASE(_speedLabel);
    CC_SAFE_RELEASE(_forceExternal);
    CC_SAFE_RELEASE(_forceFriction);
}
