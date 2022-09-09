#include "precompiled.h"


fApplication::fApplication( int & argc, char ** argv ) : QApplication( argc, argv ) {
	wndMain			= NULL;
	settings		= NULL;
	mesh			= NULL;
	brush			= NULL;

	srand( QTime::currentTime( ).second( ) );

	status			= APPS_PREPARE;
}


void fApplication::Initialize( ) {
	settings		= new QSettings( "settings.ini", QSettings::IniFormat );

	ReadSettings( );

	QApplication::setStyle( styleName );

	brush			= new fBrush( );
	wndMain			= new fMainWindow( );
	wndMain ->show( );
}


void fApplication::SetMesh( fMesh *new_mesh ) {
	if ( mesh ) {
		delete mesh;
	}
	mesh = new_mesh;
}


fApplication::~fApplication() {
	if ( wndMain ) {
		delete wndMain;
	}
	if ( mesh ) {
		delete mesh;
	}
	if ( brush ) {
		delete brush;
	}

	WriteSettings( );

	if ( settings ) {
		delete settings;
	}
}


void fApplication::Error( const QString &msg ) {
	QMessageBox msgBox( QMessageBox::Warning, tr("Phazor Vision Error"), msg, 0, wndMain );
    msgBox.addButton(tr("Ok"), QMessageBox::AcceptRole);
	msgBox.exec( );
}


void fApplication::Warning( const QString &msg ) {
	Error( msg );
}


void fApplication::ReadSettings( ) {
	// Get application settings
	QSettings * settings = GetSettings( );

	settings ->beginGroup( "application" );
		styleName				= settings ->value( "style", "default" ).toString( );
		nativeDialogs			= settings ->value( "use_native_dialogs", false ).toBool( );
		pointColorFloatFormat	= settings ->value( "point_color_float_format", false ).toBool( );

	settings ->endGroup( );
}


void fApplication::WriteSettings( ) {
	QSettings * settings = GetSettings( );

	settings ->beginGroup( "application" );
		settings ->setValue( "style", styleName );
		settings ->setValue( "use_native_dialogs", nativeDialogs );
		settings ->setValue( "point_color_float_format", pointColorFloatFormat );
	settings ->endGroup( );
}


QString fApplication::GetName( ) const {
	return QString( "FlExtract" );
}


QString fApplication::GetVersion( ) const {
	return QString( "v1.13" );
}