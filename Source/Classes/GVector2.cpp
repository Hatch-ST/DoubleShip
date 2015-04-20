#include "GVector2.h"
#include "GVector2i.h"
#include "Matrix34.h"
#include <math.h>

namespace GameLib{

Vector2::Vector2(){
	x = y = 0;
}

Vector2::Vector2( int sX, int sY ){
	x = static_cast< float >( sX );
	y = static_cast< float >( sY );
}

Vector2::Vector2( float sX, float sY ){
	x = sX;
	y = sY;
}

Vector2::Vector2( const Vector2& vec ){
	x = vec.x;
	y = vec.y;
}

void Vector2::set( int sX, int sY ){
	x = static_cast< float >( sX );
	y = static_cast< float >( sY );
}

void Vector2::set( float sX, float sY ){
	x = sX;
	y = sY;
}

void Vector2::setAdd( const Vector2& a, const Vector2& b ){
	x = a.x + b.x;
	y = a.y + b.y;
}

void Vector2::normalize(){
	float length = sqrt( static_cast< float >( sqLength() ) );
	*this *= 1.0f / length;
}

double Vector2::sqLength(){
	return static_cast< double >( x * x + y * y );
}

float Vector2::length(){
	return static_cast<float>( sqrt( sqLength() ) );
}

void Vector2::setZero(){
	set( 0, 0 );
}

int Vector2::ix() const{
	return static_cast<int>( x );
}

int Vector2::iy() const{
	return static_cast<int>( y );
}

void Vector2::operator=( const Vector2& vec ){
	x = vec.x;
	y = vec.y;
}

void Vector2::operator+=( const Vector2& vec ){
	x += vec.x;
	y += vec.y;
}
void Vector2::operator-=( const Vector2& vec ){
	x -= vec.x;
	y -= vec.y;
}

void Vector2::operator*=( float n ){
	x *= n;
	y *= n;
}

void Vector2::operator*=( const Matrix34& m ){
	m.mul( this, *this );
}

Vector2 Vector2::operator*( float n ) const{
	Vector2 out( x * n, y * n );
	return out;
}

Vector2 Vector2::operator+( const Vector2& vec ) const{
	Vector2 out( x + vec.x, y + vec.y );
	return out;
}

Vector2 Vector2::operator+( const Vector2i& vec ) const{
	Vector2 out( x + vec.x, y + vec.y );
	return out;
}

Vector2 Vector2::operator-( const Vector2& vec ) const{
	Vector2 out( x - vec.x, y - vec.y );
	return out;
}

void Vector2::operator/=( float n ){
	x /= n;
	y /= n;
}

float Vector2::dot( const Vector2& a, const Vector2& b ){
	return a.x * b.x + a.y * b.y;
}

Vector2 Vector2::normalize( const Vector2& value ){
	Vector2 out = value;
	out.normalize();
	return out;
}

} // namespace GameLib