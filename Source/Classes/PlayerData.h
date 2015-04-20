#ifndef __PLAYER_DATA_H__
#define __PLAYER_DATA_H__

#include "cocos2d.h"
#include "Box2d/Box2D.h"
#include "PhysicsSprite.h"

USING_NS_CC;

//プレイヤーデータクラス
class PlayerData {
public:
	enum PlayerType{
		PLAYER_GREEN,
		PLAYER_RED,
		PLAYER_BLUE,
	};
	enum SpriteTag{
		GREEN_ID=0,
		RED_ID=1,
		BLUE_ID=2,
	};
	PlayerData(CCLayer* layer,b2World &world,PlayerType type,float playerWidth,float playerHeight);
	PlayerData(CCLayer* layer,b2World &world,PlayerType type,float playerWidth,float playerHeight,CCPoint playerPos,float startAngle);
	~PlayerData();

	void setPos(CCPoint pos,float angle);
	void addPos(CCPoint pos);
	void rotation( float angle );

	//初期化
	virtual void init(CCLayer* layer,b2World &world,PlayerType type,float playerWidth,float playerHeight);
		
	void setTouchIndex(int index);
	void resetTouchIndex(int index);
	void resetTouchIndexAll();
	bool isTouched(float touch_x,float touch_y);
	bool isEmptyTouch();
	int getFirstTouchedIndex(){return mFirstTouchedIndex;};
	int getSecondTouchedIndex(){return mSecondTouchedIndex;};
	CCPoint getPos();
	float getAngle(){ return boxBody->GetAngle();};
	b2Body* boxBody;

	void setFirstBeginPos(float firstPos_x,float firstPos_y);
	void setBeforeVector(float secondPos_x,float secondPos_y);
	float getAngleFromBeginVector(float secondPos_x,float secondPos_y);
	cocos2d::CCSprite* mSprite;
	CCSize size() const;
private:
	PlayerType mPlayerType;

	//プレイヤーの大きさ
	CCSize* mPlayerTouchSize;
	CCSize mSize;

	int mFirstTouchedIndex;
	int mSecondTouchedIndex;

	CCPoint* mFirstBeginPos;
	CCPoint* mBeforeVector;
};

#endif // __PLAYER_H__