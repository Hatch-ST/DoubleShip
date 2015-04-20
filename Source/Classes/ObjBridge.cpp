#include "ObjBridge.h"
#include "Matrix34.h"
#include "Delete.h"
#include "Screenconverter.h"
#include "Collision.h"
#include <string>

using namespace cocos2d;
using namespace GameLib;
using namespace std;

const int ObjBridge::mUpAndLowSizeY = 48;
const int ObjBridge::mMiddleSizeY = 16;

ObjBridge::ObjBridge( int x, int y, int height, float angle, bool big ){
	string upName = "", lowName = "", midName = "";
	float width;
	if ( !big ){
		width = 64;
		upName = "image/object/bridge_up_b.png";
		lowName = "image/object/bridge_low_b.png";
		midName = "image/object/bridge_mid_b.png";
	}else{
		width = 64;
		upName = "image/object/bridge_up_b.png";
		lowName = "image/object/bridge_low_b.png";
		midName = "image/object/bridge_mid_b.png";
	}

	mUpSprite = CCSprite::create( upName.c_str() );
	mLowSprite = CCSprite::create( lowName.c_str() );

	mNumMiddle = ( height - ( mUpAndLowSizeY * 2 ) ) / mMiddleSizeY;
	mMiddleSprites = new CCSprite*[ mNumMiddle ]();
	for ( int i = 0; i < mNumMiddle; i++ ){
		mMiddleSprites[ i ] = CCSprite::create( midName.c_str() );
	}

	mSize.set( width, static_cast<float>( height ) );
	mScaleY = 1.0f;
	mAngle = angle;

	mShadowSprite = CCSprite::create( "image/object/bridge_shadow.png" );


	// ŠeˆÊ’u‚ð“o˜^
	mPosition.set( x, y );
	mUpPosition.y = -height / 2 + mUpAndLowSizeY / 2;
	mLowPosition.y = height / 2 - mUpAndLowSizeY / 2;
	mMiddlePositions = new Vector2[ mNumMiddle ];
	for ( int i = 0; i < mNumMiddle; i++ ){
		mMiddlePositions[ i ].y = mUpPosition.y + ( mUpAndLowSizeY / 2 );
		mMiddlePositions[ i ].y += mMiddleSizeY / 2 + i * mMiddleSizeY;
	}

	// ‰ñ“]
	Matrix34 rota;
	rota.setRotationZ( mAngle );
	mUpPosition *= rota;
	mLowPosition *= rota;
	for ( int i = 0; i < mNumMiddle; i++ ){
		mMiddlePositions[ i ] *= rota;
	}
	
	// À•W‚ð“o˜^
	ScreenConverter::putSprite( mUpSprite, mPosition + mUpPosition );
	ScreenConverter::putSprite( mLowSprite, mPosition + mLowPosition );
	mUpSprite->setRotation( mAngle * 180.0f / M_PI );
	mLowSprite->setRotation( mAngle * 180.0f / M_PI );
	for ( int i = 0; i < mNumMiddle; i++ ){
		ScreenConverter::putSprite( mMiddleSprites[ i ], mPosition + mMiddlePositions[ i ] );
		mMiddleSprites[ i ]->setRotation( mAngle * 180.0f / M_PI );
		mMiddleSprites[ i ]->setScaleY( 1.05f );
	}
	mShadowSprite->setScaleY( height / mShadowSprite->getContentSize().height );
	mShadowSprite->setRotation( mAngle * 180.0f / M_PI );
	ScreenConverter::putSprite( mShadowSprite, mPosition );
}

ObjBridge::~ObjBridge(){
	//mUpSprite->release();
	//mLowSprite->release();
	//for ( int i = 0; i < mNumMiddle; i++ ){
	//	mMiddleSprites[ i ]->release();
	//}

	SAFE_DELETE_ARRAY( mMiddleSprites );
	SAFE_DELETE_ARRAY( mMiddlePositions );
}

void ObjBridge::setParent( cocos2d::CCLayer* layer, cocos2d::CCLayer* underLayer ){
	layer->addChild( mUpSprite );
	layer->addChild( mLowSprite );

	for ( int i = 0; i < mNumMiddle; i++ ){
		layer->addChild( mMiddleSprites[ i ] );
	}

	underLayer->addChild( mShadowSprite );
}

void ObjBridge::update( TouchManager* tm, StageLayer* sl ){
	ScreenConverter::putSprite( mUpSprite, mPosition + mUpPosition - sl->screenPosition() );
	ScreenConverter::putSprite( mLowSprite, mPosition + mLowPosition - sl->screenPosition() );
	for ( int i = 0; i < mNumMiddle; i++ ){
		ScreenConverter::putSprite( mMiddleSprites[ i ], mPosition + mMiddlePositions[ i ] - sl->screenPosition() );
	}
	ScreenConverter::putSprite( mLowSprite, mPosition + mLowPosition - sl->screenPosition() );
	ScreenConverter::putSprite( mShadowSprite, mPosition - sl->screenPosition() );
}

bool ObjBridge::isIntersect( cocos2d::CCPoint position, const StageLayer& sl ) const{
	Vector2 pos( position.x, ScreenConverter::convertY( position.y ) );
	return ( Collision::testIntersectPositionAndSquare( pos, mPosition - sl.screenPosition(), mSize.x, mSize.y, mAngle ) );
}