#include "PhysicsSpriteTest.h"
#include "TouchManager.h"
#include "StageLayer.h"
#include "Collision.h"

using namespace cocos2d;
using namespace GameLib;

const float PhysicsSpriteTest::mPtmRatio = 32.0f;

PhysicsSpriteTest::PhysicsSpriteTest( cocos2d::CCLayer* layer, b2World* world ){

	mSprite = CCSprite::create( "WallSide.png" );
	mSprite->setPosition( ccp( 0, 0 ) );
	mSprite->setScale( 2.0f );
	layer->addChild( mSprite );

	
	mWidth = mSprite->getContentSize().width * 2.0f;
	mHeight = mSprite->getContentSize().height * 2.0f;
	mAngle = 0.9f;


	b2BodyDef bodyDef;
	bodyDef.type = b2_dynamicBody;
	bodyDef.userData = mSprite;
	bodyDef.position.Set( 300 / mPtmRatio, 300 / mPtmRatio );

	mBody = world->CreateBody( &bodyDef );
	
	b2PolygonShape poly;
	float width = mWidth * 0.5f;
	float height = mHeight * 0.5f;
	poly.SetAsBox( width / mPtmRatio, height / mPtmRatio );

	b2FixtureDef fixtureDef;
	fixtureDef.shape = &poly;
	fixtureDef.density = 0.0f;
	fixtureDef.filter.groupIndex = -2;

	mBody->CreateFixture( &fixtureDef );
	mBody->SetTransform( b2Vec2( 300 / mPtmRatio, 300 / mPtmRatio ), -mAngle );

	mPos.set( 100, 100 );

	mTouchIndices[ 0 ] = -1;
	mTouchIndices[ 1 ] = -1;
}

void PhysicsSpriteTest::update( TouchManager* touchManager, const StageLayer& stageLayer ){

	mPos.set( mBody->GetPosition().x * mPtmRatio, mBody->GetPosition().y * mPtmRatio );
	mSprite->setPositionX( mBody->GetPosition().x * mPtmRatio );
	mSprite->setPositionY( mBody->GetPosition().y * mPtmRatio );
	mSprite->setRotation( -mBody->GetAngle() * 180.0f / M_PI );
	
	for ( int i = 0; i < touchManager->getTouchNum(); i++ ){
		if ( touchManager->getTouchObject( i )->touchBeganFlag ){
			CCPoint touchPos( touchManager->getTouchObject( i )->touch_past_x, touchManager->getTouchObject( i )->touch_past_y );
			if ( Collision::testIntersectPositionAndSquare( touchPos, mSprite->getPosition(), mWidth, mHeight, -mAngle ) ){
				for ( int j = 0; j < 2; j++ ){
					if ( mTouchIndices[ j ] == -1 ){
						mTouchIndices[ j ] = i;
						break;
					}
				}
			}
		}
	}

	int numHit = 0;
	for ( int i = 0; i < 2; i++ ){
		if ( mTouchIndices[ i ] != -1 ){
			if ( !touchManager->getTouchObject( mTouchIndices[ i ] )->touchFlag ){
				mTouchIndices[ i ] = -1;
			}else{
				++numHit;
			}
		}
	}

	if ( numHit == 1 ){
		for ( int i = 0; i < 2; i++ ){
			int index = mTouchIndices[ i ];
			if ( index != -1 && touchManager->getTouchObject( index )->touchFlag ){
				CCPoint touchPos( touchManager->getTouchObject( index )->touch_past_x, touchManager->getTouchObject( index )->touch_past_y );
				if ( Collision::testIntersectPositionAndSquare( touchPos, mSprite->getPosition(), mWidth, mHeight, -mAngle ) ){
					float moveX = touchManager->getTouchObject( index )->touch_moved_x;
					float moveY = touchManager->getTouchObject( index )->touch_moved_y;

					mPos += Vector2( moveX, moveY );

					break;
				}
			}
		}
	}
	else if ( numHit == 2 ){
		bool hit = false;
		for ( int i = 0; i < 2; i++ ){
			int index = mTouchIndices[ i ];
			if ( index != -1 && touchManager->getTouchObject( index )->touchFlag ){
				CCPoint touchPos( touchManager->getTouchObject( index )->touch_past_x, touchManager->getTouchObject( index )->touch_past_y );
				if ( Collision::testIntersectPositionAndSquare( touchPos, mSprite->getPosition(), mWidth, mHeight, -mAngle ) ){
					hit = true;
				}
			}
		}

		if ( hit ){
			for ( int i = 0; i < 2; i++ ){
				CCPoint a( touchManager->getTouchObject( mTouchIndices[ i ] )->touch_x, touchManager->getTouchObject( mTouchIndices[ i ] )->touch_y );
				CCPoint b( touchManager->getTouchObject( mTouchIndices[ 1 - i ] )->touch_x, touchManager->getTouchObject( mTouchIndices[ 1 - i ] )->touch_y );
				CCPoint c( touchManager->getTouchObject( mTouchIndices[ 1 - i ] )->touch_past_x, touchManager->getTouchObject( mTouchIndices[ 1 - i ] )->touch_past_y );
		
				if ( ccpDistance( a, b ) > 60.0f ){
					float r = ( b - a ).getAngle( ( c - a ) );
					mAngle += r;
			
					CCPoint d( mPos.x, mPos.y ); // center
					CCPoint ad( d - a );
					ad = ad.rotateByAngle( ccp( 0, 0 ), -r );

					mPos.set( a.x + ad.x, a.y + ad.y );
				}
			}
		}
	}
	
	mBody->SetTransform( b2Vec2( mPos.x / mPtmRatio, mPos.y / mPtmRatio ), -mAngle );
	mBody->SetLinearVelocity( b2Vec2( 0.001f, 0 ) );
	//mSprite->setPositionX( mPos.x );
	//mSprite->setPositionY( mPos.y );
}