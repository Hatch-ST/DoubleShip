#ifndef INCLUDED_GAMELIB_VECTOR2I_H
#define INCLUDED_GAMELIB_VECTOR2I_H

namespace GameLib{

class Vector2;

class Vector2i{
public :
	Vector2i();
	Vector2i( int x, int y );
	Vector2i( float x, float y );
	Vector2i( const Vector2i& value );

	void set( int x, int y );
	void set( float x, float y );
	void normalize();
	void setAdd( const Vector2i& a, const Vector2i& b );

	void setZero();

	void operator=( const Vector2i& value );
	void operator+=( const Vector2i& value );
	void operator-=( const Vector2i& value );
	void operator*=( int value );
	void operator/=( int value );

	Vector2i operator*( float value ) const;
	Vector2i operator+( const Vector2i& value ) const;
	Vector2i operator+( const Vector2& value ) const;
	Vector2i operator-( const Vector2i& value ) const;

	double sqLength();
	float length();

	static float dot( const Vector2i& a, const Vector2i& b );

	int x;
	int y;
};

} // namespace GameLib

#endif