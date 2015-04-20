#ifndef __TITLE_SCENE_H__
#define __TITLE_SCENE_H__

#include "cocos2d.h"

class Title : public cocos2d::CCLayerColor
{
public:
	~Title();

    virtual bool init();  

    static cocos2d::CCScene* scene();
   
    //コールバック用関数の準備
    void menuCallback(CCObject* pSender);

    CREATE_FUNC(Title);

private :
	virtual void update( float delta );
	bool ccTouchBegan( cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent );

	cocos2d::CCTexture2D* mBackTex;
	cocos2d::CCSprite** mBackSprites;

	cocos2d::CCPoint mHalfBackSize;

	int mScreenX;
	static const float mScrollVelocity;
	void keyBackClicked();
};

#endif // __TITLE_SCENE_H__
