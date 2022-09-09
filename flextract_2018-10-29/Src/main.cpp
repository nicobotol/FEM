#include "precompiled.h"

fApplication *			app;


int main( int argc, char *argv[] ) {
	app = new fApplication( argc, argv );
	app ->Initialize();

	int res = app ->exec( );

	delete app;

	return res;
}