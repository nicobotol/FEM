// Copyright (C) Yuriy Elesin 2009
//

#include "precompiled.h"

fVec2 vec2_origin( 0.0f, 0.0f );


//===============================================================
//
//	idVec2
//
//===============================================================


/*=============
Lerp

Linearly inperpolates one vector to another.
=============*/
void fVec2::Lerp( const fVec2 &v1, const fVec2 &v2, const float l ) {
	if ( l <= 0.0f ) {
		(*this) = v1;
	} else if ( l >= 1.0f ) {
		(*this) = v2;
	} else {
		(*this) = v1 + l * ( v2 - v1 );
	}
}
