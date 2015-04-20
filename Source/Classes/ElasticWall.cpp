#include "ElasticWall.h"
#include "ScreenConverter.h"
#include "Collision.h"
#include "GameMainScene.h"

USING_NS_CC;
using namespace GameLib;

ElasticWall::ElasticWall( b2World* world, cocos2d::CCPoint pos, float scale, float angle, Color color ){
	mPosition = pos;
	mPosition.y = ScreenConverter::convertY( pos.y );

	mDefaultPosition = mPosition;
	mColor = color;
	mAngle = angle;
	mMoveDirection = ccp( 0, -1 ).rotateByAngle( ccp( 0, 0 ), -angle );
	
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

	mSprite->setPosition( mPosition );
	mSprite->setScaleY( scale );
	mSprite->setRotation( mAngle * 180 / M_PI );
	
	if ( color == COLOR_BLACK_L ){
		mSideSprite[ 0 ] = CCSprite::create( "image/object/wallside_big.png" );
		mSideSprite[ 1 ] = CCSprite::create( "image/object/wallside_big.png" );
	}else{
		mSideSprite[ 0 ] = CCSprite::create( "image/object/wallside.png" );
		mSideSprite[ 1 ] = CCSprite::create( "image/object/wallside.png" );
	}
	mSideSize = mSideSprite[ 0 ]->getContentSize();
	
	mSidePos[ 0 ].y = mSize.height / 2 - mSideSize.height / 2;
	mSidePos[ 1 ].y = -mSize.height / 2 + mSideSize.height / 2;
	for( int i = 0; i < 2; i++ ){
		mSidePos[ i ] = mSidePos[ i ].rotateByAngle( ccp( 0, 0 ), -angle );
		mSidePos[ i ] = mSidePos[ i ] + mPosition;
		mDefaultSidePos[ i ] = mSidePos[ i ];
	}

	mSideSprite[ 0 ]->setPosition( mSidePos[ 0 ] );
	mSideSprite[ 1 ]->setPosition( mSidePos[ 1 ] );
	mSideSprite[ 0 ]->setRotation( mAngle * 180 / M_PI );
	mSideSprite[ 1 ]->setRotation( mAngle * 180 / M_PI );

	mMaxSideDistance = ( mSidePos[ 0 ] - mSidePos[ 1 ] ).getLength() / 2.0f;
	mMinSideDistance = 48.0f;
	mSideDifference = ( mSidePos[ 0 ] - mSidePos[ 1 ] ).getLength();

	mReturnMoveValue = 10.0f;


	// 本体のボックス
	b2BodyDef bodyDef;
	bodyDef.type = b2_staticBody;
	bodyDef.userData = mSprite;

	mBody = world->CreateBody( &bodyDef );
	
	mPolygonShape.SetAsBox( ( mSize.width * 0.5f ) / GameMain::mPtmRatio, ( mSize.height * 0.5f ) / GameMain::mPtmRatio );
	mFixtureDef.shape = &mPolygonShape;
	mFixtureDef.density = 0.0f;
	mFixtureDef.filter.groupIndex = groupIndex;

	mBody->CreateFixture( &mFixtureDef );
	mBody->SetTransform( b2Vec2( mPosition.x / GameMain::mPtmRatio, mPosition.y / GameMain::mPtmRatio ), -mAngle );


	// ツマミのボックス
	for ( int i = 0; i < 2; i++ ){
		b2BodyDef bodyDef;
		bodyDef.type = b2_staticBody;
		bodyDef.userData = mSprite;

		mSideBodies[ i ] = world->CreateBody( &bodyDef );
	
		b2PolygonShape poly;
		float width = mSideSize.width * 0.5f;
		float height = mSideSize.height * 0.5f;
		poly.SetAsBox( width / GameMain::mPtmRatio, height / GameMain::mPtmRatio );

		b2FixtureDef fixtureDef;
		fixtureDef.shape = &poly;
		fixtureDef.density = 0.0f;
		fixtureDef.filter.groupIndex = FixtureGroup::GR_NONE;

		mSideBodies[ i ]->CreateFixture( &fixtureDef );
		mSideBodies[ i ]->SetTransform( b2Vec2( mSidePos[ i ].x / GameMain::mPtmRatio, mSidePos[ i ].y / GameMain::mPtmRatio ), -mAngle );
	}

	mCount = 0;
	mMoving = false;

	mCollisionSize = mSize;
	mCollisionSize.width += 48;
}

void ElasticWall::setParent( cocos2d::CCLayer* layer, cocos2d::CCLayer* underLayer ){
	layer->addChild( mSprite );
	layer->addChild( mSideSprite[ 0 ] );
	layer->addChild( mSideSprite[ 1 ] );
}

void ElasticWall::update( TouchManager* tm, StageLayer* sl ){
	mSprite->setPositionX( mBody->GetPosition().x * GameMain::mPtmRatio );
	mSprite->setPositionY( mBody->GetPosition().y * GameMain::mPtmRatio );
	mSprite->setScaleY( mSize.height / mSprite->getContentSize().height );

	for ( int i = 0; i < 2; i++ ){
		mSideSprite[ i ]->setPositionX( mSideBodies[ i ]->GetPosition().x * GameMain::mPtmRatio );
		mSideSprite[ i ]->setPositionY( mSideBodies[ i ]->GetPosition().y * GameMain::mPtmRatio );
	}
	
	bool hit = false;
	for( int i = 0; i < tm->getTouchNum(); i++ ){
		if( tm->getTouchObject( i )->touchBeganFlag || ( mMoving && tm->getTouchObject( i )->touchFlag ) ){
			CCPoint touchPos( tm->getTouchObject( i )->touch_past_x, tm->getTouchObject( i )->touch_past_y );
			
			if ( Collision::testIntersectPositionAndSquare( touchPos, mSprite->getPosition(), mCollisionSize.width, mCollisionSize.height, -mAngle ) ||
				 Collision::testIntersectPositionAndSquare( touchPos, mSideSprite[ 0 ]->getPosition(), mSideSize.width, mSideSize.height, -mAngle ) ||
				 Collision::testIntersectPositionAndSquare( touchPos, mSideSprite[ 1 ]->getPosition(), mSideSize.width, mSideSize.height, -mAngle ) ){
					CCPoint move( tm->getTouchObject( i )->touch_moved_x, tm->getTouchObject( i )->touch_moved_y );
					move = mMoveDirection * move.dot( mMoveDirection );

					// Side[ 0 ]方向へ移動
					float dot = move.dot( mDefaultSidePos[ 0 ] - mDefaultSidePos[ 1 ] );
					if ( dot > 0 && ccpDistance( mSidePos[ 0 ], mDefaultSidePos[ 0 ] ) < 1.0f ){
						mSidePos[ 1 ] = mSidePos[ 1 ] + move;

						if ( ( mDefaultSidePos[ 1 ] - mSidePos[ 1 ] ).getLength() > mSideDifference - mMinSideDistance ){
							mSidePos[ 1 ] = mDefaultSidePos[ 1 ] + ( mDefaultSidePos[ 0 ] - mDefaultSidePos[ 1 ] ).normalize() * ( mSideDifference - mMinSideDistance );
						}
					}
					// Side[ 1 ]方向へ移動
					else if ( dot < 0 && ccpDistance( mSidePos[ 1 ], mDefaultSidePos[ 1 ] ) < 1.0f ){
						mSidePos[ 0 ] = mSidePos[ 0 ] + move;

						if ( ( mDefaultSidePos[ 0 ] - mSidePos[ 0 ] ).getLength() > mSideDifference - mMinSideDistance ){
							mSidePos[ 0 ] = mDefaultSidePos[ 0 ] + ( mDefaultSidePos[ 1 ] - mDefaultSidePos[ 0 ] ).normalize() * ( mSideDifference - mMinSideDistance );
						}
					}
			
					mMoving = true;
					hit = true;
					break;
			}

			//for ( int j = 0; j < 2; j++ ){
			//	if ( Collision::testIntersectPositionAndSquare( touchPos, mSideSprite[ j ]->getPosition(), mSideSize.width, mSideSize.height, -mAngle ) ){
			//		if ( ccpDistance( mSidePos[ 1 - j ], mDefaultSidePos[ 1 - j ] ) < 1.0f ){
			//			CCPoint move( tm->getTouchObject( i )->touch_moved_x, tm->getTouchObject( i )->touch_moved_y );
			//			move = mMoveDirection * move.dot( mMoveDirection );

			//			mSidePos[ j ] = mSidePos[ j ] + move;
			//
			//			// 移動範囲を制限
			//			CCPoint defToNow = ( mSidePos[ j ] - mDefaultSidePos[ 1 - j ] );
			//			float dot = move.dot( defToNow );
			//			float distance = defToNow.getLength();
			//			if ( dot > 0 && distance > mMaxSideDistance ){
			//				mSidePos[ j ] = mDefaultPosition + defToNow.normalize() * mMaxSideDistance;
			//			}else if ( dot < 0 && distance < mMinSideDistance ){
			//				mSidePos[ j ] = mDefaultSidePos[ 1 - j ] + defToNow.normalize() * mMinSideDistance;
			//			}

			//			hit = true;
			//			break;
			//		}
			//	}
			//}
		}
	}
	
	if ( hit ){
		sl->stopScroll();
	}
	// タッチされていなければ戻す
	else{
		for ( int i = 0; i < 2; i++ ){
			CCPoint move = ( mDefaultSidePos[ i ] - mSidePos[ i ] ).normalize() * mReturnMoveValue;
			mSidePos[ i ] = mSidePos[ i ] + move;

			if ( ccpDistance( mSidePos[ i ], mDefaultSidePos[ i ] ) < mReturnMoveValue * 1.2f ){
				mSidePos[ i ] = mDefaultSidePos[ i ];
			}
		}
		mMoving = false;
	}

	mSize.height = ccpDistance( mSidePos[ 0 ], mSidePos[ 1 ] );
	mPosition = mSidePos[ 0 ] + ( ( mSidePos[ 1 ] - mSidePos[ 0 ] ) / 2 );

	// 本体のボックスを移動
	b2Vec2 dest;
	dest.x = ( mPosition.x - sl->screenPosition().x ) / GameMain::mPtmRatio;
	dest.y = ( mPosition.y + sl->screenPosition().y ) / GameMain::mPtmRatio;
	mBody->SetTransform( dest, mBody->GetAngle() );
	
	// 本体の大きさを再定義
	mBody->DestroyFixture( mBody->GetFixtureList() );
	mPolygonShape.SetAsBox( ( mSize.width * 0.5f ) / GameMain::mPtmRatio, ( mSize.height * 0.5f ) / GameMain::mPtmRatio );
	mBody->CreateFixture( &mFixtureDef );

	// ツマミのボックスを移動
	for ( int i = 0; i < 2; i++ ){
		dest.x = ( mSidePos[ i ].x - sl->screenPosition().x ) / GameMain::mPtmRatio;
		dest.y = ( mSidePos[ i ].y + sl->screenPosition().y ) / GameMain::mPtmRatio;
		mSideBodies[ i ]->SetTransform( dest, mSideBodies[ i ]->GetAngle() );
	}

	
	mVelocity.Set( ( mCount % 2 == 0 ) ? 0.001f : -0.001f, 0.0f );
	mBody->SetLinearVelocity( mVelocity );
	mSideBodies[ 0 ]->SetLinearVelocity( mVelocity );
	mSideBodies[ 1 ]->SetLinearVelocity( mVelocity );
	++mCount;
}

void ElasticWall::setColor( Color color ){
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

void ElasticWall::resetColor(){
	setColor( mColor );
}