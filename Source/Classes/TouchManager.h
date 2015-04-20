#ifndef __TOUCH__MANAGER__H__
#define __TOUCH__MANAGER__H__
 
#include "cocos2d.h"
#include "Touch.h"
 
using namespace cocos2d;

//タッチ管理クラス
class TouchManager{
public:
	TouchManager(int touch_num);
	~TouchManager();

	//タッチエフェクトを表示するか
	bool mEffectVisible;
     
	CCArray* touchData;
     
	void update(float delta);
	void setTouchBegan(int index,int x,int y);
	void setTouchMoved(int index,int x,int y);
	void setTouchEnded(int index,int x,int y);
	void setTouchCancelled(int index,int x,int y);
	Touch* getTouchObject(int index);
	CCPoint getTouchPoint(int index);
	CCString* getAllTouchPointString();
	int getTouchNum(){ return mTouchNum; };
private:
	int mTouchNum;
};
 
#endif // __TOUCH__MANAGER__H__