#include "StageLayer.h"
#include "GVector2.h"
#include "Collision.h"
#include "ObjBridge.h"
#include "ScreenConverter.h"
#include <string>
#include <sstream>
#include <fstream>
#include <sstream>

using namespace cocos2d;
using namespace GameLib;
using namespace std;

const float StageLayer::mPtmRatio = 32.0f;

StageLayer::~StageLayer(){
	mTileCollisionData.clear();
	mCameraPositions.clear();
	mCameraDistances.clear();
}

//-----------------------------------------------------------------------------
// 初期化処理
//-----------------------------------------------------------------------------
bool StageLayer::init(){
	if ( !CCLayer::init() ) return false;

	CCDirector::sharedDirector()->setProjection( kCCDirectorProjection2D );
    CCDirector::sharedDirector()->setDepthTest( false );

	loadTileCollisionData( "tileCollisionDefinition.txt" );
	
	mScrollSpeed = 0.75f;
	mTotalCameraMoved = 0;

	//setTouchMode( kCCTouchesOneByOne );
	//setTouchEnabled( true );

	mScrollEnd = false;
	mScrollStoped = false;

	//mDdebugLabel = CCLabelTTF::create( "", "", 32.0f );
	//mDdebugLabel->setPosition( ccp( 200, 100 ) );

	return true;
}

//-----------------------------------------------------------------------------
// 読み込み
//-----------------------------------------------------------------------------
void StageLayer::load( int stageID ){
	stringstream ss;
	ss << "map_" << stageID;

	// カメラの読み込み
	loadObjects( ( "mapdata/" + ss.str() + ".d_cam" ).c_str() );

	// マップの読み込み
	mTiledMap = CCTMXTiledMap::create( ( "mapdata/" + ss.str() + ".tmx" ).c_str() );
	mTiledLayer = mTiledMap->layerNamed( ss.str().c_str() );

	putTiledMap( mTiledMap, 0, 0 );

	// マップを登録
	addChild( mTiledMap );
	
	mScreenPosition = mCameraPositions[ 0 ];
	mPrevScreenPosition = mCameraPositions[ 0 ];
}

void StageLayer::load( int stageID, b2World* world ){
	load( stageID );
	
	stringstream ss;
	ss << "mapdata/box_" << stageID << ".txt";

	unsigned long size = 0;
	string data = ( char* )CCFileUtils::sharedFileUtils()->getFileData( ss.str().c_str(), "r", &size );
	data.erase( size );

	ss.str( "" );
	ss.str( data );
	string s;

	while ( getline( ss, s ) ){
		int t = s.find_first_of( "box:" );
		if ( t != string::npos ){
			s.erase( 0, s.find_first_of( ':' ) ); // box:を外す

			float x, y;
			float width, height, angle;

			// 座標
			s.erase( 0, t + 1 );
			t = s.find_first_of( "," );
			x = static_cast<float>( atof( s.substr( 0, t ).c_str() ) );
			s.erase( 0, t + 1 );
			t = s.find_first_of( ";" );
			y = ScreenConverter::convertY( static_cast<float>( atof( s.substr( 0, t ).c_str() ) ) );
			
			// 大きさ
			s.erase( 0, t + 1 );
			t = s.find_first_of( "," );
			width = static_cast<float>( atof( s.substr( 0, t ).c_str() ) );
			s.erase( 0, t + 1 );
			t = s.find_first_of( ";" );
			height = static_cast<float>( atof( s.substr( 0, t ).c_str() ) );

			// 角度
			s.erase( 0, t + 1 );
			t = s.find_first_of( ";" );
			angle = static_cast<float>( atof( s.substr( 0, t ).c_str() ) );


			// Boxデータを作る
			b2BodyDef bodyDef;
			bodyDef.type = b2_staticBody;

			bodyDef.position.Set( x / mPtmRatio, y / mPtmRatio );

			BoxBody box;
			box.body = world->CreateBody( &bodyDef );

			b2PolygonShape poly;
			width *= 0.5f;
			height *= 0.5f;
			poly.SetAsBox( width / mPtmRatio, height / mPtmRatio );

			b2FixtureDef fixtureDef;
			fixtureDef.shape = &poly;
			fixtureDef.density = 1.0f;

			box.body->CreateFixture( &fixtureDef );
			box.body->SetTransform( b2Vec2( x / mPtmRatio, y / mPtmRatio ), -angle );

			box.defPos.set( x, y );
			box.angle = angle;

			mBodies.push_back( box );
		}
	}
	
	//addChild( mDdebugLabel );
}

void StageLayer::stopScroll(){
	mScrollStoped = true;
}

//-----------------------------------------------------------------------------
// 更新処理
//-----------------------------------------------------------------------------
void StageLayer::update(){
	if ( !mScrollStoped ){
		mTotalCameraMoved += mScrollSpeed;
	}else{
		mTotalCameraMoved += mScrollSpeed * 0.4f;
	}

	if ( mCameraPositions.size() > 1 && mTotalCameraMoved > mCameraDistances[ mCameraDistances.size() - 1 ] ){
		mTotalCameraMoved = mCameraDistances[ mCameraDistances.size() - 1 ] - 1.0f;
	}

	// スクリーン座標を保存
	mPrevScreenPosition = mScreenPosition;

	if ( mCameraPositions.size() > 1 ){
		// 現在のカメラを探す
		for ( unsigned int i = 0; i < mCameraDistances.size(); i++ ){
			if ( mTotalCameraMoved < mCameraDistances[ i ] ){
				Vector2 dif = mCameraPositions[ i + 1 ] - mCameraPositions[ i ];
				float t = 1.0f - ( ( mCameraDistances[ i ] - mTotalCameraMoved ) / dif.length() );
				mScreenPosition = mCameraPositions[ i ] + dif * t;

				if ( i == mCameraDistances.size() - 1 && t > 9.9f ){
					mScrollEnd = true;
				}

				break;
			}
		}
	}else{
		mScreenPosition = mCameraPositions[ 0 ];
		mScrollEnd = true;
	}

	putTiledMap( mTiledMap, -mScreenPosition.ix(), -mScreenPosition.iy() );

	for ( int i = 0; i < mBodies.size(); i++ ){
		Vector2 pos = mBodies[ i ].defPos;
		pos.x -= mScreenPosition.x;
		pos.y += mScreenPosition.y;
		mBodies[ i ].body->SetTransform( b2Vec2( pos.x / mPtmRatio, pos.y / mPtmRatio ), -mBodies[ i ].angle );
	}

	mScrollStoped = false;

	//if ( mCameraDistances.size() > 0 ){
	//	CCString* str = CCString::createWithFormat( "%f, %f, %d", mTotalCameraMoved, mCameraDistances[ 0 ], mCameraDistances.size() );
	//	mDdebugLabel->setString( str->getCString() );
	//}
}

//-----------------------------------------------------------------------------
// マップとの衝突を調べる
//-----------------------------------------------------------------------------
bool StageLayer::testIntersect( const Vector2& start, const Vector2& end, GameLib::Vector2* out ) const{
	Vector2i startGrid, endGrid;
	startGrid.x = start.x / mTiledMap->getTileSize().width;
	startGrid.y = start.y / mTiledMap->getTileSize().height;

	endGrid.x = end.x / mTiledMap->getTileSize().width;
	endGrid.y = end.y / mTiledMap->getTileSize().height;

	int width = abs( startGrid.x - endGrid.x ) + 1;
	int height = abs( startGrid.y - endGrid.y ) + 1;

	// 上がってくか下がってくか
	bool xUp = ( startGrid.x <= endGrid.x );
	bool yUp = ( startGrid.y <= endGrid.y );

	*out = end;
	
	bool hit = false;
	for ( int y = 0; y <= height; y++ ){
		for ( int x = 0; x <= width; x++ ){
			int gridX = min( startGrid.x, endGrid.x ) + x;
			int gridY = min( startGrid.y, endGrid.y ) + y;

			Vector2 nowHitPosition;
			// 現在のグリッドと衝突チェック
			if ( isIntersectTile( start, end, gridX, gridY, &nowHitPosition ) ){
				// これまでの衝突点よりも始点に近ければ保存
				if ( ( *out - start ).sqLength() > ( nowHitPosition - start ).sqLength() ){
					*out = nowHitPosition;
				}
				hit = true;
			}
		}
	}
	
	if ( !hit ){
		out = NULL;
	}

	return hit;

	// 保留
	//if ( width > height ){
	//	float yOne = ( float )height / width;
	//	float y = 0;
	//	bool hit = false;
	//	for ( int x = 0; x < width; x++ ){
	//		int gridX = startGrid.x + ( ( xUp ) ? x : -x );
	//		int gridY = startGrid.y + static_cast<int>( y );

	//		Vector2 nowHitPosition;
	//		// 現在のグリッドと衝突チェック
	//		if ( isIntersectTile( start, end, gridX, gridY, &nowHitPosition ) ){
	//			// これまでの衝突点よりも始点に近ければ保存
	//			if ( ( *out - start ).sqLength() > ( nowHitPosition - start ).sqLength() ){
	//				*out = nowHitPosition;
	//			}
	//			hit = true;
	//		}
	//		
	//		// 隣のグリッドと衝突チェック
	//		if ( isIntersectTile( start, end, gridX, gridY + 1, &nowHitPosition ) ){
	//			// これまでの衝突点よりも始点に近ければ保存
	//			if ( ( *out - start ).sqLength() > ( nowHitPosition - start ).sqLength() ){
	//				*out = nowHitPosition;
	//			}
	//			hit = true;
	//		}
	//		// 隣のグリッドと衝突チェック
	//		if ( isIntersectTile( start, end, gridX, gridY - 1, &nowHitPosition ) ){
	//			// これまでの衝突点よりも始点に近ければ保存
	//			if ( ( *out - start ).sqLength() > ( nowHitPosition - start ).sqLength() ){
	//				*out = nowHitPosition;
	//			}
	//			hit = true;
	//		}

	//		y = ( yUp ) ? ( y + yOne ) : ( y -yOne );

	//	}
	//	// 衝突があれば終了
	//	if ( hit ){
	//		return true;
	//	}
	//}else{
	//	float xOne = ( float )width / height;
	//	float x = 0;
	//	bool hit = false;
	//	for ( int y = 0; y < height; y++ ){
	//		int gridX = startGrid.x + static_cast<int>( x );
	//		int gridY = startGrid.y + ( ( yUp ) ?  y : -y );

	//		Vector2 nowHitPosition;
	//		// 現在のグリッドと衝突チェック
	//		if ( isIntersectTile( start, end, gridX, gridY, &nowHitPosition ) ){
	//			// これまでの衝突点よりも始点に近ければ保存
	//			if ( ( *out - start ).sqLength() > ( nowHitPosition - start ).sqLength() ){
	//				*out = nowHitPosition;
	//			}
	//			hit = true;
	//		}
	//		
	//		// 隣のグリッドと衝突チェック
	//		if ( isIntersectTile( start, end, gridX + 1, gridY, &nowHitPosition ) ){
	//			// これまでの衝突点よりも始点に近ければ保存
	//			if ( ( *out - start ).sqLength() > ( nowHitPosition - start ).sqLength() ){
	//				*out = nowHitPosition;
	//			}
	//			hit = true;
	//		}
	//		// 隣のグリッドと衝突チェック
	//		if ( isIntersectTile( start, end, gridX - 1, gridY, &nowHitPosition ) ){
	//			// これまでの衝突点よりも始点に近ければ保存
	//			if ( ( *out - start ).sqLength() > ( nowHitPosition - start ).sqLength() ){
	//				*out = nowHitPosition;
	//			}
	//			hit = true;
	//		}

	//		x = ( xUp ) ? ( x + xOne ) : ( x - xOne );
	//	}
	//	// 衝突があれば終了
	//	if ( hit ){
	//		return true;
	//	}
	//}

	//out = NULL;
	//return false;

}

bool StageLayer::testIntersect( const CCPoint& start, const CCPoint& end, cocos2d::CCPoint* out ) const{
	Vector2 t;
	Vector2 vSt( start.x, ScreenConverter::convertY( start.y ) );
	Vector2 vEnd( end.x, ScreenConverter::convertY( end.y ) );

	vSt -= mScreenPosition - mPrevScreenPosition;

	vSt += mScreenPosition;
	vEnd += mScreenPosition;

	if ( testIntersect( vSt, vEnd, &t ) ){
		Vector2 d = Vector2::normalize( ( vSt - vEnd ) );
		d *= 2.0f;

		t += d;
		t -= mScreenPosition;
		ScreenConverter::toLeftDown( &t.y );
		out->x = t.x;
		out->y = t.y;
		return true;
	}else{
		out = NULL;
		return false;
	}
}

bool StageLayer::testIntersect( const CCPoint& start, const CCPoint& end, float range, cocos2d::CCPoint* out ) const{
	Vector2 t;
	Vector2 vSt( start.x, ScreenConverter::convertY( start.y ) );
	Vector2 vEnd( end.x, ScreenConverter::convertY( end.y ) );

	vSt -= mScreenPosition - mPrevScreenPosition;

	vSt += mScreenPosition;
	vEnd += mScreenPosition;

	Vector2 stEnd;
	if ( ( vEnd - vSt ).sqLength() > 0 ){
		stEnd = Vector2::normalize( vEnd - vSt );
		stEnd *= range;
		vEnd += stEnd;
	}

	if ( testIntersect( vSt, vEnd, &t ) ){
		Vector2 d = Vector2::normalize( ( vSt - vEnd ) );
		d *= 2.0f;

		t += d;
		t -= mScreenPosition;
		t -= stEnd;
		ScreenConverter::toLeftDown( &t.y );
		out->x = t.x;
		out->y = t.y;
		return true;
	}else{
		out = NULL;
		return false;
	}
}

//-----------------------------------------------------------------------------
// タイルとの衝突を調べる
//-----------------------------------------------------------------------------
bool StageLayer::isIntersectTile( const Vector2& start, const Vector2& end, int gridX, int gridY, Vector2* out ) const{
	// 範囲外
	if ( gridX < 0 || gridX >= mTiledMap->getMapSize().width || gridY < 0 || gridY >= mTiledMap->getMapSize().height ){
		out = NULL;
		return false;
	}

	// タイル番号を取得
	int tileID = mTiledLayer->tileGIDAt( ccp( gridX, gridY ) );
	if ( tileID == 0 ) return false;

	// タイルの座標を取得
	Vector2 tilePos;
	mTiledLayer->tileAt( ccp( gridX, gridY ) )->getPosition( &tilePos.x, &tilePos.y );

	// タイルの座標を左上原点に変換
	ScreenConverter::toLeftUp( &tilePos.y, mTiledMap->getTileSize().height * mTiledMap->getMapSize().height );
	tilePos.y -= mTiledMap->getTileSize().height;

	if ( mTileCollisionData[ tileID - 1 ].size() == 0 ) return false;
	return Collision::testIntersectLineAndPolygon( start, end, tilePos, mTileCollisionData[ tileID - 1 ], out );
}

//-----------------------------------------------------------------------------
// 左上を原点としてマップを設置
//-----------------------------------------------------------------------------
void StageLayer::putTiledMap( cocos2d::CCTMXTiledMap* tiledMap, int destX, int destY ){
    CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
	
	int x = destX;
	int y = visibleSize.height - destY;
	y -= ( tiledMap->getMapSize().height * tiledMap->getTileSize().height );

	tiledMap->setPosition( ccp( x, y ) );
}

//-----------------------------------------------------------------------------
// 左上を原点としてスプライトを設置
//-----------------------------------------------------------------------------
void StageLayer::putSprite( CCSprite* sprite, int destX, int destY, bool leftUp ){
    CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
	
	int x = destX ;
	int y = visibleSize.height - destY;

	if ( leftUp ){
		x += sprite->getTextureRect().size.width / 2;
		y -= sprite->getTextureRect().size.height / 2;
	}

	sprite->setPosition( ccp( x, y ) );
}

//-----------------------------------------------------------------------------
// オブジェクトの読み込み
//-----------------------------------------------------------------------------
void StageLayer::loadObjects( const char* filename ){
	mCameraPositions.clear();
	mCameraDistances.clear();

	unsigned long size = 0;
	string data = ( char* )CCFileUtils::sharedFileUtils()->getFileData( filename, "r", &size );
	data.erase( size );

	istringstream ss;
	ss.str( data );

	string s;
	
	// オブジェクト
	while ( getline( ss, s ) ){
		int t = s.find_first_of( "cam:" );
		if ( t != string::npos ){
			s.erase( 0, s.find_first_of( ':' ) + 1 ); // obj:を外す

			Vector2 pos;

			// X
			t = s.find_first_of( "," );
			pos.x = static_cast<float>( atof( s.substr( 0, t ).c_str() ) );

			// Y
			s.erase( 0, t + 1 );
			t = s.find_first_of( ";" );
			pos.y = static_cast<float>( atof( s.substr( 0, t ).c_str() ) );


			if ( mCameraPositions.size() >= 1 ){
				Vector2 dif = pos - mCameraPositions[ mCameraPositions.size() - 1 ];
				float total = ( mCameraDistances.size() > 0 ) ? mCameraDistances[ mCameraDistances.size() - 1 ] : 0;
				mCameraDistances.push_back( total + dif.length() );
			}
			mCameraPositions.push_back( pos );
		}
	}
}

//-----------------------------------------------------------------------------
// タイルの衝突定義を読み込む
//-----------------------------------------------------------------------------
void StageLayer::loadTileCollisionData( const char* filename ){
	unsigned long size = 0;
	string data = ( char* )CCFileUtils::sharedFileUtils()->getFileData( filename, "r", &size );

	istringstream ss;
	ss.str( data );

	string s;
	while ( getline( ss, s ) ){
		// 辺の数
		int t = s.find_first_of( ":" );

		if ( t != string::npos ){
			int length = atoi( s.substr( 0, t ).c_str() );
			vector<Vector2> positions;
			for ( int i = 0; i < length; i++ ){
				Vector2 pos;

				s.erase( 0, t + 1 );
				t = s.find_first_of( "," );
				pos.x = static_cast<float>( atof( s.substr( 0, t ).c_str() ) );

				s.erase( 0, t + 1 );
				t = s.find_first_of( "/" );
				if ( t == string::npos ) t = s.find_first_of( ";" );
				pos.y = static_cast<float>( atof( s.substr( 0, t ).c_str() ) );

				positions.push_back( pos );
			}

			mTileCollisionData.push_back( positions );
		}
	}
}

//-----------------------------------------------------------------------------
// 連結画像からスプライトを生成
//-----------------------------------------------------------------------------
void StageLayer::createSpriteFromConnectedImage( CCSprite** out, char* filename, int numTotal, int numX, int numY, int width, int height ){
	for ( int y = 0; y < numY; y++ ){
		for ( int x = 0; x < numX; x++ ){
			if ( y * numX + x < numTotal ){
				CCRect rect( x * width, y * height, width, height );
				out[ y * numX + x ] = CCSprite::create( filename, rect );
			}
		}
	}
}

//-----------------------------------------------------------------------------
// 画像を切り替える
//-----------------------------------------------------------------------------
void StageLayer::switchMapImage( CCSprite* out, int id ){
	//int x = id % mNumMapImageX;
	//int y = id / mNumMapImageY;
	//out->setTextureRect( CCRect( x * mMapImageWidth, y * mMapImageHeight, mMapImageWidth, mMapImageHeight ) );
}

const Vector2& StageLayer::screenPosition() const{
	return mScreenPosition;
}

CCPoint StageLayer::scrollValue() const{
	return ccp( mScreenPosition.x - mPrevScreenPosition.x, -( mScreenPosition.y - mPrevScreenPosition.y ) );
}

bool StageLayer::cleared( const cocos2d::CCPoint& playerPosition ) const{
	return ( mScrollEnd && playerPosition.x > cocos2d::CCDirector::sharedDirector()->getVisibleSize().width * 0.7f );
}

//bool StageLayer::ccTouchBegan( CCTouch *pTouch, CCEvent *pEvent ){
//	mTouchedPos.set( pTouch->getLocation().x, pTouch->getLocation().y );
//
//	convertToLeftUp( &mTouchedPos.y );
//
//	Vector2 up = mTouchedPos + Vector2( 800, -600 );
//	Vector2 cross;
//	if ( testIntersect( mTouchedPos + mScreenPosition, up + mScreenPosition, &cross ) ){
//		putSprite( mImage, -mScreenPosition.x + cross.x, -mScreenPosition.y + cross.y, false );
//	}else{
//		putSprite( mImage, -mScreenPosition.x, -mScreenPosition.y, false );
//	}
//	return true;
//}
//
//void StageLayer::ccTouchMoved( CCTouch *pTouch, CCEvent *pEvent ){
//	mTouchedPos.set( pTouch->getLocation().x, pTouch->getLocation().y );
//
//	convertToLeftUp( &mTouchedPos.y );
//	
//	Vector2 up = mTouchedPos + Vector2( 800, -600 );
//	Vector2 cross;
//	if ( testIntersect( mTouchedPos + mScreenPosition, up + mScreenPosition, &cross ) ){
//		putSprite( mImage, -mScreenPosition.x + cross.x, -mScreenPosition.y + cross.y, false );
//	}else{
//		putSprite( mImage, -mScreenPosition.x, -mScreenPosition.y, false );
//	}
//}
//
//void StageLayer::ccTouchEnded( CCTouch *pTouch, CCEvent *pEvent ){
//}