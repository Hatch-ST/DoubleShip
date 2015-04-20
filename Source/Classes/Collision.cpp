#include "Collision.h"
#include "Matrix34.h"
#include "GVector2.h"
#include <cmath>

namespace GameLib{

//-----------------------------------------------------------------------------
// 座標と四角形の衝突を調べる
//-----------------------------------------------------------------------------
bool Collision::testIntersectPositionAndSquare( const Vector2& position, const Vector2& sqLeftUp, const Vector2& sqRightDown ){
	return ( position.x >= sqLeftUp.x && position.x <= sqRightDown.x && position.y >= sqLeftUp.y && position.y <= sqRightDown.y );
}

//-----------------------------------------------------------------------------
// 座標と三角形の衝突を調べる
//-----------------------------------------------------------------------------
bool Collision::testIntersectPositionAndTriangle( const Vector2& position, const Vector2& tr0, const Vector2& tr1, const Vector2& tr2 ){
	Vector2 vA( tr1 - tr0 );
	Vector2 vB( tr2 - tr0 );
	Vector2 p = position;

	// p = tr0 + vA * u + vB * v となる u, v を算出
	float v = ( vB.x * ( p.y - tr0.y ) - vB.y * ( p.x - tr0.x ) ) / ( vB.x * vA.y - vB.y * vA.x );
	if ( v < 0.0f || v > 1.0f ) return false;

	float u = ( p.x - tr0.x - vA.x * v ) / vB.x;
	if ( v + u > 1.0f || u < 0.0f ) return false;

	return true;
}
	
//-----------------------------------------------------------------------------
// 座標と四角形の衝突を調べる
//-----------------------------------------------------------------------------
bool Collision::testIntersectPositionAndSquare( const Vector2& position, const Vector2& sq0, const Vector2& sq1, const Vector2& sq2, const Vector2& sq3 ){
	if ( testIntersectPositionAndTriangle( position, sq0, sq1, sq2 ) ) return true;
	if ( testIntersectPositionAndTriangle( position, sq2, sq3, sq0 ) ) return true;

	return false;
}

//-----------------------------------------------------------------------------
// 座標と四角形の衝突を調べる
//-----------------------------------------------------------------------------
bool Collision::testIntersectPositionAndSquare( const Vector2& position, const Vector2& squarePosition, float squareWidth, float squareHeight, float squareAngle ){
	Vector2 sqPos[ 4 ];
	Matrix34 rota;
	rota.setRotationZ( squareAngle );
	Vector2 pos( 0, 0 );

	for ( int i = 0; i < 4; i++ ){
		switch ( i ){
			case 0 :
				pos.x = -squareWidth / 2;
				pos.y = -squareHeight / 2;
				break;
			case 1 :
				pos.x = -squareWidth / 2;
				pos.y = squareHeight / 2;
				break;
			case 2 :
				pos.x = squareWidth / 2;
				pos.y = squareHeight / 2;
				break;
			case 3 :
				pos.x = squareWidth / 2;
				pos.y = -squareHeight / 2;
				break;
		}

		pos *= rota;
		sqPos[ i ].set( pos.x + squarePosition.x, pos.y + squarePosition.y );
	}
	
	return testIntersectPositionAndSquare( position, sqPos[ 0 ], sqPos[ 1 ], sqPos[ 2 ], sqPos[ 3 ] );
}

bool Collision::testIntersectPositionAndSquare( const cocos2d::CCPoint& pos, const cocos2d::CCPoint& sqPos, float sqW, float sqH, float sqAngle ){
	Vector2 vPos( pos.x, pos.y );
	Vector2 vSqPos( sqPos.x, sqPos.y );
	return testIntersectPositionAndSquare( vPos, vSqPos, sqW, sqH, sqAngle );
}

//-----------------------------------------------------------------------------
// 線分同士の衝突を調べる
//-----------------------------------------------------------------------------
bool Collision::testIntersectLine( const Vector2& a0, const Vector2& a1, const Vector2& b0, const Vector2& b1, Vector2* out ){
	Vector2 vA = a1 - a0;
	Vector2 vB = b1 - b0;
	float d = ( vB.x * vA.y - vA.x * vB.y );

	// 平行
	if ( d == 0.0f ){
		out = NULL;
		return false;
	}

	Vector2 vAB = b0 - a0;
	float at = ( vB.x * vAB.y - vB.y * vAB.x ) / d;
	float bt = ( vA.x * vAB.y - vA.y * vAB.x ) / d;

	// 範囲内ならヒット
	if ( at >= 0.0f && at <= 1.0f && bt >= 0.0f && bt <= 1.0f ){
		if ( out != NULL ) *out = a0 + vA * at;
		return true;
	}
	// 範囲外なら外れ
	else{
		out = NULL;
		return false;
	}
}

//-----------------------------------------------------------------------------
// 線分と三角形の衝突を調べる
//-----------------------------------------------------------------------------
bool Collision::testIntersectLineAndTriangle( const Vector2& start, const Vector2& end, const Vector2& tr0, const Vector2& tr1, const Vector2& tr2, Vector2* out ){
	Vector2 cross;
	bool hit = false;
	if ( out != NULL ) *out = end;
	if ( testIntersectLine( start, end, tr0, tr1, &cross ) ){
		if ( out != NULL ) *out = cross;
		hit = true;
	}
	if ( testIntersectLine( start, end, tr1, tr2, &cross ) ){
		// 交点が今までより始点に近ければ保存
		if ( out != NULL && ( *out - start ).sqLength() > ( cross - start ).sqLength() ){
			*out = cross;
		}
		hit = true;
	}
	if ( testIntersectLine( start, end, tr2, tr0, &cross ) ){
		// 交点が今までより始点に近ければ保存
		if ( out != NULL && ( *out - start ).sqLength() > ( cross - start ).sqLength() ){
			*out = cross;
		}
		hit = true;
	}

	if ( hit ){
		return true;
	}else{
		out = NULL;
		return false;
	}
}

//-----------------------------------------------------------------------------
// 線分と四角形の衝突を調べる
//-----------------------------------------------------------------------------
bool Collision::testIntersectLineAndSquare( const Vector2& start, const Vector2& end, const Vector2& sq0, const Vector2& sq1, const Vector2& sq2, const Vector2& sq3, Vector2* out ){
	Vector2 cross;
	bool hit = false;
	if ( out != NULL ) *out = end;
	if ( testIntersectLine( start, end, sq0, sq1, &cross ) ){
		if ( out != NULL ) *out = cross;
		hit = true;
	}
	if ( testIntersectLine( start, end, sq1, sq2, &cross ) ){
		// 交点が今までより始点に近ければ保存
		if ( out != NULL && ( *out - start ).sqLength() > ( cross - start ).sqLength() ){
			*out = cross;
		}
		hit = true;
	}
	if ( testIntersectLine( start, end, sq2, sq3, &cross ) ){
		// 交点が今までより始点に近ければ保存
		if ( out != NULL && ( *out - start ).sqLength() > ( cross - start ).sqLength() ){
			*out = cross;
		}
		hit = true;
	}
	if ( testIntersectLine( start, end, sq3, sq0, &cross ) ){
		// 交点が今までより始点に近ければ保存
		if ( out != NULL && ( *out - start ).sqLength() > ( cross - start ).sqLength() ){
			*out = cross;
		}
		hit = true;
	}

	if ( hit ){
		return true;
	}else{
		out = NULL;
		return false;
	}
}

//-----------------------------------------------------------------------------
// 線分と四角形の衝突を調べる
//-----------------------------------------------------------------------------
bool Collision::testIntersectLineAndSquare( const Vector2& start, const Vector2& end, const Vector2& sqPos, float sqWidth, float sqHeight, float sqAngle, Vector2* out ){
	Vector2 sqVertex[ 4 ];
	Matrix34 rota;
	rota.setRotationZ( sqAngle );
	Vector2 pos( 0, 0 );

	for ( int i = 0; i < 4; i++ ){
		switch ( i ){
			case 0 :
				pos.x = -sqWidth / 2;
				pos.y = -sqHeight / 2;
				break;
			case 1 :
				pos.x = -sqWidth / 2;
				pos.y = sqHeight / 2;
				break;
			case 2 :
				pos.x = sqWidth / 2;
				pos.y = sqHeight / 2;
				break;
			case 3 :
				pos.x = sqWidth / 2;
				pos.y = -sqHeight / 2;
				break;
		}

		pos *= rota;
		sqVertex[ i ].set( pos.x + sqPos.x, pos.y + sqPos.y );
	}
	
	return testIntersectLineAndSquare( start, end, sqVertex[ 0 ], sqVertex[ 1 ], sqVertex[ 2 ], sqVertex[ 3 ], out );
}

bool Collision::testIntersectLineAndSquare(
const cocos2d::CCPoint& lineStart,
const cocos2d::CCPoint& lineEnd,
const cocos2d::CCPoint& squarePosition,
float squareWidth,
float squareHeight,
float squareAngle,
cocos2d::CCPoint* out
){
	Vector2 start( lineStart.x, lineStart.y );
	Vector2 end( lineEnd.x, lineEnd.y );
	Vector2 sqPos( squarePosition.x, squarePosition.y );
	Vector2 vOut;
	if ( testIntersectLineAndSquare( start, end, sqPos, squareWidth, squareHeight, squareAngle, &vOut ) ){
		*out = ccp( vOut.x, vOut.y );
		return true;
	}

	out = NULL;
	return false;

}

//-----------------------------------------------------------------------------
// 線分と多角形の衝突を調べる
//-----------------------------------------------------------------------------
bool Collision::testIntersectLineAndPolygon( const Vector2& start, const Vector2& end, const Vector2& position, const Vector2* vertices, int length, Vector2* out ){
	Vector2 cross;
	bool hit = false;
	if ( out != NULL ) *out = end;

	for ( int i = 0; i < length; i++ ){
		int sqEndIndex = ( i == length - 1 ) ? 0 : i + 1;
		if ( testIntersectLine( start, end, vertices[ i ], vertices[ sqEndIndex ], &cross ) ){
			// 交点が今までより始点に近ければ保存
			if ( out != NULL && ( *out - start ).sqLength() > ( cross - start ).sqLength() ){
				*out = cross;
			}
			hit = true;
		}
	}

	if ( hit ){
		return true;
	}else{
		out = NULL;
		return false;
	}
}

//-----------------------------------------------------------------------------
// 線分と多角形の衝突を調べる
//-----------------------------------------------------------------------------
bool Collision::testIntersectLineAndPolygon( const Vector2& start, const Vector2& end, const Vector2& position, const std::vector<Vector2> vertices, Vector2* out ){
	Vector2 cross;
	bool hit = false;
	if ( out != NULL ) *out = end;

	for ( int i = 0; i < vertices.size(); i++ ){
		int sqEndIndex = ( i == vertices.size() - 1 ) ? 0 : i + 1;
		if ( testIntersectLine( start, end, position + vertices[ i ], position + vertices[ sqEndIndex ], &cross ) ){
			// 交点が今までより始点に近ければ保存
			if ( out != NULL && ( *out - start ).sqLength() > ( cross - start ).sqLength() ){
				*out = cross;
			}
			hit = true;
		}
	}

	if ( hit ){
		return true;
	}else{
		out = NULL;
		return false;
	}
}


bool Collision::testIntersectSquare(
const Vector2& sqPosA, float sqWidthA, float sqHeightA, float sqAngleA,
const Vector2& sqPosB, float sqWidthB, float sqHeightB, float sqAngleB
){
	Vector2 sqVertex[ 4 ];
	Matrix34 rota;
	rota.setRotationZ( sqAngleA );
	Vector2 pos( 0, 0 );

	for ( int i = 0; i < 4; i++ ){
		switch ( i ){
			case 0 :
				pos.x = -sqWidthA / 2;
				pos.y = -sqHeightA / 2;
				break;
			case 1 :
				pos.x = -sqWidthA / 2;
				pos.y = sqHeightA / 2;
				break;
			case 2 :
				pos.x = sqWidthA / 2;
				pos.y = sqHeightA / 2;
				break;
			case 3 :
				pos.x = sqWidthA / 2;
				pos.y = -sqHeightA / 2;
				break;
		}

		pos *= rota;
		sqVertex[ i ].set( pos.x + sqPosA.x, pos.y + sqPosA.y );
	}
	
	if ( testIntersectLineAndSquare( sqVertex[ 0 ], sqVertex[ 1 ], sqPosB, sqWidthB, sqHeightB, sqAngleB ) ){
		return true;
	}
	if ( testIntersectLineAndSquare( sqVertex[ 1 ], sqVertex[ 2 ], sqPosB, sqWidthB, sqHeightB, sqAngleB ) ){
		return true;
	}
	if ( testIntersectLineAndSquare( sqVertex[ 2 ], sqVertex[ 3 ], sqPosB, sqWidthB, sqHeightB, sqAngleB ) ){
		return true;
	}
	if ( testIntersectLineAndSquare( sqVertex[ 3 ], sqVertex[ 0 ], sqPosB, sqWidthB, sqHeightB, sqAngleB ) ){
		return true;
	}

	return false;
}


bool Collision::testIntersectSquare(
const cocos2d::CCPoint& sqPosA, float sqWidthA, float sqHeightA, float sqAngleA,
const cocos2d::CCPoint& sqPosB, float sqWidthB, float sqHeightB, float sqAngleB
){
	Vector2 posA( sqPosA.x, sqPosA.y );
	Vector2 posB( sqPosB.x, sqPosB.y );

	return ( testIntersectSquare( posA, sqWidthA, sqHeightA, sqAngleA, posB, sqWidthB, sqHeightB, sqAngleB ) );
}

} // namespace GameLib