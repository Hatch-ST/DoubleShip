#ifndef INCLUDED_OBJBRIDGE_H
#define INCLUDED_OBJBRIDGE_H

#include "StaticObject.h"

class ObjBridge : public StaticObject{
public :
	ObjBridge( int x, int y, int height, float angle, bool big );
	~ObjBridge();
	void setParent( cocos2d::CCLayer* layer, cocos2d::CCLayer* underLayer );
	void update( TouchManager* tm, StageLayer* sl );
	bool isIntersect( cocos2d::CCPoint position, const StageLayer& stageLayer ) const;

private :
	GameLib::Vector2 mPosition;
	float mScaleY;
	float mAngle;
	cocos2d::CCSprite* mUpSprite;
	cocos2d::CCSprite* mLowSprite;
	cocos2d::CCSprite** mMiddleSprites;
	cocos2d::CCSprite* mShadowSprite;

	GameLib::Vector2 mUpPosition;
	GameLib::Vector2 mLowPosition;
	GameLib::Vector2* mMiddlePositions;

	int mNumMiddle;
	GameLib::Vector2 mSize;

	static const int mUpAndLowSizeY;
	static const int mMiddleSizeY;
};

#endif