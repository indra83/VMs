#ifndef __UTIL_H__
#define __UTIL_H__

#include "cocos2d.h"

static const cocos2d::Color4B GREENISH(64,102,24,255);
void captureScreenAsSprite(std::function< void (cocos2d::Sprite *) > cb);

#endif // __UTIL_H__
