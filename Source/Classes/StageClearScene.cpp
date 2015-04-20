#include "StageClearScene.h"
#include "StageSelectScene.h"
#include "TitleScene.h"
#include "GameMainScene.h"

#include "SimpleAudioEngine.h"
using namespace CocosDenshion;

int StageClear::mStageId = 0;
bool StageClear::mIsCleared = false;

USING_NS_CC;

//StageClear�y�[�W
//�Q�l�T�C�g...http://ladywendy.com/lab/cocos2d-x/
//Start��A������������Scene�ς�����STAGEselect�ɍs���������悢�H
//���̏ꍇ��Item�����ւ���R�[�h��Layer�����ւ���R�[�h�ɂ���


CCScene *StageClear::scene(int id,bool isCleared){
	
	mStageId = id;
	mIsCleared = isCleared;

    CCScene *scene = CCScene::create();
    
    StageClear *layer = StageClear ::create();


    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}
 
bool StageClear::init(){

	if (!CCLayerColor::initWithColor(ccc4(200, 230, 250, 255))) {
        return false;
    }

	mNumStage = 21;


	//�E�B���h�E�T�C�Y���擾
	CCSize winSize = CCDirector::sharedDirector()->getWinSize();
	CCSprite* backSprite = CCSprite::create("image/title.png");
	addChild( backSprite );
	backSprite->setPosition(ccp(winSize.width*0.5f, winSize.height*0.5f));

	//���S
    CCSprite *logo = CCSprite::create();
	if(mIsCleared){
		logo->initWithFile("message/clear.png");
		//�N���A�X�e�[�W��ۑ�����
		saveClearStage();
	}else{
		logo->initWithFile("message/miss.png");
	}
	logo->setPosition(ccp(winSize.width/2, winSize.height - logo->getContentSize().height*2));
    addChild(logo);

	 //���g���Cor���փ{�^����ǉ� 
	CCSprite* retrySprite = CCSprite::create();
	if(mIsCleared && mStageId < mNumStage){
		retrySprite->initWithFile("message/next.png");
	}else{
		retrySprite->initWithFile("message/retry.png");
	}
	CCMenuItemSprite *retryItem= CCMenuItemSprite::create(retrySprite, retrySprite, this, menu_selector(StageClear::menuCallback));  
	retryItem->setTag(0);
	retryItem->setPosition(ccp(winSize.width*0.5f, winSize.height*0.5f));
	//�߂�{�^��
	CCMenuItemImage *returnItem = CCMenuItemImage::create("message/return.png", "message/return.png", this, menu_selector(StageClear::menuCallback));
	returnItem->setTag(1);
	returnItem->setPosition(ccp(winSize.width*0.5f, winSize.height*0.5f - 150));
	
	//���j���[�̍쐬
	CCMenu* menu = CCMenu::create(retryItem,returnItem, NULL);
	menu->setPosition(ccp(0.0f,0.0f));
	addChild(menu);

     
    return true;
}

void StageClear::menuCallback(CCObject* pSender)
{ 
	int menuId = ( ( CCMenu* )pSender )->getTag();

	CCScene* nextScene;
	
	//���艹
	SimpleAudioEngine::sharedEngine()->playEffect("se/decision.wav");
	if(menuId == 0){
		if(mIsCleared && mStageId < mNumStage){
			//���ւ��^�b�`���ꂽ
			nextScene = GameMain::scene(mStageId+1);
		}else{
			//���g���C���^�b�`���ꂽ
			nextScene = GameMain::scene(mStageId);
		}
		
	}else{
		//�߂邪�^�b�`���ꂽ
		nextScene = StageSelect::scene();
	}
	//�؂�ւ��̎��Ԃ�ݒ�B�����ł�1�b
	float duration = 1.0f;

	//�t�F�[�h�C�����t�F�[�h�A�E�g
	CCScene* pScene = CCTransitionFade::create(duration, nextScene, ccc3( 0xff, 0xff, 0xff ));   
	if( pScene ){
		CCDirector::sharedDirector()->replaceScene( pScene );
	}
}

void StageClear::saveClearStage(){
	CCUserDefault* userDefault = CCUserDefault::sharedUserDefault();
	CCString* labelStr = CCString::createWithFormat( "stageCleared%d", mStageId-1 ) ;
	userDefault->setBoolForKey(labelStr->getCString(),true);
}