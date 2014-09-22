#include "ValueArrow.h"
#include "SpriteLayer.h"

#include "cocos2d.h"

USING_NS_CC;
USING_NS_CC_EXT;

ValueArrow * ValueArrow::create(const std::string &fileName , const Rect &capInset)
{
	ValueArrow *sprite = new ValueArrow();
    if (sprite && sprite->initWithFile(fileName , capInset))
    {
        sprite->autorelease();
        return sprite;
    }
    CC_SAFE_DELETE(sprite);
    return nullptr;
}

bool ValueArrow::initWithFile(const std::string &fileName , const Rect &capInset)
{
	//////////////////////////////
	// 1. super init first
	if ( !Scale9Sprite::initWithFile(capInset , fileName) )
	{
		return false;
	}

	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	// force value label
	_labelForce = LabelTTF::create("", "fonts/American Typewriter.ttf", 30);
	_labelForce->setHorizontalAlignment(TextHAlignment::LEFT);
	_labelForce->setColor(Color3B::BLACK);
	this->addChild(_labelForce, 1);

	// storing the default content size to preserve it
	_contWD = this->getContentSize().width;
	_contHT = this->getContentSize().height;

	// calling adjust size to initialize the arrows with zero value, to hide them
	adjustSize();

    return true;
}

void ValueArrow::adjustSize()
{
	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	if(_val == 0.0)
	{
		this->setVisible(false);
		// resetting the content size to initial value stored in member variables
		this->setContentSize(Size(_contWD , _contHT));
	}
	else
	{
		// sets invisible node to visible
		this->setVisible(true);

		auto val = fabs(_val);

		// label for value of forces
		std::stringstream sstr;
		sstr << (int)val << " N";
		_labelForce->setString(sstr.str());
		_labelForce->setPosition(Vec2(visibleSize.width/2 , visibleSize.height/2));

		// scaling the arrows
		auto cont_wd = _contWD;
		auto cont_ht = _contHT;
		auto scalePercent = (val/SpriteLayer::MAX_FORCE)*100;
		// original size + scaling factor because it need to cover the content area to start scaling
		// looks wierd otherwise.
		this->setContentSize(Size(cont_wd + scalePercent , cont_ht));

		// positioning the arrow left/right using rotation of node
		float direction = _val < 0.0 ? -1 : 1;
		if(direction == -1)
		{
			this->setRotation(-180);
//			_labelForce->setAnchorPoint(Vec2::ANCHOR_BOTTOM_RIGHT);
//			_labelForce->setPosition(Vec2(visibleSize.width/2 , visibleSize.height/2));
		}
		else
		{
			this->setRotation(0);
//			_labelForce->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
//			_labelForce->setPosition(Vec2(visibleSize.width/2 , visibleSize.height/2));
		}
	}

}
