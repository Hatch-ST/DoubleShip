#ifndef __GAME_MAIN_SCENE_H_
#define __GAME_MAIN_SCENE_H_

#include "cocos2d.h"
#include "TouchManager.h"
#include "PhysicsPlayer.h"
#include "Box2d/Box2D.h"
#include "GLES-Render.h"

#include "PhysicsSpriteTest.h"

class ObjectLayer;
class StageLayer;
class Ripple;
class TouchImage;

class GameMain : public cocos2d::CCLayer{
public:
	~GameMain();

	virtual bool init();  
	static cocos2d::CCScene* scene( int id );
	void menuCloseCallback(CCObject* pSender);
    
	CREATE_FUNC(GameMain);

	void loadStage( int id );
	
	static const float mPtmRatio;

private:
	//シーンが始まった時に呼び出されるメソッド
	virtual void onEnter();

	virtual void update(float delta);

	virtual void draw(); 

	//マルチタッチ
	virtual void ccTouchesBegan(cocos2d::CCSet *pTouches, cocos2d::CCEvent *pEvent);
	virtual void ccTouchesMoved(cocos2d::CCSet *pTouches, cocos2d::CCEvent *pEvent);
	virtual void ccTouchesCancelled(cocos2d::CCSet *pTouches, cocos2d::CCEvent *pEvent);
	virtual void ccTouchesEnded(cocos2d::CCSet* touches, cocos2d::CCEvent* event);

	//android バックキー
	void keyBackClicked();

	//android メニューキー
	void keyMenuClicked();

	//タッチマネージャー
	TouchManager* mTouchManager;

	StageLayer* mStageLayer;
	ObjectLayer* mObjectLayer;

	PhysicsPlayer* mPlayer;
	Ripple* mRipplesA[ 5 ];
	Ripple* mRipplesB[ 5 ];

	b2World* mWorld;
	GLESDebugDraw* mDebugDraw;

	bool mOnReady;
	bool mOnSequence;

	unsigned int mTimeCount;
	unsigned int mDeadCount;

	static int mStageId;

	TouchImage* mTouchImages;
};

#endif // __GAME_MAIN_SCENE_H_
