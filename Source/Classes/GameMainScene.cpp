#include "GameMainScene.h"
#include "StageSelectScene.h"
#include "StageLayer.h"
#include "ObjectLayer.h"
#include "MoveWall.h"
#include "StaticWall.h"
#include "ElasticWall.h"
#include "Delete.h"
#include "StageClearScene.h"
#include "Ripple.h"
#include "TouchImage.h"

#include "SimpleAudioEngine.h"
using namespace CocosDenshion;

USING_NS_CC;


#define PTM_RATIO 32.0
const float GameMain::mPtmRatio = 32.0f;
int GameMain::mStageId = 0;

GameMain::~GameMain(){
	SAFE_DELETE( mTouchManager );
	SAFE_DELETE( mPlayer );
	SAFE_DELETE( mWorld );
	SAFE_DELETE( mDebugDraw );
	SAFE_DELETE_ARRAY( mTouchImages );
}

CCScene* GameMain::scene( int id ){
    CCScene *scene = CCScene::create();
    
    GameMain *layer = GameMain::create();
	layer->loadStage( id );
	mStageId = id;
    scene->addChild(layer);

    return scene;
}

bool GameMain::init(){
	if ( !CCLayer::init() ){
		return false;
	}
    
    CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
    CCPoint origin = CCDirector::sharedDirector()->getVisibleOrigin();

	//TouchManager初期化
	int touchNum = 10;
	mTouchManager = new TouchManager(touchNum);	

	this->scheduleUpdate();
	this->setTouchEnabled(true);

	mStageLayer = StageLayer::create();
	this->addChild(mStageLayer,-2);
	
	mObjectLayer = ObjectLayer::create();
	this->addChild( mObjectLayer, 1 );
	this->addChild( mObjectLayer->switchLayer() );
	mObjectLayer->setStageLayer( mStageLayer );

	//物理エンジンの初期化
	b2Vec2 gravity = b2Vec2(0.0f, 0.0f); 
	mWorld = new b2World(gravity);
	mDebugDraw = new GLESDebugDraw( PTM_RATIO );
    mWorld->SetDebugDraw(mDebugDraw);
	uint32 flags = 0;
	flags += b2Draw::e_shapeBit;
	//物理判定用描画
	//mDebugDraw->SetFlags(flags);

	//Player初期化
	mPlayer = new PhysicsPlayer(this,*mWorld);
	//mPlayer->setStartPos(ccp(200,300),0.0f,ccp(300,200),1.57f);

	for ( int i = 0; i < 5; i++ ){
		mRipplesA[ i ] = new Ripple( -30 * i - 1, false );
		mRipplesA[ i ]->setParent( this, -1 );
		mRipplesB[ i ] = new Ripple( -30 * i - 1, false );
		mRipplesB[ i ]->setParent( this, -1 );
	}

	
	mDeadCount = 0;
	mTimeCount = 0;
	mOnReady = true;
	mOnSequence = false;

	mTouchImages = new TouchImage[ mTouchManager->getTouchNum() ];
	for ( int i = 0; i < mTouchManager->getTouchNum(); i++ ){
		mTouchImages[ i ].setParent( this, 2 );
	}




    return true;
}

void GameMain::draw()
{
    CCLayer::draw();
    ccGLEnableVertexAttribs( kCCVertexAttribFlag_Position );
    kmGLPushMatrix();
    mWorld->DrawDebugData();
    kmGLPopMatrix();
}

void GameMain::loadStage( int id ){
	mStageLayer->load( id, mWorld );
	mObjectLayer->load( id, mWorld );
	
	mObjectLayer->update( mTouchManager, mPlayer );
	mStageLayer->update();

	mPlayer->setStartPos( mObjectLayer->getFirstPlayerPosition( 0 ), -M_PI / 2, mObjectLayer->getFirstPlayerPosition( 1 ), -M_PI / 2 );
}

//シーンが始まった時に呼び出されるメソッド
void GameMain::onEnter(){
    CCLayer::onEnter();
	//ハードキー有効
	this->setKeypadEnabled(true);
}

void GameMain::update( float delta ){
	if ( mOnReady ){
		if ( mPlayer->ready( mTouchManager ) ){
			mOnReady = false;
		}
	}else{
		mObjectLayer->update( mTouchManager, mPlayer );
		mStageLayer->update();
		mPlayer->update( delta, mTouchManager, *mStageLayer, *mObjectLayer );
	
		for ( int i = 0; i < 5; i++ ){
			mRipplesA[ i ]->update( mPlayer->getPosition( 0 ), -mStageLayer->scrollValue() );
			mRipplesB[ i ]->update( mPlayer->getPosition( 1 ), -mStageLayer->scrollValue() );
		}
		
		if ( mPlayer->isDead() ){
			if ( mDeadCount == 120 ){
				mOnSequence = true;
				CCScene* next = CCTransitionFade::create( 1.0f, StageClear::scene(mStageId,false) );
				CCDirector::sharedDirector()->replaceScene( next );
			}
			++mDeadCount;
		}

		if ( mPlayer->isCleared() ){
			if ( mDeadCount == 20 ){
				mOnSequence = true;
				CCScene* next = CCTransitionFade::create( 1.0f, StageClear::scene(mStageId,true) );
				CCDirector::sharedDirector()->replaceScene( next );
			}
			++mDeadCount;
		}

	}

	for ( int i = 0; i < mTouchManager->getTouchNum(); i++ ){
		bool hit = mPlayer->isTouched( i );
		mTouchImages[ i ].update( mTouchManager->getTouchPoint( i ), mTouchManager->getTouchObject( i )->touchFlag, hit );
	}

	mTouchManager->update( delta );
	mWorld->Step( delta, 10, 10 );

	++mTimeCount;
}

void GameMain::ccTouchesBegan(cocos2d::CCSet *pTouches, cocos2d::CCEvent *pEvent){
	CC_UNUSED_PARAM(pEvent);
    for (CCSetIterator it = pTouches->begin(); it != pTouches->end(); ++it)
    {
        CCTouch *touch = (CCTouch *)(*it);
        mTouchManager->setTouchBegan(touch->getID(),(int)touch->getLocation().x,(int)touch->getLocation().y);
    }
}
void GameMain::ccTouchesMoved(cocos2d::CCSet *pTouches, cocos2d::CCEvent *pEvent){
	CC_UNUSED_PARAM(pEvent);
    for (CCSetIterator it = pTouches->begin(); it != pTouches->end(); ++it)
    {
        CCTouch *touch = (CCTouch *)(*it);
		mTouchManager->setTouchMoved(touch->getID(),(int)touch->getLocation().x,(int)touch->getLocation().y);
    }
}
void GameMain::ccTouchesCancelled(cocos2d::CCSet *pTouches, cocos2d::CCEvent *pEvent){
	CC_UNUSED_PARAM(pEvent);
    for (CCSetIterator it = pTouches->begin(); it != pTouches->end(); ++it)
    {
        CCTouch *touch = (CCTouch *)(*it);
		mTouchManager->setTouchCancelled(touch->getID(),(int)touch->getLocation().x,(int)touch->getLocation().y);
    }
}
void GameMain::ccTouchesEnded(cocos2d::CCSet* pTouches, cocos2d::CCEvent* pEvent){
	CC_UNUSED_PARAM(pEvent);
    for (CCSetIterator it = pTouches->begin(); it != pTouches->end(); ++it)
    {
        CCTouch *touch = (CCTouch *)(*it);
        mTouchManager->setTouchEnded(touch->getID(),(int)touch->getLocation().x,(int)touch->getLocation().y);
	}
}

void GameMain::keyBackClicked(){
	if ( mTimeCount < 90 || mOnSequence ) return;
	mOnSequence = true;

	//ステージセレクトに戻る
	CCScene* nextScene = StageSelect::scene();
	
	//切り替えの時間を設定。ここでは1秒
	float duration = 1.0f;

	//フェードイン＆フェードアウト
	CCScene* pScene = CCTransitionFade::create(duration, nextScene, ccc3( 0xff, 0xff, 0xff ));   
	if( pScene ){
		CCDirector::sharedDirector()->replaceScene( pScene );
	}
}

void GameMain::keyMenuClicked(){
	if ( mTimeCount < 90 || mOnSequence ) return;
	mOnSequence = true;

	//リトライに戻る
	CCScene* nextScene = GameMain::scene( mStageId );

	//切り替えの時間を設定。ここでは1秒
	float duration = 1.0f;

	//フェードイン＆フェードアウト
	CCScene* pScene = CCTransitionFade::create(duration, nextScene, ccc3( 0xff, 0xff, 0xff ));   
	if( pScene ){
		CCDirector::sharedDirector()->replaceScene( pScene );
	}
}

