#ifndef __PLAYER_H__
#define __PLAYER_H__

#include "cocos2d.h"
#include "TouchManager.h"
#include "PlayerData.h"
#include "StageLayer.h"
#include "ObjectLayer.h"
#include "Box2d/Box2D.h"

USING_NS_CC;

//プレイヤー管理クラス
class PhysicsPlayer {
public:
	PhysicsPlayer(CCLayer* layer,b2World &world);
	void setStartPos(CCPoint startPositionRed,float startAngleRed, CCPoint startPositionBlue,float startAngleBlue);
	~PhysicsPlayer();
	//初期化
	virtual void init(CCLayer* layer,b2World &world);
		
	//更新処理
	virtual void update(float delta,TouchManager* touchManager,const StageLayer& stageLayer,const ObjectLayer& objectLayer);
	bool isIntersect( const cocos2d::CCPoint& position );
	bool isTouched( int touchIndex );
	
	bool ready( TouchManager* );

	CCPoint getPosition( int index ) const;
	CCSize getSize( int index ) const;
	float getAngle( int index ) const;

	bool splited() const;

	static const float GreenWidth;
	static const float GreenHeight;
	static const float RedWidth;
	static const float RedHeight;
	static const float BlueWidth;
	static const float BlueHeight;

	static const float SplitDistance;
	static const float UnitDistance;

	//合体が有効か
	bool mCanUniteFlag;

	bool isDead() const;

	bool isCleared() const;
	void setCleared( int index, bool value );

private:
	//プレイヤー
	PlayerData* mGreenPlayer;
	PlayerData* mRedPlayer;
	PlayerData* mBluePlayer;

	bool mPlayerSplited;

	bool mRedDied;
	bool mBlueDied;
	bool mRedClear;
	bool mBlueClear;
	int mDeadCountRed;
	int mDeadCountBlue;
	int mDeadTime;

	float getMax(float a,float b);
	CCLabelTTF* mDdebugLabel;
	CCLabelTTF* mTouchLabel;
	int debugCount;
};

#endif // __PLAYER_H__