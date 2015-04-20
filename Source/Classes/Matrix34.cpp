#include "Matrix34.h"
#include "GVector2.h"
#include <math.h>

namespace GameLib{

Matrix34::Matrix34(){
}

void Matrix34::setTranslation( float x, float y, float z ){
	m00 = m11 = m22 = 1.0f;
	m01 = m10 = m20 = m02 = m12 = m21 = 0.0f;
	m03 = x;
	m13 = y;
	m23 = z;
}

void Matrix34::setScaling( float x, float y, float z ){
	m00 = x;
	m11 = y;
	m22 = z;
	m01 = m10 = m02 = m20 = m12 = m21 = 0.0f;
	m03 = m13 = m23 = 0.0f;
}

void Matrix34::setRotationX( float radian ){
	float c = cos( radian );
	float s = sin( radian );
	m11 = m22 = c;
	m12 = -s;
	m21 = s;
	m00 = 1.0f;
	m01 = m02 = m10 = m20 = 0.0f;
	m03 = m13 = m23 = 0.0f;
}

void Matrix34::setRotationY( float radian ){
	float c = cos( radian );
	float s = sin( radian );
	m00 = m22 = c;
	m20 = -s;
	m02 = s;
	m11 = 1.0f;
	m01 = m10 = m12 = m21 = 0.0f;
	m03 = m13 = m23 = 0.0f;
}

void Matrix34::setRotationZ( float radian ){
	float c = cos( radian );
	float s = sin( radian );
	m00 = m11 = c;
	m01 = -s;
	m10 = s;
	m22 = 1.0f;
	m02 = m12 = m20 = m21 = 0.0f;
	m03 = m13 = m23 = 0.0f;
}

void Matrix34::mul( Vector2* out, const Vector2& in ) const{
	float tx = in.x; 
	float ty = in.y; 
	out->x = m00 * tx + m01 * ty + m02 * m03;
	out->y = m10 * tx + m11 * ty + m12 * m13;
}

void Matrix34::operator*=( const Matrix34& m ){
	float tx, ty, tz;
	tx = m00; ty = m01; tz = m02;
	m00 = tx * m.m00 + ty * m.m10 + tz * m.m20;
	m01 = tx * m.m01 + ty * m.m11 + tz * m.m21;
	m02 = tx * m.m02 + ty * m.m12 + tz * m.m22;
	m03 = tx * m.m03 + ty * m.m13 + tz * m.m23 + m03;

	tx = m10; ty = m11; tz = m12;
	m10 = tx * m.m00 + ty * m.m10 + tz * m.m20;
	m11 = tx * m.m01 + ty * m.m11 + tz * m.m21;
	m12 = tx * m.m02 + ty * m.m12 + tz * m.m22;
	m13 = tx * m.m03 + ty * m.m13 + tz * m.m23 + m13;

	tx = m20; ty = m21; tz = m22;
	m20 = tx * m.m00 + ty * m.m10 + tz * m.m20;
	m21 = tx * m.m01 + ty * m.m11 + tz * m.m21;
	m22 = tx * m.m02 + ty * m.m12 + tz * m.m22;
	m23 = tx * m.m03 + ty * m.m13 + tz * m.m23 + m23;
}

}