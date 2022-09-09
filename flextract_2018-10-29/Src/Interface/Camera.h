#ifndef __CAMERA_H__
#define __CAMERA_H__


#include "../Base/Vector.h"
#include "../Base/Bounds.h"


class fCamera2 {
public:
									fCamera2( );
									~fCamera2( );

	void							Move( fVec2 &pos );
	void							Translate( fVec2 &trans );

	void							SetProjection( fBounds2 &bounds );
	fBounds2						GetProjection( );
	float							GetAspect( )								{ return aspect; };

	fVec2							GetModelPositionAt( int x, int y );
	float							GetModelSize( int size );

	float							GetZoom( )									{ return zoom; };
	void							ResetZoom( )								{ zoom = 1.0f; };
	void							Zoom( float new_zoom, fVec2 &pos );

	void							SetupViewport( int width, int height, fBounds2 &bounds );
	void							UpdateViewport( int new_width, int new_height );

private:
	fBounds2						proj;

	int								viewWidth;
	int								viewHeight;
	float							aspect;

	float							zoom;
	fVec2							translation;
};



inline fCamera2::fCamera2( ) {
	proj[0].x = proj[0].y = -1.0f;
	proj[1].x = proj[1].y = +1.0f;

	zoom		= 1.0f;
	translation	= fVec2( 0.0f, 0.0f );
}


inline fCamera2::~fCamera2( ) {
	
}


inline void fCamera2::SetProjection( fBounds2 &bounds ) {
	proj = bounds;
}


inline void fCamera2::Move( fVec2 &pos ) {
	fVec2 c = proj.GetCenter( );
	proj.TranslateSelf( pos - c );
}


inline void fCamera2::Translate( fVec2 &trans ) {
	proj.TranslateSelf( trans );
	translation += trans;
}


inline void fCamera2::SetupViewport( int width, int height, fBounds2 &bounds ) {
	viewWidth	= width;
	viewHeight	= height;

	SetProjection( bounds );
	
	aspect			= ( bounds[1].x - bounds[0].x ) / float( width );
	zoom			= 1.0f;
	translation		= fVec2( 0.0f, 0.0f );
}

inline void fCamera2::UpdateViewport( int new_width, int new_height ) {
	// Change the projection according to the changes in window size
	float dx	= float( new_width  - viewWidth )  * aspect;
	float dy	= float( new_height - viewHeight ) * aspect;

	proj[0].y -= 0.5f * dy;
	proj[1].y += 0.5f * dy;
	proj[1].x += 0.5f * dx;
	proj[0].x -= 0.5f * dx;

	viewWidth	= new_width;
	viewHeight	= new_height;
}



inline fBounds2 fCamera2::GetProjection( ) {
	fBounds2 bounds;

	fVec2 center	= proj.GetCenter( );
	float x_half	= ( proj[1].x - proj[0].x ) / 2.0f;
	float y_half	= ( proj[1].y - proj[0].y ) / 2.0f;

	bounds[0].x		= center.x - x_half / zoom;
	bounds[1].x		= center.x + x_half / zoom;
	bounds[0].y		= center.y - y_half / zoom;
	bounds[1].y		= center.y + y_half / zoom;

	return bounds;
}


#endif//__CAMERA_H__