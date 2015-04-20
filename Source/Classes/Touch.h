#ifndef __TOUCH__H__
#define __TOUCH__H__

#include "cocos2d.h" 

//�^�b�`�N���X
class Touch:public cocos2d::CCObject{
public:
    Touch();
     
    //�^�b�`���Ă��邩
    bool touchFlag;
    //�^�b�`���J�n���ꂽ��
    bool touchBeganFlag;
	//�^�b�`���ړ�����
	bool touchMovedFlag;
    //�^�b�`���I�����ꂽ��
    bool touchEndedFlag;
     
    //�^�b�`���W
    float touch_x;
    float touch_y;
    //�ړ��O�̃^�b�`���W
    float touch_past_x;
    float touch_past_y;
    //�J�n�ʒu
    float touch_began_x;
    float touch_began_y;
	//�ړ���������
	float touch_moved_x;
	float touch_moved_y;
    //�I���ʒu
    float touch_ended_x;
    float touch_ended_y;
    //�^�b�`���Ă��鎞��
    float touch_time;
     
    void update(float delta);
    void setTouchBegan(float x,float y);
    void setTouchMoved(float x,float y);
    void setTouchEnded(float x,float y);
    void setTouchCancelled(float x,float y);
     
};
 
#endif // __TOUCH__H__