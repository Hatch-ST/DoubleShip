#ifndef INCLUDED_TOUCHIMAGE_H
#define INCLUDED_TOUCHIMAGE_H

#include "cocos2d.h"

class TouchImage{
public :
	TouchImage();
	~TouchImage();

	void setParent( cocos2d::CCLayer* layer, int z );
	void update( const cocos2d::CCPoint& position, bool enabe, bool hit );

private :
	void switchImage( cocos2d::CCSprite* out, int id );
	cocos2d::CCSprite* mSprite;
	cocos2d::CCSprite* mHitSprite;

	int mAlpha;
	unsigned int mCount;
	int mSize;
	int mNum;
};

#endif