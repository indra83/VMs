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

	// storing the default content size to preserve it
	_contWD = this->getContentSize().width;
	_contHT = this->getContentSize().height;

	// initialize arrows with zero value to hide them initially
	if(_val == 0.0)
		this->setVisible(false);

    return true;
}

void ValueArrow::adjustSize()
{
	if(_val == 0.0)
	{
		this->setVisible(false);
		// resetting the content size to initial value stored in member variables
		this->setContentSize(Size(_contWD , _contHT));
	}

	else
	{
		this->setVisible(true);

		auto val = fabs(_val);
		// scaling the arrows
		auto cont_wd = _contWD;
		auto cont_ht = _contHT;
		auto scalePercent = (val/SpriteLayer::MAX_FORCE)*100;
		// original size + scaling factor because it need to cover the content area to start scaling
		// looks wierd otherwise.
		this->setContentSize(Size(cont_wd + scalePercent , cont_ht));


//
//    std::stringstream sstr;
//    sstr << (int)val << " N";
//    _labelForce->setString(sstr.str());

	// positioning the arrow left/right
	float direction = _val < 0.0 ? -1 : 1;

		if(direction == -1)
		{
			this->setRotation(-180);
//			this->setPosition(Vec2(2*this->getPosition().x , this->getPosition().y));
		}
		else
			this->setRotation(0);

//
//    _shaft->setScaleX(scale);
//    _shaft->setAnchorPoint(anchor);
//    if (direction == -1)
//        _shaft->setPosition(Vec2(-_shaft->getContentSize().width, 0.0));
//    else
//        _shaft->setPosition(Vec2(0.0, 0.0));
//
//    if (_head)
//        removeChild(_head);
//    auto head = DrawNode::create();
//    _head = head;
//    _head->setAnchorPoint(anchor);
//    head->drawTriangle(Vec2(0.0, HEIGHT),
//                        Vec2(0.0, -HEIGHT),
//                        Vec2(direction * HEIGHT, 0.0),
//                        Color4F(_color));
//    head->setContentSize(Size(HEIGHT, 2*HEIGHT));
//    addChild(_head);
//
//    if (direction == -1)
//    {
//        _head->setPosition(Vec2(-_shaft->getContentSize().width*scale + head->getContentSize().width*scaleHead, HEIGHT/2));
//    }
//    else
//    {
//        _head->setPosition(_shaft->getPosition() + Vec2(_shaft->getContentSize().width*scale, HEIGHT/2));
//    }
//    _head->setScaleX(scaleHead);
//
//
//    if (direction == -1)
//    {
//        _labelName->setPosition(_head->getPosition() + Vec2( -2 * head->getContentSize().width * scaleHead, 0.0 ));
//    }
//    else
//    {
//        _labelName->setPosition(_head->getPosition() + Vec2(head->getContentSize().width * scaleHead, 0.0 ));
//    }
//    _labelName->setAnchorPoint(anchor);
//
//    if( _labelForce->getContentSize().width > _shaft->getContentSize().width * scale )
//    {
//        _labelForce->setPosition(_labelName->getPosition() + Vec2(direction * (_labelName->getContentSize().width + 10.0), 0.0 ) );
//        _labelForce->setAnchorPoint(anchor);
//    }
//    else
//    {
//        _labelForce->setPosition(Vec2(direction * _shaft->getContentSize().width * scale/2 , 0.0));
//        _labelForce->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
//    }
//
//    setContentSize(_head->getContentSize());

	}

}

void ValueArrow::defaultArrowSize()
{
	auto cont_wd = this->getContentSize().width;
	auto cont_ht = this->getContentSize().height;

	this->setContentSize(Size(cont_wd , cont_ht));
}
