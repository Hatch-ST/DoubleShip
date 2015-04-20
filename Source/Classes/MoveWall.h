#ifndef __MOVEWALL_H__
#define __MOVEWALL_H__

#include "StaticObject.h"
#include "Box2d/Box2D.h"

class MoveWall :public StaticObject{
public :
	MoveWall( b2World* world, cocos2d::CCPoint pos, float scale, float angle, Color color, float upMax, float downMax);
	void setParent( cocos2d::CCLayer* layer, cocos2d::CCLayer* underLayer );
	void update( TouchManager* touchManager, StageLayer* stageLayer );
	void setColor( Color color );
	void resetColor();

private :
	cocos2d::CCSprite* mSprite;
	cocos2d::CCSprite* mArrowSprite;
	cocos2d::CCSprite* mSideSprite[ 2 ];

	cocos2d::CCPoint mDefaultPosition;
	cocos2d::CCPoint mPosition;
	cocos2d::CCPoint mMoveDirection;
	cocos2d::CCSize mSize;
	cocos2d::CCSize mCollisionSize;

	cocos2d::CCPoint mSideDistance;

	b2Body* mBody;
	float mUpMax;
	float mDownMax;
	float mAngle;
	float mReturnMoveValue;

	bool mMoving;
	
	b2Vec2 mVelocity;
	unsigned int mCount;

	Color mColor;
	b2PolygonShape mPolygonShape;
	b2FixtureDef mFixtureDef;

	cocos2d::CCTexture2D* mRedTexture;
	cocos2d::CCTexture2D* mBlueTexture;
	cocos2d::CCTexture2D* mGreenTexture;
};

#endif