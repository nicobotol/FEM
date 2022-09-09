#include "../precompiled.h"


fBrush::fBrush( ) : QObject( ) {
	ReadSettings( );
}


fBrush::~fBrush( ) {
	WriteSettings( );
}

void fBrush::SetDiameter( int d ) {
	if ( d > maxDiameter ) {
		d = maxDiameter;
	}
	if ( d < minDiameter ) {
		d = minDiameter;
	}
	if ( d != this ->diameter ) {
		this ->diameter = d;
		emit sizeChanged( diameter );
	}
}


void fBrush::SetMinDiameter( int min ) {
	this ->minDiameter = min;
	if ( minDiameter > maxDiameter ) {
		minDiameter = maxDiameter;
		diameter = minDiameter;
		emit sizeChanged( diameter );
	}

	if ( minDiameter > diameter ) {
		diameter = minDiameter;
		emit sizeChanged( diameter );
	}
}


void fBrush::SetMaxDiameter( int max ) {
	this ->maxDiameter = max;
	if ( maxDiameter < minDiameter ) {
		maxDiameter = minDiameter;
		diameter = maxDiameter;
		emit sizeChanged( diameter );
	}

	if ( maxDiameter < diameter ) {
		diameter = maxDiameter;
		emit sizeChanged( diameter );
	}
}


void fBrush::SetHardness( int hardness ) {
	if ( hardness <= 100 && hardness >= 0 && hardness != this ->hardness ) {
		this ->hardness = hardness;
		emit hardnessChanged( hardness );
	}
}

void fBrush::SetInclusiveState( bool state ) {
	if ( state != inclusive ) {
		inclusive = state;
		emit inclusiveStateChanged( inclusive );
	}
}


void fBrush::OnBrushSizeChange( int d ) {
	SetDiameter( d );
}


void fBrush::OnBrushHardnessChange( int h ) {
	SetHardness( h );
}

void fBrush::OnInclusiveStateChange( bool state ) {
	SetInclusiveState( state );
}


void fBrush::ReadSettings( ) {
	// Get application settings
	QSettings * settings = app ->GetSettings( );

	settings ->beginGroup( "brush" );
		color		= settings ->value( "color", QColor( 255, 255, 255 ) ).value<QColor>( );
		diameter	= settings ->value( "diameter", 200 ).toInt( );
		minDiameter	= settings ->value( "min_diameter", 1 ).toInt( );
		maxDiameter	= settings ->value( "max_diameter", 256 ).toInt( );
		hardness	= settings ->value( "hardness", 0.0f ).toFloat( );
		inclusive	= settings ->value( "inclusive", false ).toBool( );
	settings ->endGroup( );
}

void fBrush::WriteSettings( ) {
	QSettings * settings = app ->GetSettings( );

	settings ->beginGroup( "brush" );
		settings ->setValue( "color", color );
		settings ->setValue( "diameter", diameter );
		settings ->setValue( "min_diameter", minDiameter );
		settings ->setValue( "max_diameter", maxDiameter );
		settings ->setValue( "hardness", hardness );
		settings ->setValue( "inclusive", inclusive );
	settings ->endGroup( );
}