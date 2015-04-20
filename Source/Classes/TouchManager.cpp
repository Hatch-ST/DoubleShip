#include "TouchManager.h"

/**
 *コンストラクタ
 *@param touch_num タッチできる個数
 */
TouchManager::TouchManager(int touch_num){
	if(touch_num<1){
		touch_num=1;
	}else if(touch_num>5){
		touch_num=5;
	}
	mTouchNum = touch_num;
	touchData = CCArray::create();
	touchData->retain();
	for(int i=0;i<touch_num;i++){
		Touch* touch = new Touch();
		touchData->addObject(touch);
	}
	mEffectVisible = false;
}

/**
 *デストラクタ
 */
TouchManager::~TouchManager(){
	touchData->release();
}
 
/**
 *更新処理
 */
void TouchManager::update(float delta){
	CCObject* it;
    CCARRAY_FOREACH (touchData,it){
		Touch* touch = dynamic_cast<Touch*>(it);
		touch->update(delta);
    }
}
 
/**
 *タッチ開始時に呼ぶ
 *@param index 管理上の番号
 *@param x
 *@param y
 */
void TouchManager::setTouchBegan(int index,int x,int y){
	if( index >= touchData->count() ) return;
    Touch* touch;
    touch = dynamic_cast<Touch*>( touchData->objectAtIndex(index) );
    touch->setTouchBegan(x, y);
}
 
/**
 *タッチ移動時に呼ぶ
 *@param index 管理上の番号
 *@param x
 *@param y
 */
void TouchManager::setTouchMoved(int index,int x,int y){
	if( index >= touchData->count() ) return;
    Touch* touch;
    touch = dynamic_cast<Touch*>( touchData->objectAtIndex(index) );
    touch->setTouchMoved(x, y);
}
 
/**
 *タッチキャンセル時に呼ぶ.
 *呼ばれるタイミングがよく分からないのでEndedと同じにしとく.
 *@param index 管理上の番号
 *@param x
 *@param y
 */
void TouchManager::setTouchCancelled(int index,int x,int y){
	if( index >= touchData->count() ) return;
    Touch* touch;
    touch = dynamic_cast<Touch*>( touchData->objectAtIndex(index) );
    touch->setTouchCancelled(x, y);
}
 
/**
 *タッチ終了時に呼ぶ
 *@param index 管理上の番号
 *@param x
 *@param y
 */
void TouchManager::setTouchEnded(int index,int x,int y){
	if( index >= touchData->count() ) return;
    Touch* touch;
    touch = dynamic_cast<Touch*>( touchData->objectAtIndex(index) );
    touch->setTouchEnded(x, y);
}
/**
 *タッチクラスの取得
 *@param index 管理上の番号
 *@return Touch
 */
Touch* TouchManager::getTouchObject(int index){
    Touch* touch;
    touch = dynamic_cast<Touch*>( touchData->objectAtIndex(index) );
    return touch;
}
/**
 *タッチ位置の取得
 *@param index 管理上の番号
 *@return CCPoint postion
 */
CCPoint TouchManager::getTouchPoint(int index){
    Touch* touch;
    touch = dynamic_cast<Touch*>( touchData->objectAtIndex(index) );
	CCPoint pos = CCPoint(touch->touch_x, touch->touch_y);
    return pos;
}
/**
 *Debug用 取得したTouch位置の文字列を返す
 *@return touch位置の全取得した文字列
 */
CCString* TouchManager::getAllTouchPointString(){
    std::string str;
    str.clear();
     
	for(int i=0;i<touchData->count();i++){
        char cstr[256];
        Touch* touch;
        touch = dynamic_cast<Touch*>( touchData->objectAtIndex(i) );
         
        sprintf(cstr, "%d:(%d,%d)",i,touch->touch_x,touch->touch_y);
        str.append(cstr);
        str.append("\n");
    }
	CCString* ccstr;
	ccstr = CCString::createWithFormat("%s",str.c_str());
     
    return ccstr;
}
