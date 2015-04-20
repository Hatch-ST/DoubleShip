#include "PlayerData.h"
#include "FixtureGroup.h"
#include <cmath>

#include "SimpleAudioEngine.h"
using namespace CocosDenshion;

#define PTM_RATIO 32.0

PlayerData::PlayerData(CCLayer* layer,b2World &world,PlayerType type,float playerWidth,float playerHeight){
	init(layer,world,type,playerWidth,playerHeight);
}

PlayerData::PlayerData(CCLayer* layer,b2World &world,PlayerType type,float playerWidth,float playerHeight,CCPoint startPos,float startAngle){
	init(layer,world,type,playerWidth,playerHeight);
	setPos(startPos,startAngle);
}

PlayerData::~PlayerData(){
	delete mPlayerTouchSize;
	delete mBeforeVector;
	delete mFirstBeginPos;
}

void PlayerData::init(CCLayer* layer,b2World &world,PlayerType type,float playerWidth,float playerHeight){
	//CCSpriteBatchNode* playerBatch = CCSpriteBatchNode::create("image/ship.png");
	mPlayerType = type;
	mPlayerTouchSize = new CCSize();
	mPlayerTouchSize->width = playerWidth;
	mPlayerTouchSize->height = playerHeight;

	CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
	
	//箱
	b2BodyDef boxBodyDef;
	boxBodyDef.type = b2_dynamicBody;
	boxBody = world.CreateBody(&boxBodyDef);      //Bodyを作成
 
	int fixtureGroupIndex = 0;
	b2PolygonShape rect;    //長方形を作成
	//mSprite = new PhysicsSprite();
	switch(mPlayerType){
	case PLAYER_GREEN:
		//mSprite->initWithFile("player_green.png");
		mSprite = CCSprite::create( "image/player_green.png" );
		mSprite->setVisible(false);
		mSprite->setTag(GREEN_ID);
		mSize.setSize(100,200);
		boxBody->SetActive(false);
		fixtureGroupIndex = FixtureGroup::GR_GREEN;
		break;
	case PLAYER_RED:
		mSprite = CCSprite::create( "image/ship_red.png" );
		//mSprite->initWithFile("image/ship_red.png");
		mSprite->setVisible(true);
		mSprite->setTag(RED_ID);
		mSize.setSize(60,140);
		fixtureGroupIndex = FixtureGroup::GR_RED;
		break;
	case PLAYER_BLUE:
		mSprite = CCSprite::create( "image/ship_blue.png" );
		//mSprite->initWithFile("image/ship_blue.png");
		mSprite->setVisible(true);
		mSprite->setTag(BLUE_ID);
		mSize.setSize(60,140);
		fixtureGroupIndex = FixtureGroup::GR_BLUE;
		break;
	}

	rect.SetAsBox((mSize.width/2)/PTM_RATIO, (mSize.height/2)/PTM_RATIO);

	b2FixtureDef boxFixtureDef;
	boxFixtureDef.shape = &rect;
	boxFixtureDef.density = 0.0;
	boxFixtureDef.friction = 0.5;
	boxFixtureDef.restitution = 0.5;
	boxFixtureDef.filter.groupIndex = fixtureGroupIndex;
	boxBody->CreateFixture(&boxFixtureDef); 

	//mSprite->setPhysicsBody(boxBody);

	layer->addChild(mSprite);
	
	mBeforeVector = new CCPoint();
	mFirstBeginPos = new CCPoint();

	mFirstTouchedIndex = -1;
	mSecondTouchedIndex = -1;

}

void PlayerData::setPos(CCPoint pos,float angle){
	boxBody->SetTransform(b2Vec2( pos.x/PTM_RATIO, pos.y/PTM_RATIO ),angle);
}

void PlayerData::addPos(CCPoint pos){
	b2Vec2 movedPos (boxBody->GetPosition().x + pos.x/PTM_RATIO, boxBody->GetPosition().y + pos.y/PTM_RATIO );
	boxBody->SetTransform(b2Vec2( movedPos.x, movedPos.y ),boxBody->GetAngle());
}

void PlayerData::rotation( float angle ){
	boxBody->SetTransform( boxBody->GetPosition(), boxBody->GetAngle() + angle );
}

bool PlayerData::isTouched(float touch_x,float touch_y){
	int Pos_x=boxBody->GetPosition().x * PTM_RATIO;
	int Pos_y=boxBody->GetPosition().y * PTM_RATIO;
	if( (Pos_x - mPlayerTouchSize->width * 0.5 < touch_x && touch_x < Pos_x + mPlayerTouchSize->width * 0.5) && 
		(Pos_y - mPlayerTouchSize->height * 0.5 < touch_y && touch_y < Pos_y + mPlayerTouchSize->height * 0.5) ){
		return true;
	}
	return false;
}

void PlayerData::setTouchIndex(int index){
	//タッチ音
	SimpleAudioEngine::sharedEngine()->playEffect("se/up.wav");
	if(mFirstTouchedIndex == -1){
		mFirstTouchedIndex = index;
	}else{
		mSecondTouchedIndex = index;
	}
}

void PlayerData::resetTouchIndex(int index){
	if(mFirstTouchedIndex == index){
		if(mSecondTouchedIndex != -1){
			mFirstTouchedIndex = mSecondTouchedIndex;
			mSecondTouchedIndex = -1;
		}else{
			mFirstTouchedIndex = -1;
		}
	}else if(mSecondTouchedIndex == index){
		mSecondTouchedIndex = -1;
	}
}

void PlayerData::resetTouchIndexAll(){
	mFirstTouchedIndex = -1;
	mSecondTouchedIndex = -1;
}

//タッチに空きがあるか
bool PlayerData::isEmptyTouch(){
	if(mFirstTouchedIndex == -1 || mSecondTouchedIndex == -1){
		return true;
	}
	return false;
}

void PlayerData::setBeforeVector(float secondPos_x,float secondPos_y){
	mBeforeVector->x = secondPos_x - mFirstBeginPos->x;
	mBeforeVector->y = secondPos_y - mFirstBeginPos->y;
}

void PlayerData::setFirstBeginPos(float firstPos_x,float firstPos_y){
	mFirstBeginPos->x = firstPos_x;
	mFirstBeginPos->y = firstPos_y;
}

float PlayerData::getAngleFromBeginVector(float secondPos_x,float secondPos_y){
	CCPoint nextVector;
	nextVector.x = secondPos_x - mFirstBeginPos->x;
	nextVector.y = secondPos_y - mFirstBeginPos->y;
	float angle = nextVector.getAngle( *mBeforeVector );
	float sita = -angle * 180 / M_PI;
	mBeforeVector = &nextVector;
	return sita;
}

CCPoint PlayerData::getPos(){
	return ccp(boxBody->GetPosition().x * PTM_RATIO,boxBody->GetPosition().y * PTM_RATIO);
}

CCSize PlayerData::size() const{
	return mSize;
}