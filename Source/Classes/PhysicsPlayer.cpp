#include "PhysicsPlayer.h"
#include "Collision.h"
#include "GameMainScene.h"
#include <cmath>

#include "SimpleAudioEngine.h"
using namespace CocosDenshion;

using namespace GameLib;

const float PhysicsPlayer::GreenWidth = 256.0f;
const float PhysicsPlayer::GreenHeight = 256.0f;
const float PhysicsPlayer::RedWidth = 120.0f;
const float PhysicsPlayer::RedHeight = 270.0f;
const float PhysicsPlayer::BlueWidth = 120.0f;
const float PhysicsPlayer::BlueHeight = 270.0f;

const float PhysicsPlayer::SplitDistance = 300.0f;
const float PhysicsPlayer::UnitDistance = 80.0f;

PhysicsPlayer::PhysicsPlayer(CCLayer* layer,b2World &world){
	PhysicsPlayer::init(layer,world);

	mRedClear = false;
	mBlueClear = false;
}

PhysicsPlayer::~PhysicsPlayer(){
	delete mGreenPlayer;
	delete mRedPlayer;
	delete mBluePlayer;
}

void PhysicsPlayer::init(CCLayer* layer,b2World &world){
	CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
	
	mGreenPlayer = new PlayerData(layer,world,PlayerData::PLAYER_GREEN,GreenWidth,GreenHeight);
	mRedPlayer = new PlayerData(layer,world,PlayerData::PLAYER_RED,RedWidth,RedHeight );
	mBluePlayer = new PlayerData(layer,world,PlayerData::PLAYER_BLUE,BlueWidth,BlueHeight);

	mPlayerSplited = true;
	mCanUniteFlag = false;
	mRedDied = false;
	mBlueDied = false;
	mRedClear = false;
	mBlueClear = false;

	mDeadCountRed = 0;
	mDeadCountBlue = 0;
	mDeadTime = 5;

	//�f�o�b�O�\��player
	/*
	CCString* str = CCString::createWithFormat(
		" Red1:%d, Red2:%d, Blue:%d, Blue %d",
		mRedPlayer->getFirstTouchedIndex(),mRedPlayer->getSecondTouchedIndex(),mBluePlayer->getFirstTouchedIndex(),mBluePlayer->getSecondTouchedIndex());
	mDdebugLabel = CCLabelTTF::create(str->getCString(), "", 32.0f);
	mDdebugLabel->setPosition(ccp(visibleSize.width * 0.5f, visibleSize.height - 20));
	layer->addChild(mDdebugLabel);

	mTouchLabel = CCLabelTTF::create("aaa", "", 32.0f);
	mTouchLabel->setPosition(ccp(visibleSize.width * 0.5f, visibleSize.height - 80));
	layer->addChild(mTouchLabel);
	debugCount = 0;
	*/
}

void PhysicsPlayer::setStartPos(CCPoint startPositionRed,float startAngleRed, CCPoint startPositionBlue,float startAngleBlue){
	mRedPlayer->setPos(startPositionRed,startAngleRed);
	mBluePlayer->setPos(startPositionBlue,startAngleBlue);
}

void PhysicsPlayer::update(float delta, TouchManager* touchManager,const StageLayer& stageLayer,const ObjectLayer& objectLayer){
	// �X�v���C�g�̍��W���{�b�N�X�ɍ��킹��
	mRedPlayer->mSprite->setPosition( ccp( mRedPlayer->boxBody->GetPosition().x * GameMain::mPtmRatio, mRedPlayer->boxBody->GetPosition().y * GameMain::mPtmRatio ) );
	mRedPlayer->mSprite->setRotation( -mRedPlayer->boxBody->GetAngle() * 180.0f / M_PI );
	mBluePlayer->mSprite->setPosition( ccp( mBluePlayer->boxBody->GetPosition().x * GameMain::mPtmRatio, mBluePlayer->boxBody->GetPosition().y * GameMain::mPtmRatio ) );
	mBluePlayer->mSprite->setRotation( -mBluePlayer->boxBody->GetAngle() * 180.0f / M_PI );
	mGreenPlayer->mSprite->setPosition( ccp( mGreenPlayer->boxBody->GetPosition().x * GameMain::mPtmRatio, mGreenPlayer->boxBody->GetPosition().y * GameMain::mPtmRatio ) );
	mGreenPlayer->mSprite->setRotation( -mGreenPlayer->boxBody->GetAngle() * 180.0f / M_PI );
	
	//���̒�
	if(!mPlayerSplited){
		//�^�b�`�̓o�^����
		for(int i=0;i<touchManager->getTouchNum();i++){
			//�^�b�`�ɋ󂫂�����
			if(mGreenPlayer->isEmptyTouch()){
				//�^�b�`�����u��
				if(touchManager->getTouchObject(i)->touchBeganFlag){
					//�������Ă��邩
					if(mGreenPlayer->isTouched(touchManager->getTouchObject(i)->touch_began_x,touchManager->getTouchObject(i)->touch_began_y)){
						//���̃^�b�`�ԍ���o�^����
						mGreenPlayer->setTouchIndex(i);
						//2�ڂ̃^�b�`���o�^���ꂽ�u��
						if(mGreenPlayer->getSecondTouchedIndex() != -1){
							mGreenPlayer->setFirstBeginPos(touchManager->getTouchObject(mGreenPlayer->getFirstTouchedIndex())->touch_x,touchManager->getTouchObject(mGreenPlayer->getFirstTouchedIndex())->touch_y);
							mGreenPlayer->setBeforeVector(touchManager->getTouchObject(mGreenPlayer->getSecondTouchedIndex())->touch_x,touchManager->getTouchObject(mGreenPlayer->getSecondTouchedIndex())->touch_y);
							break;
						}
					}
				}
			}
			//�͂Ȃ����u��
			if(touchManager->getTouchObject(i)->touchEndedFlag){
				//�^�b�`�ԍ������Z�b�g����
				mGreenPlayer->resetTouchIndex(i);
			}
		}
		//1�ڂ̃^�b�`(�ړ�����)
		if(mGreenPlayer->getFirstTouchedIndex() != -1){
			int touchIndex = mGreenPlayer->getFirstTouchedIndex();
			b2Vec2 vec( touchManager->getTouchObject(touchIndex)->touch_moved_x*2,touchManager->getTouchObject(touchIndex)->touch_moved_y*2);
			if(vec.Length() ==0){
				vec.Set( 0.0000001f,0.0f);
			}
			mGreenPlayer->boxBody->SetLinearVelocity(vec);
		}else{
			b2Vec2 vec( 0.0000001f,0.0f);
			mGreenPlayer->boxBody->SetLinearVelocity(vec);
			
		}
		//2�ڂ̃^�b�`(��]����)
		if(mGreenPlayer->getSecondTouchedIndex() != -1){
			int touchIndex = mGreenPlayer->getSecondTouchedIndex();
			float deltaAngle = mGreenPlayer->getAngleFromBeginVector( touchManager->getTouchObject( touchIndex )->touch_x, touchManager->getTouchObject( touchIndex )->touch_y );				
			mGreenPlayer->boxBody->SetAngularVelocity(deltaAngle);
		}else{
			mGreenPlayer->boxBody->SetAngularVelocity(0.0f);
		}
		//���􂵂���
		if(mGreenPlayer->getFirstTouchedIndex() != -1 && mGreenPlayer->getSecondTouchedIndex() != -1){
			float dir = ccpDistance( ccp(touchManager->getTouchObject(mGreenPlayer->getFirstTouchedIndex())->touch_x,touchManager->getTouchObject(mGreenPlayer->getFirstTouchedIndex())->touch_y),
				ccp(touchManager->getTouchObject(mGreenPlayer->getSecondTouchedIndex())->touch_x,touchManager->getTouchObject(mGreenPlayer->getSecondTouchedIndex())->touch_y));
			if(dir > SplitDistance){
				mPlayerSplited = true;
				b2Vec2 splitVec = b2Vec2(touchManager->getTouchObject(mGreenPlayer->getFirstTouchedIndex())->touch_x - touchManager->getTouchObject(mGreenPlayer->getSecondTouchedIndex())->touch_x,
					touchManager->getTouchObject(mGreenPlayer->getFirstTouchedIndex())->touch_y - touchManager->getTouchObject(mGreenPlayer->getSecondTouchedIndex())->touch_y);

				mRedPlayer->setTouchIndex(mGreenPlayer->getFirstTouchedIndex());
				mBluePlayer->setTouchIndex(mGreenPlayer->getSecondTouchedIndex());

				mRedPlayer->setPos(mGreenPlayer->getPos(),mGreenPlayer->getAngle());
				mBluePlayer->setPos(mGreenPlayer->getPos(),mGreenPlayer->getAngle());;
				
				mRedPlayer->boxBody->SetLinearVelocity(splitVec);
				mBluePlayer->boxBody->SetLinearVelocity(-splitVec);


				mGreenPlayer->boxBody->SetActive(false);
				mGreenPlayer->mSprite->setVisible(false);
				mRedPlayer->boxBody->SetActive(true);
				mRedPlayer->mSprite->setVisible(true);
				mBluePlayer->boxBody->SetActive(true);	
				mBluePlayer->mSprite->setVisible(true);
				mGreenPlayer->resetTouchIndexAll();
			}
		}
	}else{
		//�^�b�`�̓o�^����
		for(int i=0;i<touchManager->getTouchNum();i++){
			//�Ԃ𒲂ׂ�
			//�^�b�`�ɋ󂫂�����
			if(mRedPlayer->isEmptyTouch()){
				if(mRedPlayer->getFirstTouchedIndex() != -1){
					//�^�b�`�����u��
					if(touchManager->getTouchObject(i)->touchBeganFlag){
						//�������Ă��邩
						if(Collision::testIntersectPositionAndSquare( ccp( touchManager->getTouchObject(i)->touch_x,touchManager->getTouchObject(i)->touch_y ),
							mRedPlayer->getPos(),RedWidth,RedHeight,mRedPlayer->getAngle() ) ){
							//���̃^�b�`�ԍ���o�^����
							mRedPlayer->setTouchIndex(i);
							mDeadCountRed = 0;
							//2�ڂ̃^�b�`���o�^���ꂽ�u��
							if(mRedPlayer->getSecondTouchedIndex() != -1){
								mRedPlayer->setFirstBeginPos(touchManager->getTouchObject(mRedPlayer->getFirstTouchedIndex())->touch_x,touchManager->getTouchObject(mRedPlayer->getFirstTouchedIndex())->touch_y);
								mRedPlayer->setBeforeVector(touchManager->getTouchObject(mRedPlayer->getSecondTouchedIndex())->touch_x,touchManager->getTouchObject(mRedPlayer->getSecondTouchedIndex())->touch_y);
							}
							continue;
						}
					}
				}else{
					//�^�b�`���Ă���Ƃ�
					if(touchManager->getTouchObject(i)->touchFlag){
						//�������Ă��邩
						if(Collision::testIntersectPositionAndSquare( ccp( touchManager->getTouchObject(i)->touch_x,touchManager->getTouchObject(i)->touch_y ),
							mRedPlayer->getPos(),RedWidth,RedHeight,mRedPlayer->getAngle() ) ){
							//���̃^�b�`�ԍ���o�^����
							mRedPlayer->setTouchIndex(i);
							//mDeadCountRed = 0;
							//2�ڂ̃^�b�`���o�^���ꂽ�u��
							if(mRedPlayer->getSecondTouchedIndex() != -1){
								mRedPlayer->setFirstBeginPos(touchManager->getTouchObject(mRedPlayer->getFirstTouchedIndex())->touch_x,touchManager->getTouchObject(mRedPlayer->getFirstTouchedIndex())->touch_y);
								mRedPlayer->setBeforeVector(touchManager->getTouchObject(mRedPlayer->getSecondTouchedIndex())->touch_x,touchManager->getTouchObject(mRedPlayer->getSecondTouchedIndex())->touch_y);
							}
							continue;
						}
					}
				}
			}
			//�𒲂ׂ�
			//�^�b�`�ɋ󂫂�����
			if(mBluePlayer->isEmptyTouch()){
				if(mBluePlayer->getFirstTouchedIndex() != -1){
					//�^�b�`�����u��
					if(touchManager->getTouchObject(i)->touchBeganFlag){
						//�������Ă��邩
						if(Collision::testIntersectPositionAndSquare( ccp( touchManager->getTouchObject(i)->touch_x,touchManager->getTouchObject(i)->touch_y ),
							mBluePlayer->getPos(),BlueWidth,BlueHeight,mBluePlayer->getAngle() ) ){
							//���̃^�b�`�ԍ���o�^����
							mBluePlayer->setTouchIndex(i);
							mDeadCountBlue = 0;
							//2�ڂ̃^�b�`���o�^���ꂽ�u��
							if(mBluePlayer->getSecondTouchedIndex() != -1){
								mBluePlayer->setFirstBeginPos(touchManager->getTouchObject(mBluePlayer->getFirstTouchedIndex())->touch_x,touchManager->getTouchObject(mBluePlayer->getFirstTouchedIndex())->touch_y);
								mBluePlayer->setBeforeVector(touchManager->getTouchObject(mBluePlayer->getSecondTouchedIndex())->touch_x,touchManager->getTouchObject(mBluePlayer->getSecondTouchedIndex())->touch_y);
							}
							continue;
						}
					}
				}else{
					//�^�b�`���Ă���Ƃ�
					if(touchManager->getTouchObject(i)->touchFlag){
						//�������Ă��邩
						if(Collision::testIntersectPositionAndSquare( ccp( touchManager->getTouchObject(i)->touch_x,touchManager->getTouchObject(i)->touch_y ),
							mBluePlayer->getPos(),BlueWidth,BlueHeight,mBluePlayer->getAngle() ) ){
							//���̃^�b�`�ԍ���o�^����
							mBluePlayer->setTouchIndex(i);
							//mDeadCountBlue = 0;
							//2�ڂ̃^�b�`���o�^���ꂽ�u��
							if(mBluePlayer->getSecondTouchedIndex() != -1){
								mBluePlayer->setFirstBeginPos(touchManager->getTouchObject(mBluePlayer->getFirstTouchedIndex())->touch_x,touchManager->getTouchObject(mBluePlayer->getFirstTouchedIndex())->touch_y);
								mBluePlayer->setBeforeVector(touchManager->getTouchObject(mBluePlayer->getSecondTouchedIndex())->touch_x,touchManager->getTouchObject(mBluePlayer->getSecondTouchedIndex())->touch_y);
							}
							continue;
						}
					}
				}
			}
			// ����Ă���
			if( !touchManager->getTouchObject(i)->touchFlag ){
				//�^�b�`�ԍ������Z�b�g����
				mRedPlayer->resetTouchIndex(i);
				mBluePlayer->resetTouchIndex(i);
			}
		}

		// ���ɓ������Ă�����^�b�`�����Z�b�g
		for ( int i = 0; i < objectLayer.numObject(); i++ ){
			for ( int j = 0; j < 2; j++ ){
				int index = ( ( j == 0 ) ? mRedPlayer->getFirstTouchedIndex() : mRedPlayer->getSecondTouchedIndex() );
				if ( index != -1 ){
					CCPoint touchPos( touchManager->getTouchObject( index )->touch_past_x, touchManager->getTouchObject( index )->touch_past_y );
					if ( objectLayer.get( i )->isIntersect( touchPos, stageLayer ) ){
						mDeadCountRed = mDeadTime;
						mRedPlayer->resetTouchIndex( index );
					}
				}
			}
			for ( int j = 0; j < 2; j++ ){
				int index = ( ( j == 0 ) ? mBluePlayer->getFirstTouchedIndex() : mBluePlayer->getSecondTouchedIndex() );
				if ( index != -1 ){
					CCPoint touchPos( touchManager->getTouchObject( index )->touch_past_x, touchManager->getTouchObject( index )->touch_past_y );
					if ( objectLayer.get( i )->isIntersect( touchPos, stageLayer ) ){
						mDeadCountBlue = mDeadTime;
						mBluePlayer->resetTouchIndex( index );
					}
				}
			}
		}

		if ( !mRedDied ){
			//�Ԃ̈ړ�
			if(mRedPlayer->getFirstTouchedIndex() != -1 && mRedPlayer->getSecondTouchedIndex() == -1){
				// Update 6/24
				CCPoint touchPos( touchManager->getTouchObject( mRedPlayer->getFirstTouchedIndex() )->touch_past_x, touchManager->getTouchObject( mRedPlayer->getFirstTouchedIndex() )->touch_past_y );
				if(Collision::testIntersectPositionAndSquare( touchPos, mRedPlayer->getPos(),RedWidth,RedHeight,mRedPlayer->getAngle() ) ){
					int touchIndex = mRedPlayer->getFirstTouchedIndex();
					CCPoint vec( touchManager->getTouchObject(touchIndex)->touch_moved_x,touchManager->getTouchObject(touchIndex)->touch_moved_y);
					if ( vec.getLengthSq() < 60 * 60 ){
						mRedPlayer->addPos( vec );
					}
				}
			}		
			//�Ԃ̉�]
			if(mRedPlayer->getSecondTouchedIndex() != -1){
				//int touchIndex = mRedPlayer->getSecondTouchedIndex();
				//float deltaAngle = mRedPlayer->getAngleFromBeginVector( touchManager->getTouchObject( touchIndex )->touch_x, touchManager->getTouchObject( touchIndex )->touch_y );
				//mRedPlayer->boxBody->SetAngularVelocity(deltaAngle);
			
				// Update 6/24
				// 2�_�̂ǂ��炩�Ń^�b�`����Ă��邩�`�F�b�N
				bool hit = false;
				for ( int i = 0; i < 2; i++ ){
					int index = ( ( i == 0 ) ? mRedPlayer->getFirstTouchedIndex() : mRedPlayer->getSecondTouchedIndex() );
					if ( index != -1 && touchManager->getTouchObject( index )->touchFlag ){
						CCPoint touchPos( touchManager->getTouchObject( index )->touch_past_x, touchManager->getTouchObject( index )->touch_past_y );
						if ( Collision::testIntersectPositionAndSquare( touchPos, mRedPlayer->getPos(), RedWidth, RedHeight, mRedPlayer->getAngle() ) ){
							hit = true;
						}
					}
				}

				if ( hit ){
					CCPoint pos = mRedPlayer->getPos();
					float angle = -mRedPlayer->getAngle();
					for ( int i = 0; i < 2; i++ ){
						int indexA = ( ( i == 0 ) ? mRedPlayer->getFirstTouchedIndex() : mRedPlayer->getSecondTouchedIndex() );
						int indexB = ( ( i == 0 ) ? mRedPlayer->getSecondTouchedIndex() : mRedPlayer->getFirstTouchedIndex() );
						CCPoint a( touchManager->getTouchObject( indexA )->touch_x, touchManager->getTouchObject( indexA )->touch_y );
						CCPoint b( touchManager->getTouchObject( indexB )->touch_x, touchManager->getTouchObject( indexB )->touch_y );
						CCPoint c( touchManager->getTouchObject( indexB )->touch_past_x, touchManager->getTouchObject( indexB )->touch_past_y );
		
						if ( ccpDistance( a, b ) > 30.0f ){
							float r = ( b - a ).getAngle( ( c - a ) );
							angle += r;
			
							CCPoint d( pos ); // ���S�̍��W
							CCPoint ad( d - a );
							ad = ad.rotateByAngle( ccp( 0, 0 ), -r );

							pos = a + ad;
						}
					}

					mRedPlayer->setPos( pos, -angle );
				}
			}
		}else if ( !mRedClear ){
			mRedPlayer->rotation( 0.3f );
			mRedPlayer->mSprite->setScale( getMax( 0, mRedPlayer->mSprite->getScale() - 0.01f ) );
		}

		if ( !mBlueDied ){
			//�̈ړ�
			if(mBluePlayer->getFirstTouchedIndex() != -1 && mBluePlayer->getSecondTouchedIndex() == -1){
				// Update 6/24
				CCPoint touchPos( touchManager->getTouchObject( mBluePlayer->getFirstTouchedIndex() )->touch_past_x, touchManager->getTouchObject( mBluePlayer->getFirstTouchedIndex() )->touch_past_y );
				if(Collision::testIntersectPositionAndSquare( touchPos, mBluePlayer->getPos(),BlueWidth,BlueHeight,mBluePlayer->getAngle() ) ){
					int touchIndex = mBluePlayer->getFirstTouchedIndex();
					CCPoint vec( touchManager->getTouchObject(touchIndex)->touch_moved_x,touchManager->getTouchObject(touchIndex)->touch_moved_y);
					if ( vec.getLengthSq() < 60 * 60 ){
						mBluePlayer->addPos( vec );
					}
				}
			}
			//�̉�]
			if(mBluePlayer->getSecondTouchedIndex() != -1){
				//int touchIndex = mBluePlayer->getSecondTouchedIndex();
				//float deltaAngle = mBluePlayer->getAngleFromBeginVector( touchManager->getTouchObject( touchIndex )->touch_x, touchManager->getTouchObject( touchIndex )->touch_y );				
				//mBluePlayer->boxBody->SetAngularVelocity(deltaAngle);
			
				// Update 6/24
				// 2�_�̂ǂ��炩�Ń^�b�`����Ă��邩�`�F�b�N
				bool hit = false;
				for ( int i = 0; i < 2; i++ ){
					int index = ( ( i == 0 ) ? mBluePlayer->getFirstTouchedIndex() : mBluePlayer->getSecondTouchedIndex() );
					if ( index != -1 && touchManager->getTouchObject( index )->touchFlag ){
						CCPoint touchPos( touchManager->getTouchObject( index )->touch_past_x, touchManager->getTouchObject( index )->touch_past_y );
						if ( Collision::testIntersectPositionAndSquare( touchPos, mBluePlayer->getPos(), BlueWidth, BlueHeight, mBluePlayer->getAngle() ) ){
							hit = true;
						}
					}
				}

				if ( hit ){
					for ( int i = 0; i < 2; i++ ){
						int indexA = ( ( i == 0 ) ? mBluePlayer->getFirstTouchedIndex() : mBluePlayer->getSecondTouchedIndex() );
						int indexB = ( ( i == 0 ) ? mBluePlayer->getSecondTouchedIndex() : mBluePlayer->getFirstTouchedIndex() );
						CCPoint a( touchManager->getTouchObject( indexA )->touch_x, touchManager->getTouchObject( indexA )->touch_y );
						CCPoint b( touchManager->getTouchObject( indexB )->touch_x, touchManager->getTouchObject( indexB )->touch_y );
						CCPoint c( touchManager->getTouchObject( indexB )->touch_past_x, touchManager->getTouchObject( indexB )->touch_past_y );
		
						if ( ccpDistance( a, b ) > 30.0f ){
							float r = ( b - a ).getAngle( ( c - a ) );
							mBluePlayer->rotation( -r );
			
							CCPoint d( mBluePlayer->getPos().x, mBluePlayer->getPos().y ); // ���S�̍��W
							CCPoint ad( d - a );
							ad = ad.rotateByAngle( ccp( 0, 0 ), -r );

							mBluePlayer->setPos( a + ad, mBluePlayer->getAngle() );
						}
					}
				}
			}
		}else if ( !mBlueClear ){
			mBluePlayer->rotation( 0.3f );
			mBluePlayer->mSprite->setScale( getMax( 0, mBluePlayer->mSprite->getScale() - 0.01f ) );
		}

		//�����ɍ��W��␳
		b2Vec2 vec( 0.0000001f,0.0f);
		mRedPlayer->boxBody->SetLinearVelocity(vec);
		mBluePlayer->boxBody->SetLinearVelocity(vec);

		// ���S�m�F
		if (!mRedClear && !mRedDied){
			if ( mRedPlayer->getFirstTouchedIndex() == -1){
				if(mDeadCountRed < mDeadTime){
					mDeadCountRed++;
				}else{
					//���S��
					SimpleAudioEngine::sharedEngine()->playEffect("se/drop.wav");
					mRedDied = true;
				}
			}else{
				CCPoint touchPos( touchManager->getTouchObject( mRedPlayer->getFirstTouchedIndex() )->touch_past_x, touchManager->getTouchObject( mRedPlayer->getFirstTouchedIndex() )->touch_past_y );
				if (!Collision::testIntersectPositionAndSquare( touchPos, mRedPlayer->getPos(),RedWidth,RedHeight,mRedPlayer->getAngle() ) ){
					if(mDeadCountRed < mDeadTime){
						mDeadCountRed++;
					}else{
						//���S��
						SimpleAudioEngine::sharedEngine()->playEffect("se/drop.wav");
						mRedDied = true;
						mRedPlayer->resetTouchIndex(mRedPlayer->getFirstTouchedIndex());
					}
				}
			}
		}

		if ( !mBlueClear && !mBlueDied){
			if ( mBluePlayer->getFirstTouchedIndex() == -1 ){
				if(mDeadCountBlue < mDeadTime){
					mDeadCountBlue++;
				}else{
					//���S��
					SimpleAudioEngine::sharedEngine()->playEffect("se/drop.wav");
					mBlueDied = true;
				}
			}else {
				CCPoint touchPos( touchManager->getTouchObject( mBluePlayer->getFirstTouchedIndex() )->touch_past_x, touchManager->getTouchObject( mBluePlayer->getFirstTouchedIndex() )->touch_past_y );
				if (!Collision::testIntersectPositionAndSquare( touchPos, mBluePlayer->getPos(),BlueWidth,BlueHeight,mBluePlayer->getAngle() ) ){
					if(mDeadCountBlue < mDeadTime){
						mDeadCountBlue++;
					}else{
						//���S��
						SimpleAudioEngine::sharedEngine()->playEffect("se/drop.wav");
						mBlueDied = true;
						mBluePlayer->resetTouchIndex(mBluePlayer->getFirstTouchedIndex());
					}
				}
			}
		}

		////�ړ���ɉ摜����^�b�`�����ꂽ���`�F�b�N����
		////�Ԃ�2��
		//if(mRedPlayer->getSecondTouchedIndex() != -1){
		//	if(!Collision::testIntersectPositionAndSquare( ccp( touchManager->getTouchObject(mRedPlayer->getSecondTouchedIndex())->touch_x,touchManager->getTouchObject(mRedPlayer->getSecondTouchedIndex())->touch_y ),
		//		mRedPlayer->getPos(),RedWidth,RedHeight,mRedPlayer->getAngle() ) ){
		//			mRedPlayer->resetTouchIndex(mRedPlayer->getSecondTouchedIndex());
		//	}
		//}
		////�Ԃ�1��
		//if(mRedPlayer->getFirstTouchedIndex() != -1){
		//	if(!Collision::testIntersectPositionAndSquare( ccp( touchManager->getTouchObject(mRedPlayer->getFirstTouchedIndex())->touch_x,touchManager->getTouchObject(mRedPlayer->getFirstTouchedIndex())->touch_y ),
		//		mRedPlayer->getPos(),RedWidth,RedHeight,mRedPlayer->getAngle() ) ){
		//			mRedPlayer->resetTouchIndex(mRedPlayer->getFirstTouchedIndex());
		//	}
		//}
		////��2��
		//if(mBluePlayer->getSecondTouchedIndex() != -1){
		//	if(!Collision::testIntersectPositionAndSquare( ccp( touchManager->getTouchObject(mBluePlayer->getSecondTouchedIndex())->touch_x,touchManager->getTouchObject(mBluePlayer->getSecondTouchedIndex())->touch_y ),
		//		mBluePlayer->getPos(),BlueWidth,BlueHeight,mBluePlayer->getAngle() ) ){
		//			mBluePlayer->resetTouchIndex(mRedPlayer->getSecondTouchedIndex());
		//	}
		//}
		////��1��
		//if(mBluePlayer->getFirstTouchedIndex() != -1){
		//	if(!Collision::testIntersectPositionAndSquare( ccp( touchManager->getTouchObject(mBluePlayer->getFirstTouchedIndex())->touch_x,touchManager->getTouchObject(mBluePlayer->getFirstTouchedIndex())->touch_y ),
		//		mBluePlayer->getPos(),BlueWidth,BlueHeight,mBluePlayer->getAngle() ) ){
		//			mBluePlayer->resetTouchIndex(mBluePlayer->getFirstTouchedIndex());
		//	}
		//}

		if(mCanUniteFlag){
			if(mRedPlayer->getFirstTouchedIndex() != -1 || mBluePlayer->getFirstTouchedIndex() != -1){
				//���̂�����
				float dir = ccpDistance(mRedPlayer->getPos(),mBluePlayer->getPos());
				//2���d�Ȃ�ƍ��̂���
				if(dir < UnitDistance){
					mPlayerSplited = false;
					mGreenPlayer->setTouchIndex(mRedPlayer->getFirstTouchedIndex());
					mGreenPlayer->setTouchIndex(mBluePlayer->getFirstTouchedIndex());
					int green_x = (mRedPlayer->getPos().x + mBluePlayer->getPos().x) * 0.5f;
					int green_y = (mRedPlayer->getPos().y + mBluePlayer->getPos().y) * 0.5f;
					float angle = ( mRedPlayer->getAngle() + mBluePlayer->getAngle() ) * 0.5f;
					mGreenPlayer->setPos(ccp(green_x,green_y),angle);

					mGreenPlayer->boxBody->SetActive(true);
					mGreenPlayer->mSprite->setVisible(true);
					mRedPlayer->boxBody->SetActive(false);
					mRedPlayer->mSprite->setVisible(false);
					mBluePlayer->boxBody->SetActive(false);
					mBluePlayer->mSprite->setVisible(false);

					mRedPlayer->resetTouchIndexAll();
					mBluePlayer->resetTouchIndexAll();
				}
			}
		}

	}

	//�f�o�b�O�\��
	CCString* str = CCString::createWithFormat(
	" Red1:%d, Red2:%d, Blue:%d, Blue %d",
	mRedPlayer->getFirstTouchedIndex(),mRedPlayer->getSecondTouchedIndex(),mBluePlayer->getFirstTouchedIndex(),mBluePlayer->getSecondTouchedIndex());
	mDdebugLabel->setString(str->getCString());

	//�f�o�b�O�\��touch
	CCString* touchStr = CCString::createWithFormat(
		" touch0:%d,touch1:%d, touch2:%d,touch13:%d,touch4:%d",
		(int)touchManager->getTouchObject(0)->touchFlag,(int)touchManager->getTouchObject(1)->touchFlag,
		(int)touchManager->getTouchObject(2)->touchFlag,(int)touchManager->getTouchObject(3)->touchFlag,
		(int)touchManager->getTouchObject(4)->touchFlag);
	mTouchLabel->setString(touchStr->getCString());
	debugCount++;
}

bool PhysicsPlayer::ready( TouchManager* touchManager ){
	// �X�v���C�g�̍��W���{�b�N�X�ɍ��킹��
	mRedPlayer->mSprite->setPosition( ccp( mRedPlayer->boxBody->GetPosition().x * GameMain::mPtmRatio, mRedPlayer->boxBody->GetPosition().y * GameMain::mPtmRatio ) );
	mRedPlayer->mSprite->setRotation( -mRedPlayer->boxBody->GetAngle() * 180.0f / M_PI );
	mBluePlayer->mSprite->setPosition( ccp( mBluePlayer->boxBody->GetPosition().x * GameMain::mPtmRatio, mBluePlayer->boxBody->GetPosition().y * GameMain::mPtmRatio ) );
	mBluePlayer->mSprite->setRotation( -mBluePlayer->boxBody->GetAngle() * 180.0f / M_PI );
	mGreenPlayer->mSprite->setPosition( ccp( mGreenPlayer->boxBody->GetPosition().x * GameMain::mPtmRatio, mGreenPlayer->boxBody->GetPosition().y * GameMain::mPtmRatio ) );
	mGreenPlayer->mSprite->setRotation( -mGreenPlayer->boxBody->GetAngle() * 180.0f / M_PI );
	

	//�^�b�`�̓o�^����
	for(int i=0;i<touchManager->getTouchNum();i++){
		//�Ԃ𒲂ׂ�
		//�^�b�`�ɋ󂫂�����
		if(mRedPlayer->isEmptyTouch()){
			//�^�b�`�����u��
			if(touchManager->getTouchObject(i)->touchBeganFlag){
				//�������Ă��邩
				if(Collision::testIntersectPositionAndSquare( ccp( touchManager->getTouchObject(i)->touch_x,touchManager->getTouchObject(i)->touch_y ),
					mRedPlayer->getPos(),RedWidth,RedHeight,mRedPlayer->getAngle() ) ){
					//���̃^�b�`�ԍ���o�^����
					mRedPlayer->setTouchIndex(i);

					continue;
				}
			}
		}
		//�𒲂ׂ�
		//�^�b�`�ɋ󂫂�����
		if(mBluePlayer->isEmptyTouch()){
			//�^�b�`�����u��
			if(touchManager->getTouchObject(i)->touchBeganFlag){
				//�������Ă��邩
				if(Collision::testIntersectPositionAndSquare( ccp( touchManager->getTouchObject(i)->touch_x,touchManager->getTouchObject(i)->touch_y ),
					mBluePlayer->getPos(),BlueWidth,BlueHeight,mBluePlayer->getAngle() ) ){
					//���̃^�b�`�ԍ���o�^����
					mBluePlayer->setTouchIndex(i);
					continue;
				}
			}
		}
		// ����Ă���
		if( !touchManager->getTouchObject(i)->touchFlag ){
			//�^�b�`�ԍ������Z�b�g����
			mRedPlayer->resetTouchIndex(i);
			mBluePlayer->resetTouchIndex(i);
		}
	}

	if ( mRedPlayer->getFirstTouchedIndex() != -1 && mBluePlayer->getFirstTouchedIndex() != -1 ){
		return true;
	}

	return false;
}

CCPoint PhysicsPlayer::getPosition( int index ) const{
	switch ( index ){
		case 0 :
			return mRedPlayer->getPos();
		case 1 :
			return mBluePlayer->getPos();
	}

	return ccp( 0, 0 );
}

CCSize PhysicsPlayer::getSize( int index ) const{
	switch ( index ){
		case 0 :
			return mRedPlayer->size();
		case 1 :
			return mBluePlayer->size();
	}

	return CCSize( 0, 0 );
}

float PhysicsPlayer::getAngle( int index ) const{
	switch ( index ){
		case 0 :
			return mRedPlayer->getAngle();
		case 1 :
			return mBluePlayer->getAngle();
	}
	return 0;
}

bool PhysicsPlayer::splited() const{
	return mPlayerSplited;
}

bool PhysicsPlayer::isDead() const{
	return ( mRedDied || mBlueDied );
}

float PhysicsPlayer::getMax(float a,float b){
	if(a>b)
		return a;
	else
		return b;
}

bool PhysicsPlayer::isCleared() const{
	return ( mRedClear && mBlueClear && !mRedDied && !mBlueDied );
}

void PhysicsPlayer::setCleared( int index, bool value ){
	switch ( index ){
		case 0 :
			if ( value && mRedDied ) break;
			mRedClear = value;
			break;
		case 1 :
			if ( value && mBlueDied ) break;
			mBlueClear = value;
			break;
	}
}

bool PhysicsPlayer::isIntersect( const CCPoint& position ){
	if ( Collision::testIntersectPositionAndSquare( position, mRedPlayer->getPos(), RedWidth, RedHeight, mRedPlayer->getAngle() ) ){
		return true;
	}
	if ( Collision::testIntersectPositionAndSquare( position, mBluePlayer->getPos(), BlueWidth, BlueHeight, mBluePlayer->getAngle() ) ){
		return true;
	}

	return false;
}

bool PhysicsPlayer::isTouched( int touchIndex ){
	if ( mRedPlayer->getFirstTouchedIndex() == touchIndex || mRedPlayer->getSecondTouchedIndex() == touchIndex ||
		 mBluePlayer->getFirstTouchedIndex() == touchIndex || mBluePlayer->getSecondTouchedIndex() == touchIndex ){
			return true;
	}
	return false;
}