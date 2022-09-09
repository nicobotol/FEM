#ifndef __BV_BOUNDS_H__
#define __BV_BOUNDS_H__


#include <float.h>
#include "Vector.h"

/*
===============================================================================

	Axis Aligned Bounding Box

===============================================================================
*/

class fBounds2 {
public:
					fBounds2( void );
					explicit fBounds2( const fVec2 &mins, const fVec2 &maxs );
					explicit fBounds2( const fVec2 &point );

	const fVec2 &	operator[]( const int index ) const;
	fVec2 &			operator[]( const int index );
	fBounds2		operator+( const fVec2 &t ) const;				// returns translated bounds
	fBounds2 &		operator+=( const fVec2 &t );					// translate the bounds
	fBounds2		operator+( const fBounds2 &a ) const;
	fBounds2 &		operator+=( const fBounds2 &a );
	fBounds2		operator-( const fBounds2 &a ) const;
	fBounds2 &		operator-=( const fBounds2 &a );

	bool			Compare( const fBounds2 &a ) const;								// exact compare, no epsilon
	bool			Compare( const fBounds2 &a, const float epsilon ) const;		// compare with epsilon
	bool			operator==(	const fBounds2 &a ) const;							// exact compare, no epsilon
	bool			operator!=(	const fBounds2 &a ) const;							// exact compare, no epsilon

	void			Clear( void );									// inside out bounds
	void			Zero( void );									// single point at origin

	fVec2			GetCenter( void ) const;						// returns center of bounds
	float			GetRadius( void ) const;						// returns the radius relative to the bounds origin
	float			GetRadius( const fVec2 &center ) const;			// returns the radius relative to the given center
	float			GetArea( void ) const;							// returns the volume of the bounds
	bool			IsCleared( void ) const;						// returns true if bounds are inside out

	bool			AddPoint( const fVec2 &v );						// add the point, returns true if the bounds expanded
	bool			AddBounds( const fBounds2 &a );					// add the bounds, returns true if the bounds expanded
	fBounds2		Intersect( const fBounds2 &a ) const;			// return intersection of this bounds with the given bounds
	fBounds2 &		IntersectSelf( const fBounds2 &a );				// intersect this bounds with the given bounds
	fBounds2		Expand( const float d ) const;					// return bounds expanded in all directions with the given value
	fBounds2 &		ExpandSelf( const float d );					// expand bounds in all directions with the given value
	fBounds2		Translate( const fVec2 &translation ) const;	// return translated bounds
	fBounds2 &		TranslateSelf( const fVec2 &translation );		// translate this bounds

	fBounds2		Scale( const float d ) const;					// return scaled bounds
	fBounds2 &		ScaleSelf( const float d );						// scale this bounds

	bool			ContainsPoint( const fVec2 &p ) const;			// includes touching
	bool			IntersectsBounds( const fBounds2 &a ) const;	// includes touching
	bool			LineIntersection( const fVec2 &start, const fVec2 &end ) const;
					// intersection point is start + dir * scale
	bool			RayIntersection( const fVec2 &start, const fVec2 &dir, float &scale ) const;

					// most tight bounds for a point set
	void			FromPoints( const fVec2 *points, const int numPoints );

	void			ToPoints( fVec2 points[4] ) const;

private:
	fVec2			b[2];
};

extern fBounds2	bounds_zero;

inline fBounds2::fBounds2( void ) {
}

inline fBounds2::fBounds2( const fVec2 &mins, const fVec2 &maxs ) {
	b[0] = mins;
	b[1] = maxs;
}

inline fBounds2::fBounds2( const fVec2 &point ) {
	b[0] = point;
	b[1] = point;
}

inline const fVec2 &fBounds2::operator[]( const int index ) const {
	return b[index];
}

inline fVec2 &fBounds2::operator[]( const int index ) {
	return b[index];
}

inline fBounds2 fBounds2::operator+( const fVec2 &t ) const {
	return fBounds2( b[0] + t, b[1] + t );
}

inline fBounds2 &fBounds2::operator+=( const fVec2 &t ) {
	b[0] += t;
	b[1] += t;
	return *this;
}

inline fBounds2 fBounds2::operator+( const fBounds2 &a ) const {
	fBounds2 newBounds;
	newBounds = *this;
	newBounds.AddBounds( a );
	return newBounds;
}

inline fBounds2 &fBounds2::operator+=( const fBounds2 &a ) {
	fBounds2::AddBounds( a );
	return *this;
}

inline fBounds2 fBounds2::operator-( const fBounds2 &a ) const {
	Q_ASSERT( b[1][0] - b[0][0] > a.b[1][0] - a.b[0][0] &&
				b[1][1] - b[0][1] > a.b[1][1] - a.b[0][1] );
	return fBounds2( fVec2( b[0][0] + a.b[1][0], b[0][1] + a.b[1][1] ),
					fVec2( b[1][0] + a.b[0][0], b[1][1] + a.b[0][1] ) );
}

inline fBounds2 &fBounds2::operator-=( const fBounds2 &a ) {
	Q_ASSERT( b[1][0] - b[0][0] > a.b[1][0] - a.b[0][0] &&
				b[1][1] - b[0][1] > a.b[1][1] - a.b[0][1] );
	b[0] += a.b[1];
	b[1] += a.b[0];
	return *this;
}

inline bool fBounds2::Compare( const fBounds2 &a ) const {
	return ( b[0].Compare( a.b[0] ) && b[1].Compare( a.b[1] ) );
}

inline bool fBounds2::Compare( const fBounds2 &a, const float epsilon ) const {
	return ( b[0].Compare( a.b[0], epsilon ) && b[1].Compare( a.b[1], epsilon ) );
}

inline bool fBounds2::operator==( const fBounds2 &a ) const {
	return Compare( a );
}

inline bool fBounds2::operator!=( const fBounds2 &a ) const {
	return !Compare( a );
}

inline void fBounds2::Clear( void ) {
	b[0][0] = b[0][1] = +FLT_MAX;
	b[1][0] = b[1][1] = -FLT_MAX;
}

inline void fBounds2::Zero( void ) {
	b[0][0] = b[0][1] =
	b[1][0] = b[1][1] = 0.0;
}

inline fVec2 fBounds2::GetCenter( void ) const {
	return fVec2( ( b[1][0] + b[0][0] ) * 0.5f, ( b[1][1] + b[0][1] ) * 0.5f );
}

inline float fBounds2::GetArea( void ) const {
	if ( b[0][0] >= b[1][0] || b[0][1] >= b[1][1] ) {
		return 0.0f;
	}
	return ( ( b[1][0] - b[0][0] ) * ( b[1][1] - b[0][1] ) );
}

inline bool fBounds2::IsCleared( void ) const {
	return b[0][0] > b[1][0];
}

inline bool fBounds2::AddPoint( const fVec2 &v ) {
	bool expanded = false;
	if ( v[0] < b[0][0]) {
		b[0][0] = v[0];
		expanded = true;
	}
	if ( v[0] > b[1][0]) {
		b[1][0] = v[0];
		expanded = true;
	}
	if ( v[1] < b[0][1] ) {
		b[0][1] = v[1];
		expanded = true;
	}
	if ( v[1] > b[1][1]) {
		b[1][1] = v[1];
		expanded = true;
	}
	return expanded;
}

inline bool fBounds2::AddBounds( const fBounds2 &a ) {
	bool expanded = false;
	if ( a.b[0][0] < b[0][0] ) {
		b[0][0] = a.b[0][0];
		expanded = true;
	}
	if ( a.b[0][1] < b[0][1] ) {
		b[0][1] = a.b[0][1];
		expanded = true;
	}
	if ( a.b[1][0] > b[1][0] ) {
		b[1][0] = a.b[1][0];
		expanded = true;
	}
	if ( a.b[1][1] > b[1][1] ) {
		b[1][1] = a.b[1][1];
		expanded = true;
	}
	return expanded;
}

inline fBounds2 fBounds2::Intersect( const fBounds2 &a ) const {
	fBounds2 n;
	n.b[0][0] = ( a.b[0][0] > b[0][0] ) ? a.b[0][0] : b[0][0];
	n.b[0][1] = ( a.b[0][1] > b[0][1] ) ? a.b[0][1] : b[0][1];
	n.b[1][0] = ( a.b[1][0] < b[1][0] ) ? a.b[1][0] : b[1][0];
	n.b[1][1] = ( a.b[1][1] < b[1][1] ) ? a.b[1][1] : b[1][1];
	return n;
}

inline fBounds2 &fBounds2::IntersectSelf( const fBounds2 &a ) {
	if ( a.b[0][0] > b[0][0] ) {
		b[0][0] = a.b[0][0];
	}
	if ( a.b[0][1] > b[0][1] ) {
		b[0][1] = a.b[0][1];
	}
	if ( a.b[1][0] < b[1][0] ) {
		b[1][0] = a.b[1][0];
	}
	if ( a.b[1][1] < b[1][1] ) {
		b[1][1] = a.b[1][1];
	}
	return *this;
}

inline fBounds2 fBounds2::Expand( const float d ) const {
	return fBounds2( fVec2( b[0][0] - d, b[0][1] - d ),
						fVec2( b[1][0] + d, b[1][1] + d ) );
}

inline fBounds2 &fBounds2::ExpandSelf( const float d ) {
	b[0][0] -= d;
	b[0][1] -= d;
	b[1][0] += d;
	b[1][1] += d;
	return *this;
}

inline fBounds2 fBounds2::Scale( const float d ) const {
	float w_half_d = 0.5f * ( b[1].x - b[0].x ) * ( d - 1.0f );
	float h_half_d = 0.5f * ( b[1].y - b[0].y ) * ( d - 1.0f );

	return fBounds2( fVec2( b[0][0] - w_half_d, b[0][1] - h_half_d ),
					 fVec2( b[1][0] + w_half_d, b[1][1] + h_half_d ) );
}

inline fBounds2 &fBounds2::ScaleSelf( const float d ) {
	float w_half_d = 0.5f * ( b[1].x - b[0].x ) * ( d - 1.0f );
	float h_half_d = 0.5f * ( b[1].y - b[0].y ) * ( d - 1.0f );

	b[0][0] -= w_half_d;
	b[0][1] -= h_half_d;
	b[1][0] += w_half_d;
	b[1][1] += h_half_d;
	return *this;
}


inline fBounds2 fBounds2::Translate( const fVec2 &translation ) const {
	return fBounds2( b[0] + translation, b[1] + translation );
}

inline fBounds2 &fBounds2::TranslateSelf( const fVec2 &translation ) {
	b[0] += translation;
	b[1] += translation;
	return *this;
}

inline bool fBounds2::ContainsPoint( const fVec2 &p ) const {
	if ( p[0] < b[0][0] || p[1] < b[0][1] || p[0] > b[1][0] || p[1] > b[1][1] ) {
		return false;
	}
	return true;
}

inline bool fBounds2::IntersectsBounds( const fBounds2 &a ) const {
	if ( a.b[1][0] < b[0][0] || a.b[1][1] < b[0][1] || a.b[0][0] > b[1][0] || a.b[0][1] > b[1][1] ) {
		return false;
	}
	return true;
}



#endif /* !__BV_BOUNDS_H__ */
