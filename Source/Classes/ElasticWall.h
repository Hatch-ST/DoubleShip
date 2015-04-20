#ifndef __ELASTICWALL_H__
#define __ELASTICWALL_H__

#include "StaticObject.h"
#include "Box2d/Box2D.h"

class ElasticWall : public StaticObject{
public :
	ElasticWall( b2World* world, cocos2d::CCPoint pos, float scale, float angle, Color color);
	void setParent( cocos2d::CCLayer* layer, cocos2d::CCLayer* underLayer );
	void update( TouchManager* touchManager, StageLayer* stageLayer );
	void setColor( Color color );
	void resetColor();

private :
	cocos2d::CCSprite* mSprite;
	cocos2d::CCSprite* mSideSprite[ 2 ];

	cocos2d::CCSize mSize;
	cocos2d::CCSize mSideSize;
	cocos2d::CCSize mCollisionSize;

	cocos2d::CCPoint mPosition;
	cocos2d::CCPoint mSidePos[ 2 ];

	cocos2d::CCPoint mDefaultPosition;
	cocos2d::CCPoint mDefaultSidePos[ 2 ];

	cocos2d::CCPoint mMoveDirection;

	b2Body* mBody;
	b2Body* mSideBodies[ 2 ];

	float mMaxSideDistance;
	float mMinSideDistance;
	float mSideDifference;

	float mAngle;
	float mReturnMoveValue;

	b2PolygonShape mPolygonShape;
	b2FixtureDef mFixtureDef;
	
	bool mMoving;

	b2Vec2 mVelocity;
	unsigned int mCount;

	Color mColor;
	cocos2d::CCTexture2D* mRedTexture;
	cocos2d::CCTexture2D* mBlueTexture;
	cocos2d::CCTexture2D* mGreenTexture;
};

#endif