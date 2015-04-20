#ifndef INDLUED_STATIC_OBJECT_H
#define INDLUED_STATIC_OBJECT_H

#include "cocos2d.h"
#include "GVector2.h"
#include "TouchManager.h"
#include "StageLayer.h"
#include "FixtureGroup.h"

class StaticObject{
public :
	enum Color{
		COLOR_RED,
		COLOR_BLUE,
		COLOR_GREEN,
		COLOR_BLACK_S,
		COLOR_BLACK_M,
		COLOR_BLACK_L,
	};

	virtual ~StaticObject(){}
	virtual void setParent( cocos2d::CCLayer* layer, cocos2d::CCLayer* underLayer ) = 0;
	virtual void update( TouchManager* touchManager, StageLayer* stageLayer ) = 0;
	virtual bool isIntersect( cocos2d::CCPoint position, const StageLayer& stageLayer ) const{ return false; };
	virtual void setColor( Color ){};
	virtual void resetColor(){};
};

#endif