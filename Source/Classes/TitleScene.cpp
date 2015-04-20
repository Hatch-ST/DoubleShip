#include "TitleScene.h"
#include "StageSelectScene.h"
#include "Delete.h"

#include "SimpleAudioEngine.h"
using namespace CocosDenshion;

USING_NS_CC;

//Title�y�[�W
//�Q�l�T�C�g...http://ladywendy.com/lab/cocos2d-x/
//Start��A������������Scene�ς�����STAGEselect�ɍs���������悢�H
//���̏ꍇ��Item�����ւ���R�[�h��Layer�����ւ���R�[�h�ɂ���

const float Title::mScrollVelocity = 0.5f;

Title::~Title(){
	SAFE_DELETE_ARRAY( mBackSprites );
}

CCScene *Title::scene(){
     CCScene *scene = CCScene::create();
    
    Title *layer = Title ::create();

    scene->addChild(layer);

    return scene;
}
 
bool Title::init(){

	if (!CCLayerColor::initWithColor(ccc4(200, 230, 250, 255))) {
        return false;
    }

	scheduleUpdate();

  
	//// ���j���[�{�^����ǉ� 
	//CCMenuItemImage *stageSelectItem = CCMenuItemImage::create("start.png", "start2.png", this, menu_selector(Title::menuSelectCallback));
	//// ���j���[�̍쐬
	//CCMenu* stageSelectBtn = CCMenu::create(stageSelectItem, NULL);
  
	//// �I���{�^����ǉ� 
	//CCMenuItemImage *CloseItem = CCMenuItemImage::create("exit.png", "exit2.png", this, menu_selector(Title::menuCloseCallback));
	//// ���j���[�̍쐬
	//CCMenu* CloseBtn = CCMenu::create(CloseItem, NULL);
     
	////Btn�z�u
	//stageSelectBtn->setPosition(ccp(winSize.width/2, winSize.height/5*2));
	//addChild(stageSelectBtn);
	//CloseBtn->setPosition(ccp(winSize.width-20, 20));
	//addChild(CloseBtn);

	mBackTex = CCTextureCache::sharedTextureCache()->addImage( "image/title.png" );

	mBackSprites = new CCSprite*[ 2 ];
	mBackSprites[ 0 ] = CCSprite::createWithTexture( mBackTex );
	mBackSprites[ 1 ] = CCSprite::createWithTexture( mBackTex );

	addChild( mBackSprites[ 0 ] );
	addChild( mBackSprites[ 1 ] );

	mHalfBackSize.setPoint( mBackTex->getContentSize().width / 2, mBackTex->getContentSize().height / 2 );

	mBackSprites[ 0 ]->setPosition( ccp( 0, 0 ) + mHalfBackSize );
	mBackSprites[ 1 ]->setPosition( ccp( mBackTex->getContentSize().width - 2, 0 ) + mHalfBackSize );

	mScreenX = 0;



	// �E�B���h�E�T�C�Y���擾
	CCSize winSize = CCDirector::sharedDirector()->getWinSize();
     
	// CCSprite���摜�@setPosition�ňʒu������Ƃ肠����logo�H
    CCSprite *titleLogo = CCSprite::create("image/title_logo.png");
	titleLogo->setPosition(ccp(winSize.width*0.5f, winSize.height*0.5f + 100));
	
	// ���S��ǉ��B
    addChild(titleLogo);
	
	//�͂��߂�{�^����ǉ� 
	CCMenuItemImage *startItem = CCMenuItemImage::create("message/start.png", "message/start.png", this, menu_selector(Title::menuCallback));  
	startItem->setPosition(ccp(winSize.width*0.5f, winSize.height*0.5f-50));

	//���j���[�̍쐬
	CCMenu* menu = CCMenu::create(startItem, NULL);
	menu->setPosition(ccp(0.0f,0.0f));
	addChild(menu);

	
	setTouchMode( kCCTouchesOneByOne );
	setTouchEnabled( true );
	//�n�[�h�L�[�L��
	this->setKeypadEnabled(true);

	//SE�̉��ʂ̐ݒ�
	SimpleAudioEngine::sharedEngine()->setEffectsVolume(1.0f);
	//SE���������Ă���
	SimpleAudioEngine::sharedEngine()->preloadEffect("se/decision.wav");
	SimpleAudioEngine::sharedEngine()->preloadEffect("se/drop.wav");
	SimpleAudioEngine::sharedEngine()->preloadEffect("se/up.wav");

    return true;
}

void Title::menuCallback(CCObject* pSender)
{ 
	//���艹
	SimpleAudioEngine::sharedEngine()->playEffect("se/decision.wav");
	//���̃V�[���Ƃ��āAStageSelect�V�[�����Z�b�g
    CCScene* nextScene =StageSelect::scene();
    //�؂�ւ��̎��Ԃ�ݒ�B�����ł�1�b
    float duration = 1.0f;
    //�t�F�[�h�C�����t�F�[�h�A�E�g�iCCTransitionFade�j�Ńg�����W�V����
    CCScene* pScene = CCTransitionFade::create(duration, nextScene, ccc3( 0xff, 0xff, 0xff ) );   
    if(pScene){
        CCDirector::sharedDirector()->replaceScene(pScene);
	}
}

void Title::update( float delta ){
	for ( int i = 0; i < 2; i++ ){
		mBackSprites[ i ]->setPositionX( mBackSprites[ i ]->getPositionX() - mScrollVelocity );

		if ( mBackSprites[ i ]->getPositionX() < -mHalfBackSize.x ){
			float dif = mBackSprites[ i ]->getPositionX() - ( -mHalfBackSize.x ) - 2;
			mBackSprites[ i ]->setPosition( ccp( mBackTex->getContentSize().width + dif, 0 ) + mHalfBackSize );
		}
	}
}
bool Title::ccTouchBegan( CCTouch *pTouch, CCEvent *pEvent ){
	menuCallback(NULL);
	return true;
}

void Title::keyBackClicked(){
	#if (CC_TARGET_PLATFORM == CC_PLATFORM_WINRT) || (CC_TARGET_PLATFORM == CC_PLATFORM_WP8)
		CCMessageBox("You pressed the close button. Windows Store Apps do not implement a close button.","Alert");
	#else
		CCDirector::sharedDirector()->end();
	#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
		exit(0);
	#endif
	#endif
}

