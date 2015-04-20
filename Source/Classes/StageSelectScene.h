#ifndef __STAGESELECT_SCENE_H__
#define __STAGESELECT_SCENE_H__

#include "cocos2d.h"

class Ripple;

class StageSelect : public cocos2d::CCLayerColor{
public:
	~StageSelect();
	virtual bool init();  

	static cocos2d::CCScene* scene();
    
	void stageSelectCallback(CCObject* pSender);
	void menuCloseCallback(CCObject* pSender);
        
	CREATE_FUNC(StageSelect);

private :
	void update( float delta );
	bool ccTouchBegan( cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent );
	void ccTouchMoved( cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent );
	void ccTouchEnded( cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent );

	bool mIsTouch;
	cocos2d::CCPoint mTouchedPos;
	cocos2d::CCPoint mPrevTouchedPos;

	cocos2d::CCMenu** mButtons;
	cocos2d::CCSprite** mNumLeftSprites;
	cocos2d::CCSprite** mNumRightSprites;

	cocos2d::CCSprite* mPlayerA;
	cocos2d::CCSprite* mPlayerB;

	Ripple* mRipplesA[ 5 ];
	Ripple* mRipplesB[ 5 ];

	cocos2d::CCPoint mMove;

	int mNumStage;
	bool mOnSequence;
	unsigned int mTimeCount;

	static const int mButtonListLeft;
	static const int mButtonListUp;
	static const int mButtonIntervalX;
	static const int mButtonIntervalY;
	void keyBackClicked();

	void onEnter();

	cocos2d::CCUserDefault* mUserDefault;
	cocos2d::CCSprite** mCheckSprite;
};

#endif // __STAGESELECT_SCENE_H__
