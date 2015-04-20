#ifndef INCLUDED_RIPPLE_H
#define INCLUDED_RIPPLE_H

#include "cocos2d.h"

class Ripple{
public :
	Ripple( int firstTime, bool mini );
	~Ripple();

	void update( cocos2d::CCPoint position, cocos2d::CCPoint move );
	void setParent( cocos2d::CCLayer* layer, int z );

private :
	void switchImage( cocos2d::CCSprite* out, int id );

	cocos2d::CCTexture2D* mTexture;
	cocos2d::CCSprite* mSprite;
	int mCount;
	int mSize;
};

#endif