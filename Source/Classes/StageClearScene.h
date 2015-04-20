#ifndef __StageClear_SCENE_H__
#define __StageClear_SCENE_H__

#include "cocos2d.h"

class StageClear : public cocos2d::CCLayerColor
{
public:
    virtual bool init();  

    static cocos2d::CCScene* scene(int id,bool isCleared);
   
    //コールバック用関数の準備
    void menuCallback(CCObject* pSender);

    CREATE_FUNC(StageClear);
	static int mStageId;
	static bool mIsCleared;
private:
	void saveClearStage();
	int mNumStage;
};

#endif // __StageClear_SCENE_H__
