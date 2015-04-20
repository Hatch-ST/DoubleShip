#include "StageSelectScene.h"
#include "GameMainScene.h"
#include "ScreenConverter.h"
#include "Ripple.h"
#include "Delete.h"
#include "TitleScene.h"

#include "SimpleAudioEngine.h"
using namespace CocosDenshion;

USING_NS_CC;

const int StageSelect::mButtonListLeft = 100;
const int StageSelect::mButtonListUp = 190;
const int StageSelect::mButtonIntervalX = 100;
const int StageSelect::mButtonIntervalY = 150;

StageSelect::~StageSelect(){
	SAFE_DELETE_ARRAY( mButtons );
	SAFE_DELETE_ARRAY( mNumLeftSprites );
	SAFE_DELETE_ARRAY( mNumRightSprites );
}

CCScene *StageSelect::scene(){
    CCScene *scene = CCScene::create();
    
    StageSelect *layer = StageSelect ::create();
    scene->addChild(layer);

    return scene;
}
 
bool StageSelect::init(){
	if ( !CCLayerColor::initWithColor( ccc4( 0xff, 0xff, 0xff, 0xff ) ) ){
		return false;
    }
	scheduleUpdate();

	//クリアステージの読み込み
	mUserDefault = CCUserDefault::sharedUserDefault();

	CCSprite* spriteBack = CCSprite::create( "image/stageBack.png" );
	CCSize winSize = CCDirector::sharedDirector()->getWinSize();
	spriteBack->setPosition( ccp( winSize.width / 2, winSize.height / 2 ) );
	addChild( spriteBack );

	mNumStage = 21;

	mButtons = new CCMenu*[ mNumStage ];
	mNumLeftSprites = new CCSprite*[ mNumStage ];
	mNumRightSprites = new CCSprite*[ mNumStage ];
	mCheckSprite = new CCSprite*[ mNumStage ];
	CCTexture2D* numberTex = CCTextureCache::sharedTextureCache()->addImage( "image/number.png" );
	for ( int i = 0; i < mNumStage; i++ ){
		CCMenuItemImage* item = CCMenuItemImage::create( "image/stagebutton_0.png", "image/stagebutton_1.png", this, menu_selector( StageSelect::stageSelectCallback ) );
		item->setTag( i );

		mButtons[ i ] = CCMenu::create( item, NULL );

		float x = mButtonListLeft + i * mButtonIntervalX;
		float y = mButtonListUp + ( i % 2 ) * mButtonIntervalY;
		ScreenConverter::toLeftUp( &y );

		mButtons[ i ]->setPosition( ccp( x, y ) );

		addChild( mButtons[ i ] );

		//クリアチェック
		mCheckSprite[ i ] = CCSprite::create( "image/clear_check.png" );
		mCheckSprite[ i ]->setPosition( ccp(x,y) );
		addChild(mCheckSprite[ i ]);
		CCString* keyStr = CCString::createWithFormat( "stageCleared%d", i ) ;
		if(!mUserDefault->getBoolForKey(keyStr->getCString(),false)){
			mCheckSprite[ i ]->setVisible( false );	
		}

		int n = i + 1;
		CCRect rect;
		rect.setRect( ( n % 10 ) * 32, 0, 32, 32 );
		mNumRightSprites[ i ] = CCSprite::createWithTexture( numberTex, rect );

		rect.setRect( ( n / 10 ) * 32, 0, 32, 32 );
		mNumLeftSprites[ i ] = CCSprite::createWithTexture( numberTex, rect );

		mNumRightSprites[ i ]->setPosition( ccp( x + 16, y ) );
		mNumLeftSprites[ i ]->setPosition( ccp( x - 16, y ) );

		addChild( mNumRightSprites[ i ] );
		addChild( mNumLeftSprites[ i ] );

		
	}

	mPlayerA = CCSprite::create( "image/ship_stage.png" );
	mPlayerB = CCSprite::create( "image/ship_stage.png" );

	mPlayerA->setPosition( ccp( -100.0f, 90.0f ) );
	mPlayerA->setRotation( 90 );
	mPlayerB->setPosition( ccp( winSize.width + 100.0f, winSize.height - 90.0f ) );
	mPlayerB->setRotation( -90 );

	addChild( mPlayerA, 1 );
	addChild( mPlayerB, 1 );

	for ( int i = 0; i < 5; i++ ){
		mRipplesA[ i ] = new Ripple( -30 * i - 1, true );
		mRipplesA[ i ]->setParent( this, 0 );
		mRipplesB[ i ] = new Ripple( -30 * i - 1, true );
		mRipplesB[ i ]->setParent( this, 0 );
	}
	
	setTouchMode( kCCTouchesOneByOne );
	setTouchEnabled( true );
	mIsTouch = false;

	mOnSequence = false;
	mTimeCount = 0;

    return true;
}

void StageSelect::update( float delta ){
	mPlayerA->setPositionX( mPlayerA->getPositionX() + 2.0f );
	mPlayerB->setPositionX( mPlayerB->getPositionX() - 2.0f );

	for ( int i = 0; i < 5; i++ ){
		mRipplesA[ i ]->update( mPlayerA->getPosition(), ccp( 0, 0 ) );
		mRipplesB[ i ]->update( mPlayerB->getPosition(), ccp( 0, 0 ) );
	}

	if ( mPlayerA->getPositionX() > CCDirector::sharedDirector()->getWinSize().width + 200 ){
		mPlayerA->setPosition( ccp( -100.0f, 100.0f ) );
	}

	if ( mPlayerB->getPositionX() < -200 ){
		CCSize winSize = CCDirector::sharedDirector()->getWinSize();
		mPlayerB->setPosition( ccp( winSize.width + 100.0f, winSize.height - 90.0f ) );
	}

	if ( mIsTouch ){
		mMove = ( mTouchedPos - mPrevTouchedPos ) * 1.2f;
	}else{
		mMove = mMove * 0.9f;
	}

	bool validMove = true;
	if ( mMove.x > 0 && mButtons[ 0 ]->getPositionX() > mButtonListLeft ){
		validMove = false;
	}
	CCSize winSize = CCDirector::sharedDirector()->getWinSize();
	if ( mMove.x < 0 && mButtons[ mNumStage - 1 ]->getPositionX() < winSize.width - mButtonListLeft ){
		validMove = false;
	}

	if ( validMove ){
		for ( int i = 0; i < mNumStage; i++ ){
			mButtons[ i ]->setPositionX( mButtons[ i ]->getPositionX() + mMove.x );
			mNumLeftSprites[ i ]->setPositionX( mNumLeftSprites[ i ]->getPositionX() + mMove.x );
			mNumRightSprites[ i ]->setPositionX( mNumRightSprites[ i ]->getPositionX() + mMove.x );
			mCheckSprite[ i ]->setPositionX( mCheckSprite[ i ]->getPositionX() + mMove.x );
		}
	}

	++mTimeCount;
}

void StageSelect::stageSelectCallback( CCObject* pSender ){ 
	if ( mTimeCount < 60 || mOnSequence ) return;
	mOnSequence = true;
	//決定音
	SimpleAudioEngine::sharedEngine()->playEffect("se/decision.wav");
	int id = ( ( CCMenu* )pSender )->getTag();

	//次のシーンとして、StageSelectシーンをセット
	CCScene* nextScene = GameMain::scene( id + 1 );

	//切り替えの時間を設定。ここでは1秒
	float duration = 1.0f;

	//フェードイン＆フェードアウト（CCTransitionFade）でトランジション
	CCScene* pScene = CCTransitionFade::create(duration, nextScene, ccc3( 0xff, 0xff, 0xff ));   
	if( pScene ){
		CCDirector::sharedDirector()->replaceScene( pScene );
	}
}

bool StageSelect::ccTouchBegan( CCTouch *pTouch, CCEvent *pEvent ){
	mIsTouch = true;
	mTouchedPos.setPoint( pTouch->getLocation().x, pTouch->getLocation().y );
	mPrevTouchedPos = mTouchedPos;
	return true;
}

void StageSelect::ccTouchMoved( CCTouch *pTouch, CCEvent *pEvent ){
	mPrevTouchedPos = mTouchedPos;
	mTouchedPos.setPoint( pTouch->getLocation().x, pTouch->getLocation().y );
}

void StageSelect::ccTouchEnded( CCTouch *pTouch, CCEvent *pEvent ){
	mIsTouch = false;
}

void StageSelect::keyBackClicked(){
	if ( mTimeCount < 90 || mOnSequence ) return;
	mOnSequence = true;

	//ステージセレクトに戻る
	CCScene* nextScene = Title::scene();

	//切り替えの時間を設定。ここでは1秒
	float duration = 1.0f;

	//フェードイン＆フェードアウト
	CCScene* pScene = CCTransitionFade::create(duration, nextScene, ccc3( 0xff, 0xff, 0xff ));   
	if( pScene ){
		CCDirector::sharedDirector()->replaceScene( pScene );
	}
}

//シーンが始まった時に呼び出されるメソッド
void StageSelect::onEnter(){
    CCLayer::onEnter();
	//ハードキー有効
	this->setKeypadEnabled(true);
}