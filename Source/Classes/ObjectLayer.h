#ifndef INCLUDED_OBJECT_LAYER_H
#define INCLUDED_OBJECT_LAYER_H

#include "cocos2d.h"
#include "StaticObject.h"
#include "Box2d/Box2D.h"
#include <vector>

class StageLayer;
class TouchManager;
class ObjSwitch;
class PhysicsPlayer;
class ObjGoalLine;

class ObjectLayer : public cocos2d::CCLayer{
public :
	~ObjectLayer();
	bool init();
	CREATE_FUNC( ObjectLayer );

	void update( TouchManager* touchManager, PhysicsPlayer* player );
	void setStageLayer( StageLayer* );
	void load( int stageID, b2World* world );

	const StaticObject* get( int index ) const;
	int numObject() const;

	const cocos2d::CCPoint& getFirstPlayerPosition( int index );

	cocos2d::CCLayer* switchLayer();

private :
	StageLayer* mStageLayer;
	std::vector<StaticObject*> mStaticObjects;
	std::vector<ObjSwitch*> mSwitches;
	std::vector<ObjGoalLine*> mGoalLines;

	cocos2d::CCLayer* mSwitchLayer;

	cocos2d::CCPoint mFirstPlayerPositions[ 2 ];

	unsigned int mCount;
};

#endif