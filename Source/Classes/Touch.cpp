#include "Touch.h"
 
/**
 *コンストラクタ
 */
Touch::Touch(){
    touchFlag = false;
	touchMovedFlag = false;
    touchBeganFlag=false;
    touchEndedFlag=false;
	touch_x=0;
	touch_y=0;
	touch_past_x=0;
	touch_past_y=0;
	touch_moved_x = 0;
	touch_moved_y = 0;
}
 
/**
 *更新処理
 */
void Touch::update(float delta){
    if(touchFlag==true){
        touch_time += delta;
    }
    touchBeganFlag=false;
    touchEndedFlag=false;
	touchMovedFlag=false;
	touch_past_x = touch_x;
	touch_past_y = touch_y;
	touch_moved_x = 0;
	touch_moved_y = 0;
}
 
/**
 *タッチ開始時に呼ぶ
 *@param x
 *@param y
 */
void Touch::setTouchBegan(float x,float y){
    touch_x=x;
    touch_y=y;
    touch_began_x=x;
    touch_began_y=y;
	touch_past_x = touch_x;
	touch_past_y = touch_y;
    touchBeganFlag = true;
    touchEndedFlag = false;
    touchFlag = true;
    touch_time=0;
}
 
/**
 *タッチ移動時に呼ぶ
 *@param x
 *@param y
 */
void Touch::setTouchMoved(float x,float y){
    touch_x=x;
    touch_y=y;
    touchFlag = true;
	touchMovedFlag = true;
	touch_moved_x = touch_x - touch_past_x;
	touch_moved_y = touch_y - touch_past_y;
}
 
/**
 *タッチキャンセル時に呼ぶ.
 *呼ばれるタイミングがよく分からないのでEndedと同じにしとく.
 *@param x
 *@param y
 */
void Touch::setTouchCancelled(float x,float y){
    touch_x=x;
    touch_y=y;
    touch_ended_x=x;
    touch_ended_y=y;
    touchEndedFlag = true;
	touchMovedFlag = false;
    touchFlag = false;
}
 
/**
 *タッチ終了時に呼ぶ
 *@param x
 *@param y
 */
void Touch::setTouchEnded(float x,float y){
    touch_x=x;
    touch_y=y;
    touch_ended_x=x;
    touch_ended_y=y;
    touchEndedFlag = true;
	touchMovedFlag = false;
    touchFlag = false;
}