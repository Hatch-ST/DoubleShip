#ifndef INCLUDED_OBJSWITCH_H
#define INCLUDED_OBJSWITCH_H

#include "cocos2d.h"
#include "GVector2.h"
#include "StaticObject.h"

class TouchManager;
class StageLayer;

class ObjSwitch{
public :
	ObjSwitch( int x, int y, StaticObject::Color color );
	~ObjSwitch();
	void setParent( cocos2d::CCLayer* layer );

	void update( TouchManager* touchManager, StageLayer* stageLayer );
	bool isIntersect( cocos2d::CCPoint position, float range );
	bool isIntersect( cocos2d::CCPoint position, cocos2d::CCSize size, float angle );
	StaticObject::Color color() const;

private :
	void switchImage( CCSprite* out, int id );

	StaticObject::Color mColor;
	cocos2d::CCSprite* mSprite;
	GameLib::Vector2 mPosition;

	float mRange;
};

#endif