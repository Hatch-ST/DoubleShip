#ifndef INCLUDED_GAMELIB_COLLISION_H
#define INCLUDED_GAMELIB_COLLISION_H

#include "cocos2d.h"
#include "Delete.h"
#include <vector>

namespace GameLib{
	
class Vector2;

class Collision{
public :
	// 座標と四角形の衝突を調べる
	static bool testIntersectPositionAndSquare( const Vector2& position, const Vector2& sqLeftUp, const Vector2& sqRightDown );
	
	// 座標と三角形の衝突を調べる
	static bool testIntersectPositionAndTriangle(
		const Vector2& position,
		const Vector2& triangle0,
		const Vector2& triangle1,
		const Vector2& triangle2
	);
	
	// 座標と四角形の衝突を調べる
	static bool testIntersectPositionAndSquare(
		const Vector2& position,
		const Vector2& square0,
		const Vector2& square1,
		const Vector2& square2,
		const Vector2& square3
	);
	
	// 座標と四角形の衝突を調べる
	static bool testIntersectPositionAndSquare(
		const Vector2& position,
		const Vector2& squarePosition,
		float squareWidth,
		float squareHeight,
		float squareAngle
	);

	static bool testIntersectPositionAndSquare(
		const cocos2d::CCPoint& position,
		const cocos2d::CCPoint& squarePosition,
		float squareWidth,
		float squareHeight,
		float squareAngle
	);

	// 線分同士の衝突を調べる
	static bool testIntersectLine( const Vector2& a0, const Vector2& a1, const Vector2& b0, const Vector2& b1, Vector2* out = NULL );
	
	// 線分と三角形の衝突を調べる
	static bool testIntersectLineAndTriangle(
		const Vector2& lineStart,
		const Vector2& lineEnd,
		const Vector2& triangle0,
		const Vector2& triangle1,
		const Vector2& triangle2,
		Vector2* out = NULL
	);
	
	// 線分と四角形の衝突を調べる
	static bool testIntersectLineAndSquare(
		const Vector2& lineStart,
		const Vector2& lineEnd,
		const Vector2& square0,
		const Vector2& square1,
		const Vector2& square2,
		const Vector2& square3,
		Vector2* out = NULL
	);
	
	static bool testIntersectLineAndSquare(
		const Vector2& lineStart,
		const Vector2& lineEnd,
		const Vector2& squarePosition,
		float squareWidth,
		float squareHeight,
		float squareAngle,
		Vector2* out = NULL
	);

	static bool testIntersectLineAndSquare(
		const cocos2d::CCPoint& lineStart,
		const cocos2d::CCPoint& lineEnd,
		const cocos2d::CCPoint& squarePosition,
		float squareWidth,
		float squareHeight,
		float squareAngle,
		cocos2d::CCPoint* out = NULL
	);
	
	static bool testIntersectLineAndPolygon(
		const Vector2& lineStart,
		const Vector2& lineEnd,
		const Vector2& position,
		const Vector2* vertices,
		int length,
		Vector2* out = NULL
	);

	static bool testIntersectLineAndPolygon(
		const Vector2& lineStart,
		const Vector2& lineEnd,
		const Vector2& position,
		const std::vector<Vector2> vertices,
		Vector2* out = NULL
	);
	
	static bool testIntersectSquare(
		const Vector2& sqPosA, float sqWidthA, float sqHeightA, float sqAngleA,
		const Vector2& sqPosB, float sqWidthB, float sqHeightB, float sqAngleB
	);

	static bool testIntersectSquare(
		const cocos2d::CCPoint& sqPosA, float sqWidthA, float sqHeightA, float sqAngleA,
		const cocos2d::CCPoint& sqPosB, float sqWidthB, float sqHeightB, float sqAngleB
	);
};

} // namespace GameLib

#endif