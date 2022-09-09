#ifndef __MATH_H__
#define __MATH_H__


class fMath {
public:
	static bool							IntersectCircleSegment( fVec2 &c, float r, fVec2 &p1, fVec2 &p2 );

};

#endif//__MATH_H__




inline bool fMath::IntersectCircleSegment( fVec2 &c, float r, fVec2 &p1, fVec2 &p2 ) {
    fVec2 dir	= p2 - p1;
    fVec2 diff	= c - p1;
    float t		= ( diff * dir ) / ( dir * dir );

	if ( t < 0.0f ) {
		t = 0.0f;
	}
    if ( t > 1.0f ) {
		t = 1.0f;
	}

    fVec2 closest	= p1 + t * dir;
    fVec2 d			= c - closest;
    float dist_sqr	= d * d;

    return ( dist_sqr <= r * r );
}