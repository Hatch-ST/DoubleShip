#include "ObjGoalLine.h"
#include "TouchManager.h"
#include "StageLayer.h"
#include "ScreenConverter.h"
#include "Collision.h"

using namespace cocos2d;
using namespace GameLib;

ObjGoalLine::ObjGoalLine( CCPoint pos, float scale, float angle ){
	mSprite = CCSprite::create( "image/object/goalline.png" );
	mPosition = pos;
	mPosition.y = ScreenConverter::convertY( mPosition.y );

	mAngle = angle;

	mSize = mSprite->getContentSize();
	mSize.height *= scale;
	
	mSprite->setScaleY( scale );
	mSprite->setRotation( mAngle * 180.0f / M_PI );
	mSprite->setPosition( mPosition );
}

void ObjGoalLine::setParent( CCLayer* layer ){
	layer->addChild( mSprite );
}

void ObjGoalLine::update( StageLayer* sl ){
	CCPoint dest = ccp( mPosition.x - sl->screenPosition().x, mPosition.y + sl->screenPosition().y );
	mSprite->setPosition( dest );
}

bool ObjGoalLine::isIntersect( StageLayer* sl, CCPoint position, float width, float height, float angle ){
	CCPoint myPos = ccp( mPosition.x - sl->screenPosition().x, mPosition.y + sl->screenPosition().y );
	return Collision::testIntersectSquare( myPos, mSize.width, mSize.height, mAngle, position, width, height, angle );
}