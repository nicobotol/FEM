#include "../precompiled.h"

fBounds2 bounds_zero( vec2_zero, vec2_zero );


/*
============
fBounds2::GetRadius
============
*/
float fBounds2::GetRadius( void ) const {
	int		i;
	float	total, b0, b1;

	total = 0.0f;
	for ( i = 0; i < 3; i++ ) {
		b0 = (float)fabs( b[0][i] );
		b1 = (float)fabs( b[1][i] );
		if ( b0 > b1 ) {
			total += b0 * b0;
		} else {
			total += b1 * b1;
		}
	}
	return sqrt( total );
}

/*
============
fBounds2::GetRadius
============
*/
float fBounds2::GetRadius( const fVec2 &center ) const {
	int		i;
	float	total, b0, b1;

	total = 0.0f;
	for ( i = 0; i < 2; i++ ) {
		b0 = (float)fabs( center[i] - b[0][i] );
		b1 = (float)fabs( b[1][i] - center[i] );
		if ( b0 > b1 ) {
			total += b0 * b0;
		} else {
			total += b1 * b1;
		}
	}
	return sqrt( total );
}

/*
============
fBounds2::LineIntersection

  Returns true if the line intersects the bounds between the start and end point.
============
*/
bool fBounds2::LineIntersection( const fVec2 &start, const fVec2 &end ) const {
    float ld[3];
	fVec2 center = ( b[0] + b[1] ) * 0.5;
	fVec2 extents = b[1] - center;
    fVec2 lineDir = 0.5 * ( end - start );
    fVec2 lineCenter = start + lineDir;
    fVec2 dir = lineCenter - center;

    ld[0] = fabs( lineDir[0] );
	if ( fabs( dir[0] ) > extents[0] + ld[0] ) {
        return false;
	}

    ld[1] = fabs( lineDir[1] );
	if ( fabs( dir[1] ) > extents[1] + ld[1] ) {
        return false;
	}

    return true;
}

/*
============
fBounds2::RayIntersection

  Returns true if the ray intersects the bounds.
  The ray can intersect the bounds in both directions from the start point.
  If start is inside the bounds it is considered an intersection with scale = 0
============
*/
bool fBounds2::RayIntersection( const fVec2 &start, const fVec2 &dir, float &scale ) const {
	int i, ax0, ax1, ax2, side, inside;
	float f;
	fVec2 hit;

	ax0 = -1;
	inside = 0;
	for ( i = 0; i < 2; i++ ) {
		if ( start[i] < b[0][i] ) {
			side = 0;
		}
		else if ( start[i] > b[1][i] ) {
			side = 1;
		}
		else {
			inside++;
			continue;
		}
		if ( dir[i] == 0.0f ) {
			continue;
		}
		f = ( start[i] - b[side][i] );
		if ( ax0 < 0 || fabs( f ) > fabs( scale * dir[i] ) ) {
			scale = - ( f / dir[i] );
			ax0 = i;
		}
	}

	if ( ax0 < 0 ) {
		scale = 0.0f;
		// return true if the start point is inside the bounds
		return ( inside == 2 );
	}

	ax1 = (ax0+1)%2;
	ax2 = (ax0+2)%2;
	hit[ax1] = start[ax1] + scale * dir[ax1];
	hit[ax2] = start[ax2] + scale * dir[ax2];

	return ( hit[ax1] >= b[0][ax1] && hit[ax1] <= b[1][ax1] &&
				hit[ax2] >= b[0][ax2] && hit[ax2] <= b[1][ax2] );
}

/*
============
fBounds2::FromPoints

  Most tight bounds for a point set.
============
*/
void fBounds2::FromPoints( const fVec2 *points, const int numPoints ) {
	if (numPoints <= 0) return;

	float min_x = points[0].x;
	float max_x = points[0].x;
	float min_y = points[0].y;
	float max_y = points[0].y;
	
	for (int i = 1; i < numPoints; i++) {
		if (points[i].x < min_x) {
			min_x = points[i].x;
		} else if (points[i].x > max_x) {
			max_x = points[i].x;
		}

		if (points[i].y < min_y) {
			min_y = points[i].y;
		} else if (points[i].y > max_y) {
			max_y = points[i].y;
		}
	}
	b[0].x = min_x;
	b[0].y = min_y;
	b[1].x = max_x;
	b[1].y = max_y;
}

/*
============
fBounds2::ToPoints
============
*/
void fBounds2::ToPoints( fVec2 points[4] ) const {
	for ( int i = 0; i < 4; i++ ) {
		points[i][0] = b[(i^(i>>1))&1][0];
		points[i][1] = b[(i>>1)&1][1];
	}
}
