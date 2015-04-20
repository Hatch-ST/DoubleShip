#include "MoveWall.h"
#include "ScreenConverter.h"
#include "Collision.h"
#include "GameMainScene.h"

USING_NS_CC;
using namespace GameLib;

MoveWall::MoveWall( b2World* world, cocos2d::CCPoint pos, float scale, float angle, Color color, float upMax, float downMax ){
	mPosition = pos;
	mPosition.y = ScreenConverter::convertY( pos.y );

	mDefaultPosition = mPosition;
	mColor = color;
	mAngle = angle;
	mMoveDirection = ccp( 0, -1 ).rotateByAngle( ccp( 0, 0 ), -angle );
	
	mUpMax = upMax;
	mDownMax = downMax;
	mReturnMoveValue = 10.0f;
	
	mRedTexture = CCTextureCache::sharedTextureCache()->addImage( "image/object/wall_red.png" );
	mBlueTexture = CCTextureCache::sharedTextureCache()->addImage( "image/object/wall_blue.png" );
	mGreenTexture = CCTextureCache::sharedTextureCache()->addImage( "image/object/wall_blue.png" );
	
	int groupIndex = 0;
	switch( color ){
		case COLOR_RED:
			mSprite = CCSprite::create();
			mSprite->initWithTexture( mRedTexture );
			groupIndex = FixtureGroup::GR_RED;
			break;
		case COLOR_BLUE:
			mSprite = CCSprite::create();
			mSprite->initWithTexture( mBlueTexture );
			groupIndex = FixtureGroup::GR_BLUE;
			break;
		case COLOR_GREEN:
			mSprite = CCSprite::create();
			mSprite->initWithTexture( mGreenTexture );
			groupIndex = FixtureGroup::GR_GREEN;
			break;
		case COLOR_BLACK_S:
		case COLOR_BLACK_M:
			mSprite = CCSprite::create( "image/object/wall_black.png" );
			groupIndex = FixtureGroup::GR_NONE;
			break;
		case COLOR_BLACK_L:
			mSprite = CCSprite::create( "image/object/wall_black_b.png" );
			groupIndex = FixtureGroup::GR_NONE;
			break;
	}
	
	mSprite->setPosition( mPosition );
	mSprite->setScaleY( scale );
	mSprite->setRotation( mAngle * 180 / M_PI );

	mSize = mSprite->getContentSize();
	mSize.height *= scale;
	
	if ( color == COLOR_BLACK_L ){
		mSideSprite[ 0 ] = CCSprite::create( "image/object/wallside_big.png" );
		mSideSprite[ 1 ] = CCSprite::create( "image/object/wallside_big.png" );
	}else{
		mSideSprite[ 0 ] = CCSprite::create( "image/object/wallside.png" );
		mSideSprite[ 1 ] = CCSprite::create( "image/object/wallside.png" );
	}
	
	mSideDistance.setPoint( 0, mSize.height / 2 - mSideSprite[ 0 ]->getContentSize().height / 2 );
	mSideDistance = mSideDistance.rotateByAngle( ccp( 0, 0 ), -mAngle );

	mSideSprite[ 0 ]->setPosition( mSprite->getPosition() + mSideDistance );
	mSideSprite[ 0 ]->setRotation( mAngle * 180 / M_PI );
	mSideSprite[ 1 ]->setPosition( mSprite->getPosition() - mSideDistance );
	mSideSprite[ 1 ]->setRotation( mAngle * 180 / M_PI );

	mArrowSprite = CCSprite::create( "image/object/move.png" );
	mArrowSprite->setPosition( mPosition );
	mArrowSprite->setRotation( mAngle * 180 / M_PI );
	
	
	b2BodyDef bodyDef;
	bodyDef.type = b2_staticBody;
	bodyDef.userData = mSprite;

	mBody = world->CreateBody( &bodyDef );
	
	float width = mSize.width * 0.5f;
	float height = mSize.height * 0.5f;
	mPolygonShape.SetAsBox( width / GameMain::mPtmRatio, height / GameMain::mPtmRatio );

	mFixtureDef.shape = &mPolygonShape;
	mFixtureDef.density = 0.0f;
	mFixtureDef.filter.groupIndex = groupIndex;

	mBody->CreateFixture( &mFixtureDef );
	mBody->SetTransform( b2Vec2( mPosition.x / GameMain::mPtmRatio, mPosition.y / GameMain::mPtmRatio ), -mAngle );

	mCount = 0;
	mMoving = false;
	mCollisionSize = mSize;
	mCollisionSize.width += 48;
}

void MoveWall::setParent( cocos2d::CCLayer* layer, cocos2d::CCLayer* underLayer ){
	layer->addChild( mSprite );
	layer->addChild( mArrowSprite );
	layer->addChild( mSideSprite[ 0 ] );
	layer->addChild( mSideSprite[ 1 ] );
}

void MoveWall::update( TouchManager* tm, StageLayer* sl ){
	mSprite->setPosition( ccp( mBody->GetPosition().x * GameMain::mPtmRatio, mBody->GetPosition().y * GameMain::mPtmRatio ) );
	mSprite->setRotation( -mBody->GetAngle() * 180.0f / M_PI );
	
	mArrowSprite->setPosition( ccp( mBody->GetPosition().x * GameMain::mPtmRatio, mBody->GetPosition().y * GameMain::mPtmRatio ) );
	mArrowSprite->setRotation( -mBody->GetAngle() * 180.0f / M_PI );

	mSideSprite[ 0 ]->setPosition( ccp( mBody->GetPosition().x * GameMain::mPtmRatio, mBody->GetPosition().y * GameMain::mPtmRatio ) + mSideDistance );
	mSideSprite[ 0 ]->setRotation( -mBody->GetAngle() * 180.0f / M_PI );
	mSideSprite[ 1 ]->setPosition( ccp( mBody->GetPosition().x * GameMain::mPtmRatio, mBody->GetPosition().y * GameMain::mPtmRatio ) - mSideDistance );
	mSideSprite[ 1 ]->setRotation( -mBody->GetAngle() * 180.0f / M_PI );

	bool hit = false;
	for ( int i = 0; i < tm->getTouchNum(); i++ ){
		if ( tm->getTouchObject( i )->touchBeganFlag || ( mMoving && tm->getTouchObject( i )->touchFlag ) ){
			CCPoint touchPos( tm->getTouchObject( i )->touch_past_x, tm->getTouchObject( i )->touch_past_y );
			if ( Collision::testIntersectPositionAndSquare( touchPos, mSprite->getPosition(), mCollisionSize.width, mCollisionSize.height, -mAngle ) ){

				CCPoint move( tm->getTouchObject( i )->touch_moved_x, tm->getTouchObject( i )->touch_moved_y );
				move = mMoveDirection * move.dot( mMoveDirection );

				mPosition = mPosition + move;
			
				// ˆÚ“®”ÍˆÍ‚ð§ŒÀ
		 		float dot = mMoveDirection.dot( mPosition - mDefaultPosition );
				float distance = ccpDistance( mPosition, mDefaultPosition );
				if ( dot > 0 && distance > mUpMax ){
					mPosition = mDefaultPosition + mMoveDirection * mUpMax;
				}else if ( dot < 0 && distance > mDownMax ){
					mPosition = mDefaultPosition + mMoveDirection * ( -mDownMax );
				}

				mMoving = true;
				hit = true;
				break;
			}
		}
	}
	
	if ( hit ){
		sl->stopScroll();
	}
	// ƒ^ƒbƒ`‚³‚ê‚Ä‚¢‚È‚¯‚ê‚Î–ß‚·
	else{
		CCPoint move = ( mDefaultPosition - mPosition ).normalize() * mReturnMoveValue;
		mPosition = mPosition + move;

		if ( ccpDistance( mPosition, mDefaultPosition ) < mReturnMoveValue * 1.2f ){
			mPosition = mDefaultPosition;
		}

		mMoving = false;
	}
	
	b2Vec2 dest;
	dest.x = ( mPosition.x - sl->screenPosition().x ) / GameMain::mPtmRatio;
	dest.y = ( mPosition.y + sl->screenPosition().y ) / GameMain::mPtmRatio;
	mBody->SetTransform( dest, mBody->GetAngle() );
	
	mVelocity.Set( ( mCount % 2 == 0 ) ? 0.001f : -0.001f, 0.0f );
	mBody->SetLinearVelocity( mVelocity );
	++mCount;
}

void MoveWall::setColor( Color color ){
	if ( mColor != COLOR_RED && mColor != COLOR_BLUE && mColor != COLOR_GREEN ){
		return;
	}

	int groupIndex = 0;
	switch( color ){
		case COLOR_RED:
			mSprite->setTexture( mRedTexture );
			groupIndex = FixtureGroup::GR_RED;
			break;
		case COLOR_BLUE:
			mSprite->setTexture( mBlueTexture );
			groupIndex = FixtureGroup::GR_BLUE;
			break;
		case COLOR_GREEN:
			mSprite->setTexture( mGreenTexture );
			groupIndex = FixtureGroup::GR_GREEN;
			break;
	}
	
	mFixtureDef.filter.groupIndex = groupIndex;
	mBody->DestroyFixture( mBody->GetFixtureList() );
	mBody->CreateFixture( &mFixtureDef );
}

void MoveWall::resetColor(){
	setColor( mColor );
}