#include "staticWall.h"
#include "ScreenConverter.h"
#include "Collision.h"
#include "GameMainScene.h"

USING_NS_CC;
using namespace GameLib;

StaticWall::StaticWall( b2World* world, cocos2d::CCPoint pos, float scale, float angle, Color color ){
	mPosition = pos;
	mPosition.y = ScreenConverter::convertY( pos.y );

	mColor = color;
	mAngle = angle;
	
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

	mSize = mSprite->getContentSize();
	mSize.height *= scale;
	
	ScreenConverter::putSprite( mSprite, mPosition );
	mSprite->setScaleY( scale );
	mSprite->setRotation( mAngle * 180.0f / M_PI );

	if ( groupIndex == FixtureGroup::GR_NONE ){
		if ( color == COLOR_BLACK_L ){
			mSideSprite[ 0 ] = CCSprite::create( "image/object/wallside_static_big.png" );
			mSideSprite[ 1 ] = CCSprite::create( "image/object/wallside_static_big.png" );
		}else{
			mSideSprite[ 0 ] = CCSprite::create( "image/object/wallside_static.png" );
			mSideSprite[ 1 ] = CCSprite::create( "image/object/wallside_static.png" );
		}
	
		mSideDistance.setPoint( 0, mSize.height / 2 - mSideSprite[ 0 ]->getContentSize().height / 2 );
		mSideDistance = mSideDistance.rotateByAngle( ccp( 0, 0 ), -mAngle );

		mSideSprite[ 0 ]->setPosition( mSprite->getPosition() + mSideDistance );
		mSideSprite[ 0 ]->setRotation( mAngle * 180 / M_PI );
		mSideSprite[ 1 ]->setPosition( mSprite->getPosition() - mSideDistance );
		mSideSprite[ 1 ]->setRotation( mAngle * 180 / M_PI );
	}else{
		mSideSprite[ 0 ] = NULL;
		mSideSprite[ 1 ] = NULL;
	}

	
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
}

void StaticWall::setParent( cocos2d::CCLayer* layer, cocos2d::CCLayer* underLayer ){
	int z = ( mColor == COLOR_BLUE || mColor == COLOR_RED || mColor == COLOR_GREEN ) ? -1 : 0;

	layer->addChild( mSprite, z );

	if ( mSideSprite[ 0 ] != NULL ) layer->addChild( mSideSprite[ 0 ] );
	if ( mSideSprite[ 1 ] != NULL ) layer->addChild( mSideSprite[ 1 ] );
}

void StaticWall::update( TouchManager* tm, StageLayer* sl ){
	mSprite->setPositionX( mBody->GetPosition().x * GameMain::mPtmRatio );
	mSprite->setPositionY( mBody->GetPosition().y * GameMain::mPtmRatio );
	
	for ( int i = 0; i < 2; i++ ){
		if ( mSideSprite[ i ] != NULL ){
			CCPoint dis = ( i == 0 ) ? mSideDistance : -mSideDistance;
			mSideSprite[ i ]->setPosition( ccp( mBody->GetPosition().x * GameMain::mPtmRatio, mBody->GetPosition().y * GameMain::mPtmRatio ) + dis );
			mSideSprite[ i ]->setRotation( -mBody->GetAngle() * 180.0f / M_PI );
		}
	}

	b2Vec2 dest;
	dest.x = ( mPosition.x - sl->screenPosition().x ) / GameMain::mPtmRatio;
	dest.y = ( mPosition.y + sl->screenPosition().y ) / GameMain::mPtmRatio;
	mBody->SetTransform( dest, mBody->GetAngle() );

	mVelocity.Set( ( mCount % 2 == 0 ) ? 0.001f : -0.001f, 0.0f );
	mBody->SetLinearVelocity( mVelocity );
	++mCount;
}

void StaticWall::setColor( Color color ){
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

void StaticWall::resetColor(){
	setColor( mColor );
}