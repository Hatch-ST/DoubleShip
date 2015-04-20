#ifndef __TOUCH__H__
#define __TOUCH__H__

#include "cocos2d.h" 

//タッチクラス
class Touch:public cocos2d::CCObject{
public:
    Touch();
     
    //タッチしているか
    bool touchFlag;
    //タッチが開始されたか
    bool touchBeganFlag;
	//タッチが移動中か
	bool touchMovedFlag;
    //タッチが終了されたか
    bool touchEndedFlag;
     
    //タッチ座標
    float touch_x;
    float touch_y;
    //移動前のタッチ座標
    float touch_past_x;
    float touch_past_y;
    //開始位置
    float touch_began_x;
    float touch_began_y;
	//移動した距離
	float touch_moved_x;
	float touch_moved_y;
    //終了位置
    float touch_ended_x;
    float touch_ended_y;
    //タッチしている時間
    float touch_time;
     
    void update(float delta);
    void setTouchBegan(float x,float y);
    void setTouchMoved(float x,float y);
    void setTouchEnded(float x,float y);
    void setTouchCancelled(float x,float y);
     
};
 
#endif // __TOUCH__H__