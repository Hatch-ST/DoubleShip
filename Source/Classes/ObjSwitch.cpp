#include "ObjSwitch.h"
#include "Collision.h"
#include "ScreenConverter.h"
#include "StageLayer.h"
#include <string>

using namespace GameLib;
using namespace cocos2d;
using namespace std;

ObjSwitch::ObjSwitch( int x, int y, StaticObject::Color color ){
	mSprite = CCSprite::create( "image/object/switch.png" );
	mColor = color;
	
	if ( color == StaticObject::COLOR_BLUE ){
		switchImage( mSprite, 2 );
	}else if ( color == StaticObject::COLOR_RED ){
		switchImage( mSprite, 0 );
	}else if ( color == StaticObject::COLOR_GREEN ){
		switchImage( mSprite, 0 );
	}

	mPosition.set( x, y );
	mPosition.y = ScreenConverter::convertY( y );

	mSprite->setPosition( ccp( mPosition.x, mPosition.y ) );

	mRange = 32.0f;
}

ObjSwitch::~ObjSwitch(){
	//mSprite->release();
}

void ObjSwitch::setParent( CCLayer* layer ){
	layer->addChild( mSprite );
}

void ObjSwitch::update( TouchManager* tm, StageLayer* sl ){
	CCPoint dest = ccp( mPosition.x - sl->screenPosition().x, mPosition.y + sl->screenPosition().y );
	mSprite->setPosition( dest );
}

bool ObjSwitch::isIntersect( cocos2d::CCPoint position, float range ){
	Vector2 pos( position.x, position.y );

	float r = range + mRange;
	if ( ( pos - mPosition ).sqLength() < r * r ){
		switchImage( mSprite, ( ( mColor == StaticObject::COLOR_BLUE ) ? 3 : 1 ) );
		return true;
	}else{
		switchImage( mSprite, ( ( mColor == StaticObject::COLOR_BLUE ) ? 2 : 0 ) );
		return false;
	}
}

bool ObjSwitch::isIntersect( cocos2d::CCPoint position, cocos2d::CCSize size, float angle ){
	CCPoint pos( mPosition.x, mPosition.y );
	CCSize s( mRange * 2, mRange * 2 );
	if ( Collision::testIntersectSquare( pos, s.width, s.height, 0.0f, position, size.width, size.height, angle ) ){
		switchImage( mSprite, ( ( mColor == StaticObject::COLOR_BLUE ) ? 3 : 1 ) );
		return true;
	}else{
		switchImage( mSprite, ( ( mColor == StaticObject::COLOR_BLUE ) ? 2 : 0 ) );
		return false;
	}
}

StaticObject::Color ObjSwitch::color() const{
	return mColor;
}

void ObjSwitch::switchImage( CCSprite* out, int id ){
	int x = id % 2;
	int y = id / 2;
	out->setTextureRect( CCRect( x * 48, y * 48, 48, 48 ) );
}
