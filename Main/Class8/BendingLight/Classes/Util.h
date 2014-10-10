#ifndef __UTIL_H__
#define __UTIL_H__

#include "cocos2d.h"

static const std::string CHIME("audio/notification.mp3");
static const cocos2d::Color4B GREENISH(64,102,24,255);
void captureScreenAsSprite(std::function< void (cocos2d::Sprite *) > cb);
cocos2d::Vec2 getAbsolutePosition(cocos2d::Node *);

#endif // __UTIL_H__
