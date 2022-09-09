#include "../precompiled.h"


void fCamera2::Zoom( float new_zoom, fVec2 &pos ) {
	float m1		= zoom;
	float m2		= new_zoom;
	zoom			= new_zoom;

	fVec2 center	= proj.GetCenter( );
	fVec2 trans		= pos - ( pos - center ) * m1 / m2 - center;

	Translate( trans );
}


fVec2 fCamera2::GetModelPositionAt( int win_x, int win_y ) {
	float mod_w = proj[1].x - proj[0].x;
	float mod_h = proj[1].y - proj[0].y;
	float win_w = mod_w / aspect;
	float win_h = mod_h / aspect;

	float shift_x = float( win_x ) * mod_w / win_w / zoom;
	float shift_y = float( win_h - win_y ) * mod_h / win_h / zoom;
	float x11	= 0.5f * ( proj[1].x + proj[0].x ) - 0.5f / zoom * ( proj[1].x - proj[0].x );
	float y11	= 0.5f * ( proj[1].y + proj[0].y ) - 0.5f / zoom * ( proj[1].y - proj[0].y );

	float x = x11 + shift_x;
	float y = y11 + shift_y;
	
	return fVec2( x, y );
}


float fCamera2::GetModelSize( int size ) {
	float mod_w = proj[1].x - proj[0].x;
	float win_w = mod_w / aspect;

	return float( size ) * mod_w / win_w / zoom;
}