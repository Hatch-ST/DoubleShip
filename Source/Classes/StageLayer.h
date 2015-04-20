#ifndef INCLUDED_STAGE_H
#define INCLUDED_STAGE_H

#include "cocos2d.h"
#include "GVector2.h"
#include "GVector2i.h"
#include "Box2D\Box2D.h"
#include <vector>

class StaticObject;

class StageLayer : public cocos2d::CCLayer{
public :
	~StageLayer();

	bool init();
	void load( int stageID );
	void load( int stageID, b2World* world );

	// createä÷êî
	CREATE_FUNC( StageLayer );
	
	bool testIntersect( const cocos2d::CCPoint& start, const cocos2d::CCPoint& end, cocos2d::CCPoint* out ) const;
	bool testIntersect( const cocos2d::CCPoint& start, const cocos2d::CCPoint& end, float range, cocos2d::CCPoint* out ) const;

	const GameLib::Vector2& screenPosition() const;
	void update();

	bool cleared( const cocos2d::CCPoint& playerPosition ) const;
	void stopScroll();
	cocos2d::CCPoint scrollValue() const;

private :
	bool testIntersect( const GameLib::Vector2& start, const GameLib::Vector2& end, GameLib::Vector2* out ) const;
	bool isIntersectTile( const GameLib::Vector2& start, const GameLib::Vector2& end, int gridX, int gridY, GameLib::Vector2* out ) const;

	void putTiledMap( cocos2d::CCTMXTiledMap* tiledMap, int destX, int destY );
	void putSprite( cocos2d::CCSprite* sprite, int destX, int destY, bool leftUp = true );

	void loadObjects( const char* filename );
	void loadTileCollisionData( const char* filename );

	void createSpriteFromConnectedImage( cocos2d::CCSprite** out, char* filename, int numTotal, int numX, int numY, int width, int height );
	void switchMapImage( cocos2d::CCSprite* out, int id );
	
	// É^ÉCÉãÇÃè’ìÀíËã`
	std::vector< std::vector<GameLib::Vector2> > mTileCollisionData;

	cocos2d::CCTMXTiledMap* mTiledMap;
	cocos2d::CCTMXLayer* mTiledLayer;

	GameLib::Vector2 mScreenPosition;
	bool mScrollStoped;
	float mScrollSpeed;

	float mTotalCameraMoved;

	std::vector<GameLib::Vector2> mCameraPositions;
	std::vector<float> mCameraDistances;

	GameLib::Vector2 mPrevScreenPosition;

	bool mScrollEnd;

	struct BoxBody{
		b2Body* body;
		GameLib::Vector2 defPos;
		float angle;
	};
	std::vector<BoxBody> mBodies;

	cocos2d::CCLabelTTF* mDdebugLabel;

	//GameLib::Vector2 mTouchedPos;
	//bool ccTouchBegan( cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent );
	//void ccTouchMoved( cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent );
	//void ccTouchEnded( cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent );

	static const float mPtmRatio;
};

#endif