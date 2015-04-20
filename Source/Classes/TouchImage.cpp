#include "TouchImage.h"

using namespace cocos2d;

TouchImage::TouchImage() :
mCount( 0 ),
mAlpha( 0 ){
	mSprite = CCSprite::create( "image/touch.png" );
	mHitSprite = CCSprite::create( "image/touch_hit.png" );
	
	mSprite->setVisible( false );
	mHitSprite->setVisible( false );

	mSprite->setScale( 2.0f );
	mHitSprite->setScale( 2.0f );

	mSize = 64;
	mNum = 6;
}

TouchImage::~TouchImage(){
}

void TouchImage::setParent( cocos2d::CCLayer* layer, int z ){
	layer->addChild( mSprite, z );
	layer->addChild( mHitSprite, z );
}

void TouchImage::update( const CCPoint& position, bool enabe, bool hit ){
	mSprite->setPosition( position );
	mHitSprite->setPosition( position );

	
	if ( hit ){
		mSprite->setVisible( false );
		mHitSprite->setVisible( true );

		switchImage( mHitSprite, mCount % 48 / 8 );
	}else{
		mSprite->setVisible( true );
		mHitSprite->setVisible( false );

		switchImage( mSprite, mCount % 48 / 8 );
	}

	mSprite->setOpacity( mAlpha );
	mHitSprite->setOpacity( mAlpha );

	if ( enabe ){
		mAlpha += 10;
		if ( mAlpha > 180 ) mAlpha = 180;
	}else{
		mAlpha -= 10;
		if ( mAlpha < 0 ) mAlpha = 0;
	}

	++mCount;
}

void TouchImage::switchImage( CCSprite* out, int id ){
	int x = id % mNum;
	out->setTextureRect( CCRect( x * mSize, 0, mSize, mSize ) );
}
