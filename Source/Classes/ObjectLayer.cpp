#include "ObjectLayer.h"
#include "GVector2.h"
#include "ObjBridge.h"

#include "StaticWall.h"
#include "MoveWall.h"
#include "ElasticWall.h"

#include "ObjSwitch.h"
#include "TouchManager.h"
#include "StageLayer.h"
#include "PhysicsPlayer.h"
#include "ObjGoalLine.h"
#include "Delete.h"
#include <string>
#include <sstream>

using namespace cocos2d;
using namespace std;
using namespace GameLib;

ObjectLayer::~ObjectLayer(){
	for ( int i = 0; i < mStaticObjects.size(); i++ ){
		SAFE_DELETE( mStaticObjects[ i ] );
	}
	mStaticObjects.clear();
	for ( int i = 0; i < mSwitches.size(); i++ ){
		SAFE_DELETE( mSwitches[ i ] );
	}
	mSwitches.clear();
	for ( int i = 0; i < mGoalLines.size(); i++ ){
		SAFE_DELETE( mGoalLines[ i ] );
	}
	mGoalLines.clear();
}

bool ObjectLayer::init(){	
	if ( !CCLayer::init() ) return false;

	mSwitchLayer = CCLayer::create();

	mCount = 0;

	return true;
}

void ObjectLayer::update( TouchManager* touchManager, PhysicsPlayer* player ){
	for ( unsigned int i = 0; i < mStaticObjects.size(); i++ ){
		mStaticObjects[ i ]->update( touchManager, mStageLayer );
	}

	for ( unsigned int i = 0; i < mSwitches.size(); i++ ){
		mSwitches[ i ]->update( touchManager, mStageLayer );
	}
	
	for ( int i = 0; i < mGoalLines.size(); i++ ){
		mGoalLines[ i ]->update( mStageLayer );
	}

	float range = 40.0f;
	int hitColor = -1;
	for ( int i = 0; i < 2; i++ ){
		CCPoint pos = player->getPosition( i );
		pos.x += mStageLayer->screenPosition().x;
		pos.y += -mStageLayer->screenPosition().y;

		for ( unsigned int j = 0; j < mSwitches.size(); j++ ){
			if ( mSwitches[ j ]->isIntersect( pos, player->getSize( i ), player->getAngle( i ) ) ){
				hitColor = mSwitches[ j ]->color();
				break;
			}
		}

		if ( hitColor != -1 ) break;
	}

	if ( hitColor != -1 ){
		for ( unsigned int i = 0; i < mStaticObjects.size(); i++ ){
			mStaticObjects[ i ]->setColor( ( StaticObject::Color )hitColor );
		}
	}else{
		for ( unsigned int i = 0; i < mStaticObjects.size(); i++ ){
			mStaticObjects[ i ]->resetColor();
		}
	}

	
	for ( int i = 0; i < mGoalLines.size(); i++ ){
		if ( mGoalLines[ i ]->isIntersect( mStageLayer, player->getPosition( 0 ), 32, 32, 0.0f ) ){
			player->setCleared( 0, true );
		}
		if ( mGoalLines[ i ]->isIntersect( mStageLayer, player->getPosition( 1 ), 32, 32, 0.0f ) ){
			player->setCleared( 1, true );
		}
	}

	++mCount;
}

void ObjectLayer::setStageLayer( StageLayer* stageLayer ){
	mStageLayer = stageLayer;
}

CCLayer* ObjectLayer::switchLayer(){
	return mSwitchLayer;
}

void ObjectLayer::load( int stageID, b2World* world ){
	stringstream ss;
	ss << "mapdata/map_" << stageID << ".d_obj";

	string t = ss.str();

	unsigned long size = 0;
	string data = ( char* )CCFileUtils::sharedFileUtils()->getFileData( ss.str().c_str(), "r", &size );
	data.erase( size );

	ss.str( "" );
	ss.str( data );

	string s;
	
	int numPlayer = 0;

	// オブジェクト
	while ( getline( ss, s ) ){
		int t = s.find_first_of( "obj:" );
		if ( t != string::npos ){
			s.erase( 0, s.find_first_of( ':' ) + 1 ); // obj:を外す

			// ID
			t = s.find_first_of( ";" );
			int id = atoi( s.substr( 0, t ).c_str() );

			// X, Y
			CCPoint pos;
			s.erase( 0, t + 1 );
			t = s.find_first_of( "," );
			pos.x = static_cast<float>( atof( s.substr( 0, t ).c_str() ) );
			s.erase( 0, t + 1 );
			t = s.find_first_of( ";" );
			pos.y = static_cast<float>( atof( s.substr( 0, t ).c_str() ) );
		
			// ScaleY
			s.erase( 0, t + 1 );
			t = s.find_first_of( "," );
			float scaleY = static_cast<float>( atof( s.substr( 0, t ).c_str() ) );
			
			// HeightPixel
			s.erase( 0, t + 1 );
			t = s.find_first_of( ";" );
			int height = atoi( s.substr( 0, t ).c_str() );

			// Angle
			s.erase( 0, t + 1 );
			t = s.find_first_of( ";" );
			float angle = static_cast<float>( atof( s.substr( 0, t ).c_str() ) );

			switch ( id ){
				// プレイヤー位置
				case 0 : mFirstPlayerPositions[ numPlayer ] = pos; ++numPlayer; break;

				case 1 : mStaticObjects.push_back( new StaticWall( world, pos, scaleY, angle, StaticObject::COLOR_BLACK_S ) ); break;
				case 2 : mStaticObjects.push_back( new MoveWall( world, pos, scaleY, angle, StaticObject::COLOR_BLACK_S, 400, 400 ) ); break;
				case 3 : mStaticObjects.push_back( new ElasticWall( world, pos, scaleY, angle, StaticObject::COLOR_BLACK_S ) ); break;

				case 4 : mStaticObjects.push_back( new StaticWall( world, pos, scaleY, angle, StaticObject::COLOR_BLACK_M ) ); break;
				case 5 : mStaticObjects.push_back( new MoveWall( world, pos, scaleY, angle, StaticObject::COLOR_BLACK_M, 400, 400 ) ); break;
				case 6 : mStaticObjects.push_back( new ElasticWall( world, pos, scaleY, angle, StaticObject::COLOR_BLACK_M ) ); break;

				case 7 : mStaticObjects.push_back( new StaticWall( world, pos, scaleY, angle, StaticObject::COLOR_BLACK_L ) ); break;
				case 8 : mStaticObjects.push_back( new MoveWall( world, pos, scaleY, angle, StaticObject::COLOR_BLACK_L, 400, 400 ) ); break;
				case 9 : mStaticObjects.push_back( new ElasticWall( world, pos, scaleY, angle, StaticObject::COLOR_BLACK_L ) ); break;

				case 10 : mStaticObjects.push_back( new StaticWall( world, pos, scaleY, angle, StaticObject::COLOR_BLUE ) ); break;
				case 11 : mStaticObjects.push_back( new MoveWall( world, pos, scaleY, angle, StaticObject::COLOR_BLUE, 400, 400 ) ); break;
				case 12 : mStaticObjects.push_back( new ElasticWall( world, pos, scaleY, angle, StaticObject::COLOR_BLUE ) ); break;

				case 13 : mStaticObjects.push_back( new StaticWall( world, pos, scaleY, angle, StaticObject::COLOR_RED ) ); break;
				case 14 : mStaticObjects.push_back( new MoveWall( world, pos, scaleY, angle, StaticObject::COLOR_RED, 400, 400 ) ); break;
				case 15 : mStaticObjects.push_back( new ElasticWall( world, pos, scaleY, angle, StaticObject::COLOR_RED ) ); break;

				case 16 : mStaticObjects.push_back( new ObjBridge( ( int )pos.x, ( int )pos.y, height, angle, false ) ); break;
				case 17 : mStaticObjects.push_back( new ObjBridge( ( int )pos.x, ( int )pos.y, height, angle, true ) ); break;

				case 18 : mGoalLines.push_back( new ObjGoalLine( pos, scaleY, angle ) ); break;
				//case 18 : mStaticObjects.push_back( new StaticWall( world, pos, scaleY, angle, StaticObject::COLOR_GREEN ) ); break;
				//case 19 : mStaticObjects.push_back( new MoveWall( world, pos, scaleY, angle, StaticObject::COLOR_GREEN, 400, 400 ) ); break;
				//case 20 : mStaticObjects.push_back( new ElasticWall( world, pos, scaleY, angle, StaticObject::COLOR_GREEN ) ); break;

				case 21 : mSwitches.push_back( new ObjSwitch( ( int )pos.x, ( int )pos.y, StaticObject::COLOR_BLUE ) ); break;
				case 22 : mSwitches.push_back( new ObjSwitch( ( int )pos.x, ( int )pos.y, StaticObject::COLOR_RED ) ); break;
				case 23 : mSwitches.push_back( new ObjSwitch( ( int )pos.x, ( int )pos.y, StaticObject::COLOR_GREEN ) ); break;
			}
		}
	}
	
	for ( int i = 0; i < mStaticObjects.size(); i++ ){
		mStaticObjects[ i ]->setParent( this, mSwitchLayer );
	}
	
	for ( int i = 0; i < mSwitches.size(); i++ ){
		mSwitches[ i ]->setParent( mSwitchLayer );
	}

	for ( int i = 0; i < mGoalLines.size(); i++ ){
		mGoalLines[ i ]->setParent( mSwitchLayer );
	}
}

const StaticObject* ObjectLayer::get( int index ) const{
	if ( index < 0 || index >= mStaticObjects.size() ) return NULL;
	return mStaticObjects[ index ];
}

int ObjectLayer::numObject() const{
	return mStaticObjects.size();
}

const cocos2d::CCPoint& ObjectLayer::getFirstPlayerPosition( int index ){
	return mFirstPlayerPositions[ index ];
}