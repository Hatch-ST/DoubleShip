#ifndef INCLUDED_GAMELIB_VECTOR2_H
#define INCLUDED_GAMELIB_VECTOR2_H

namespace GameLib{

class Matrix34;
class Vector2i;

class Vector2{
public :
	Vector2();
	Vector2( int x, int y );
	Vector2( float x, float y );
	Vector2( const Vector2& x );
	void set( int x, int y );
	void set( float x, float y );
	void normalize();
	void setAdd( const Vector2& a, const Vector2& b );

	void setZero();
	void operator=( const Vector2& );
	void operator+=( const Vector2& );
	void operator-=( const Vector2& );
	void operator*=( float value );
	void operator*=( const Matrix34& );
	Vector2 operator*( float value ) const;
	Vector2 operator+( const Vector2& ) const;
	Vector2 operator+( const Vector2i& ) const;
	Vector2 operator-( const Vector2& ) const;
	void operator/=( float value );
	double sqLength();
	float length();

	static float dot( const Vector2& a, const Vector2& b );
	static Vector2 normalize( const Vector2& value );

	float x;
	float y;

	int ix() const;
	int iy() const;
};

} // namespace GameLib

#endif