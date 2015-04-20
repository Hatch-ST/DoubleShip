#include "TouchManager.h"

/**
 *�R���X�g���N�^
 *@param touch_num �^�b�`�ł����
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
 *�f�X�g���N�^
 */
TouchManager::~TouchManager(){
	touchData->release();
}
 
/**
 *�X�V����
 */
void TouchManager::update(float delta){
	CCObject* it;
    CCARRAY_FOREACH (touchData,it){
		Touch* touch = dynamic_cast<Touch*>(it);
		touch->update(delta);
    }
}
 
/**
 *�^�b�`�J�n���ɌĂ�
 *@param index �Ǘ���̔ԍ�
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
 *�^�b�`�ړ����ɌĂ�
 *@param index �Ǘ���̔ԍ�
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
 *�^�b�`�L�����Z�����ɌĂ�.
 *�Ă΂��^�C�~���O���悭������Ȃ��̂�Ended�Ɠ����ɂ��Ƃ�.
 *@param index �Ǘ���̔ԍ�
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
 *�^�b�`�I�����ɌĂ�
 *@param index �Ǘ���̔ԍ�
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
 *�^�b�`�N���X�̎擾
 *@param index �Ǘ���̔ԍ�
 *@return Touch
 */
Touch* TouchManager::getTouchObject(int index){
    Touch* touch;
    touch = dynamic_cast<Touch*>( touchData->objectAtIndex(index) );
    return touch;
}
/**
 *�^�b�`�ʒu�̎擾
 *@param index �Ǘ���̔ԍ�
 *@return CCPoint postion
 */
CCPoint TouchManager::getTouchPoint(int index){
    Touch* touch;
    touch = dynamic_cast<Touch*>( touchData->objectAtIndex(index) );
	CCPoint pos = CCPoint(touch->touch_x, touch->touch_y);
    return pos;
}
/**
 *Debug�p �擾����Touch�ʒu�̕������Ԃ�
 *@return touch�ʒu�̑S�擾����������
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
