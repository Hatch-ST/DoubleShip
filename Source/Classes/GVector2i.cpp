#include "GVector2i.h"
#include "GVector2.h"
#include <math.h>

namespace GameLib{

Vector2i::Vector2i(){
	x = y = 0;
}

Vector2i::Vector2i( int sX, int sY ){
	x = sX;
	y = sY;
}

Vector2i::Vector2i( float sX, float sY ){
	x = static_cast< int >( sX );
	y = static_cast< int >( sY );
}

Vector2i::Vector2i( const Vector2i& value ){
	x = value.x;
	y = value.y;
}

void Vector2i::set( int sX, int sY ){
	x = sX;
	y = sY;
}

void Vector2i::set( float sX, float sY ){
	x = static_cast< int >( sX );
	y = static_cast< int >( sY );
}

void Vector2i::setAdd( const Vector2i& a, const Vector2i& b ){
	x = a.x + b.x;
	y = a.y + b.y;
}

void Vector2i::normalize(){
	float length = sqrt( static_cast< float >( sqLength() ) );
	*this *= 1.0f / length;
}

double Vector2i::sqLength(){
	return static_cast< double >( x * x + y * y );
}

float Vector2i::length(){
	return static_cast<float>( sqrt( sqLength() ) );
}

void Vector2i::setZero(){
	set( 0, 0 );
}

void Vector2i::operator=( const Vector2i& vec ){
	x = vec.x;
	y = vec.y;
}

void Vector2i::operator+=( const Vector2i& vec ){
	x += vec.x;
	y += vec.y;
}

void Vector2i::operator-=( const Vector2i& vec ){
	x -= vec.x;
	y -= vec.y;
}

void Vector2i::operator*=( int n ){
	x *= n;
	y *= n;
}

void Vector2i::operator/=( int n ){
	x /= n;
	y /= n;
}

Vector2i Vector2i::operator*( float n ) const{
	Vector2i out( x * n, y * n );
	return out;
}

Vector2i Vector2i::operator+( const Vector2i& vec ) const{
	Vector2i out( x + vec.x, y + vec.y );
	return out;
}

Vector2i Vector2i::operator+( const Vector2& vec ) const{
	Vector2i out( x + vec.x, y + vec.y );
	return out;
}

Vector2i Vector2i::operator-( const Vector2i& vec ) const{
	Vector2i out( x - vec.x, y - vec.y );
	return out;
}

} // namespace GameLib