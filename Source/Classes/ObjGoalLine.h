#ifndef INCLUDED_OBJGOALLINE_H
#define INCLUDED_OBJGOALLINE_H

#include "cocos2d.h"

class TouchManager;
class StageLayer;

class ObjGoalLine{
public :
	ObjGoalLine( cocos2d::CCPoint pos, float scale, float angle );
	void setParent( cocos2d::CCLayer* layer );

	void update( StageLayer* stageLayer );
	bool isIntersect( StageLayer* sl, cocos2d::CCPoint position, float width, float height, float angle );

private :
	cocos2d::CCSprite* mSprite;
	
	cocos2d::CCPoint mPosition;
	cocos2d::CCSize mSize;
	float mAngle;

};

#endif