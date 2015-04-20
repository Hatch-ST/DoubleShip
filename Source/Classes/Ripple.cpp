#include "Ripple.h"

using namespace cocos2d;

Ripple::Ripple( int firstTime, bool mini ){
	if ( mini ){
		mTexture = CCTextureCache::sharedTextureCache()->addImage( "image/ripple_min.png" );
		mSize = 64;
	}else{
		mTexture = CCTextureCache::sharedTextureCache()->addImage( "image/ripple.png" );
		mSize = 128;
	}

	mSprite = CCSprite::createWithTexture( mTexture );
	mSprite->setPosition(ccp(-500,0));
	switchImage( mSprite, 0 );

	mCount = firstTime;
}

Ripple::~Ripple(){
}

void Ripple::setParent( cocos2d::CCLayer* layer, int z ){
	layer->addChild( mSprite, z );
}

void Ripple::update( CCPoint position, CCPoint move ){
	int alpha = ( mCount < 0 ) ? 0 : ( 255 - mCount * ( 255 / 40 ) );
	mSprite->setOpacity( alpha );

	switchImage( mSprite, mCount % 40 / 10 );
	mSprite->setPosition( mSprite->getPosition() + move );

	if ( mCount == 0 ){
		mSprite->setPosition( position );
	}
	if ( mCount == 40 ){
		mCount = -1;
	}

	++mCount;
}

void Ripple::switchImage( CCSprite* out, int id ){
	int x = id % 4;
	out->setTextureRect( CCRect( x * mSize, 0, mSize, mSize ) );
}
