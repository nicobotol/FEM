#ifndef __APPLICATION_H__
#define __APPLICATION_H__


class fMainWindow;


enum fAppStatus_e {
	APPS_PREPARE					= 0,
	APPS_EDIT_MESH					= 1,
};




class fApplication : public QApplication {
public:
									fApplication( int & argc, char ** argv );
	virtual							~fApplication( );

	void							Initialize( );

	void							Error( const QString &msg );
	void							Warning( const QString &msg );

	fMainWindow *					GetMainWindow( ) const						{ return wndMain; };
	QSettings *						GetSettings( ) const						{ return settings; };

	QString							GetName( ) const;
	QString							GetVersion( ) const;

	fMesh *							GetMesh( ) const							{ return mesh; };
	void							SetMesh( fMesh *new_mesh );

	fAppStatus_e					GetStatus( ) const							{ return status; };
	void							SetStatus( fAppStatus_e status )			{ this ->status = status; };

	fBrush *						GetBrush( ) const							{ return brush; };

	bool							UseNativeDialogs( )							{ return nativeDialogs; };
	bool							IsPointColorFloatFormat( )					{ return pointColorFloatFormat; };

private:
	void							ReadSettings( );
	void							WriteSettings( );

	fMainWindow *					wndMain;
	QSettings *						settings;

	fMesh *							mesh;
	fBrush *						brush;

	fAppStatus_e					status;


	QString							styleName;
	bool							nativeDialogs;
	bool							pointColorFloatFormat;
};



extern fApplication *				app;


#endif//__APPLICATION_H__
