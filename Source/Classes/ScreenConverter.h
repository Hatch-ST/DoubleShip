#ifndef INCLUDED_SCREENCONVERTER_H
#define INCLUDED_SCREENCONVERTER_H

#include "cocos2d.h"
#include "GVector2.h"

class ScreenConverter{
public :
	static void toLeftUp( int* y, int height = -1 ){
		if ( height < 0 ) height = cocos2d::CCDirector::sharedDirector()->getVisibleSize().height;
		*y = height - *y;
	}

	static void toLeftUp( float* y, int height = -1 ){
		if ( height < 0 ) height = cocos2d::CCDirector::sharedDirector()->getVisibleSize().height;
		*y = height - *y;
	}

	static void toLeftDown( float* y, int height = -1 ){
		if ( height < 0 ) height = cocos2d::CCDirector::sharedDirector()->getVisibleSize().height;
		*y = height - *y;
	}

	static float convertY( float y ){
		return cocos2d::CCDirector::sharedDirector()->getVisibleSize().height - y;
	}

	static void putSprite( cocos2d::CCSprite* sprite, float x, float y ){
		toLeftUp( &y );
		sprite->setPosition( ccp( x, y ) );
	}

	static void putSprite( cocos2d::CCSprite* sprite, const GameLib::Vector2& position ){
		int y = position.y;
		toLeftUp( &y );
		sprite->setPosition( ccp( position.x, y ) );
	}

	static void putSprite( cocos2d::CCSprite* sprite, const cocos2d::CCPoint& position ){
		int y = position.y;
		toLeftUp( &y );
		sprite->setPosition( ccp( position.x, y ) );
	}

};

#endif