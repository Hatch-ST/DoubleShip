#ifndef INCLUDED_PHYSICSSPRITETEST_H
#define INCLUDED_PHYSICSSPRITETEST_H

#include "cocos2d.h"
#include "Box2d/Box2D.h"
#include "GVector2.h"

class TouchManager;
class StageLayer;

class PhysicsSpriteTest{
public :
	PhysicsSpriteTest( cocos2d::CCLayer* layer, b2World* world );
	void update( TouchManager* touchManager, const StageLayer& stageLayer );

private :
	cocos2d::CCSprite* mSprite;
	b2Body* mBody;

	float mWidth, mHeight, mAngle;
	GameLib::Vector2 mPos;

	int mTouchIndices[ 2 ];

	static const float mPtmRatio;
};

#endif
