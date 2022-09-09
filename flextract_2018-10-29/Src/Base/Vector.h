#ifndef VEC_H
#define VEC_H


#include <math.h>


class fVec2 {
public:
	float			x;
	float			y;

					fVec2( void );
					explicit fVec2( const float x, const float y );

	void 			Set( const float x, const float y );
	void			Zero( void );

	float			operator[]( int index ) const;
	float &			operator[]( int index );
	fVec2			operator-() const;
	float			operator*( const fVec2 &a ) const;
	fVec2			operator*( const float a ) const;
	fVec2			operator/( const float a ) const;
	fVec2			operator+( const fVec2 &a ) const;
	fVec2			operator-( const fVec2 &a ) const;
	fVec2 &			operator+=( const fVec2 &a );
	fVec2 &			operator-=( const fVec2 &a );
	fVec2 &			operator/=( const fVec2 &a );
	fVec2 &			operator/=( const float a );
	fVec2 &			operator*=( const float a );

	friend fVec2	operator*( const float a, const fVec2 b );

	bool			Compare( const fVec2 &a ) const;							// exact compare, no epsilon
	bool			Compare( const fVec2 &a, const float epsilon ) const;		// compare with epsilon
	bool			operator==(	const fVec2 &a ) const;						// exact compare, no epsilon
	bool			operator!=(	const fVec2 &a ) const;						// exact compare, no epsilon

	float			Length( void ) const;
	float			LengthFast( void ) const;
	float			LengthSqr( void ) const;
	float			Normalize( void );			// returns length
	float			NormalizeFast( void );		// returns length
	fVec2 &			Truncate( float length );	// cap length
	void			Clamp( const fVec2 &min, const fVec2 &max );
	void			Snap( void );				// snap to closest integer value
	void			SnapInt( void );			// snap towards integer (floor)

	int				GetDimension( void ) const;

	const float *	ToFloatPtr( void ) const;
	float *			ToFloatPtr( void );

	void			Lerp( const fVec2 &v1, const fVec2 &v2, const float l );
};

extern	fVec2		vec2_origin;
#define vec2_zero	vec2_origin

inline fVec2::fVec2( void ) {
}

inline fVec2::fVec2( const float x, const float y ) {
	this->x = x;
	this->y = y;
}

inline void fVec2::Set( const float x, const float y ) {
	this->x = x;
	this->y = y;
}

inline void fVec2::Zero( void ) {
	x = y = 0.0f;
}

inline bool fVec2::Compare( const fVec2 &a ) const {
	return ( ( x == a.x ) && ( y == a.y ) );
}

inline bool fVec2::Compare( const fVec2 &a, const float epsilon ) const {
	if ( fabs( x - a.x ) > epsilon ) {
		return false;
	}
			
	if ( fabs( y - a.y ) > epsilon ) {
		return false;
	}

	return true;
}

inline bool fVec2::operator==( const fVec2 &a ) const {
	return Compare( a );
}

inline bool fVec2::operator!=( const fVec2 &a ) const {
	return !Compare( a );
}

inline float fVec2::operator[]( int index ) const {
	return ( &x )[ index ];
}

inline float& fVec2::operator[]( int index ) {
	return ( &x )[ index ];
}

inline float fVec2::Length( void ) const {
	return ( float )sqrt( x * x + y * y );
}

inline float fVec2::LengthSqr( void ) const {
	return ( x * x + y * y );
}

inline float fVec2::Normalize( void ) {
	float sqrLength, invLength;

	sqrLength = x * x + y * y;
	invLength = 1.0f / sqrt( sqrLength );
	x *= invLength;
	y *= invLength;
	return invLength * sqrLength;
}

inline fVec2 &fVec2::Truncate( float length ) {
	float length2;
	float ilength;

	if ( !length ) {
		Zero();
	}
	else {
		length2 = LengthSqr();
		if ( length2 > length * length ) {
			ilength = length * 1.0f / sqrt( length2 );
			x *= ilength;
			y *= ilength;
		}
	}

	return *this;
}

inline void fVec2::Clamp( const fVec2 &min, const fVec2 &max ) {
	if ( x < min.x ) {
		x = min.x;
	} else if ( x > max.x ) {
		x = max.x;
	}
	if ( y < min.y ) {
		y = min.y;
	} else if ( y > max.y ) {
		y = max.y;
	}
}

inline void fVec2::Snap( void ) {
	x = floor( x + 0.5f );
	y = floor( y + 0.5f );
}

inline void fVec2::SnapInt( void ) {
	x = float( int( x ) );
	y = float( int( y ) );
}

inline fVec2 fVec2::operator-() const {
	return fVec2( -x, -y );
}
	
inline fVec2 fVec2::operator-( const fVec2 &a ) const {
	return fVec2( x - a.x, y - a.y );
}

inline float fVec2::operator*( const fVec2 &a ) const {
	return x * a.x + y * a.y;
}

inline fVec2 fVec2::operator*( const float a ) const {
	return fVec2( x * a, y * a );
}

inline fVec2 fVec2::operator/( const float a ) const {
	float inva = 1.0f / a;
	return fVec2( x * inva, y * inva );
}

inline fVec2 operator*( const float a, const fVec2 b ) {
	return fVec2( b.x * a, b.y * a );
}

inline fVec2 fVec2::operator+( const fVec2 &a ) const {
	return fVec2( x + a.x, y + a.y );
}

inline fVec2 &fVec2::operator+=( const fVec2 &a ) {
	x += a.x;
	y += a.y;

	return *this;
}

inline fVec2 &fVec2::operator/=( const fVec2 &a ) {
	x /= a.x;
	y /= a.y;

	return *this;
}

inline fVec2 &fVec2::operator/=( const float a ) {
	float inva = 1.0f / a;
	x *= inva;
	y *= inva;

	return *this;
}

inline fVec2 &fVec2::operator-=( const fVec2 &a ) {
	x -= a.x;
	y -= a.y;

	return *this;
}

inline fVec2 &fVec2::operator*=( const float a ) {
	x *= a;
	y *= a;

	return *this;
}

inline int fVec2::GetDimension( void ) const {
	return 2;
}

inline const float *fVec2::ToFloatPtr( void ) const {
	return &x;
}

inline float *fVec2::ToFloatPtr( void ) {
	return &x;
}

#endif //!VEC_H
