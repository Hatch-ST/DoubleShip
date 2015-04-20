#ifndef __STATICWALL_H__
#define __STATICWALL_H__

#include "StaticObject.h"
#include "Box2d/Box2D.h"

class StaticWall : public StaticObject{
public :
	StaticWall( b2World* world, cocos2d::CCPoint pos, float scale, float angle, Color color );
	void setParent( cocos2d::CCLayer* layer, cocos2d::CCLayer* underLayer );
	void update( TouchManager* touchManager, StageLayer* stageLayer );
	void setColor( Color color );
	void resetColor();
	
private :
	cocos2d::CCSprite* mSprite;
	cocos2d::CCSprite* mSideSprite[ 2 ];

	cocos2d::CCPoint mPosition;
	cocos2d::CCSize mSize;
	
	cocos2d::CCPoint mSideDistance;

	b2Body* mBody;
	float mAngle;

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