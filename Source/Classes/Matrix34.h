#ifndef INCLUDED_MATRIX34_H
#define INCLUDED_MATRIX34_H

namespace GameLib{

class Vector2;

class Matrix34{
public :
	Matrix34();

	void setTranslation( float x, float y, float z );
	void setScaling( float x, float y, float z );
	void setRotationX( float radian );
	void setRotationY( float radian );
	void setRotationZ( float radian );

	void mul( Vector2* out, const Vector2& in ) const;

	void operator*=( const Matrix34& );

private :
	float m00, m01, m02, m03;
	float m10, m11, m12, m13;
	float m20, m21, m22, m23;
};

} // namespace GameLib

#endif