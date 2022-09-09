#include "../precompiled.h"





fNewMeshDialog::fNewMeshDialog( QWidget *parent ) : QDialog( parent ) {
	gbMeshType			= CreateMeshTypeGroupBox( );
	wTrussMeshPanel		= CreateTrussMeshPanel( );
	wQuardMeshPanel		= CreateContinuumMeshPanel( );

	swMeshDetails		= new QStackedWidget( );
	swMeshDetails ->addWidget( wTrussMeshPanel );
	swMeshDetails ->addWidget( wQuardMeshPanel );

	connect( bgMeshType, SIGNAL( buttonClicked( int ) ), this, SLOT( ChangeMeshTypePanel( int ) ) );

	
	dbbConfirmation		= new QDialogButtonBox( QDialogButtonBox::Ok | QDialogButtonBox::Cancel );
    connect( dbbConfirmation, SIGNAL(accepted()), this, SLOT(accept()) );
    connect( dbbConfirmation, SIGNAL(rejected()), this, SLOT(reject()) );

	QHBoxLayout *h_layout = new QHBoxLayout( );
    h_layout ->addWidget( gbMeshType );
    h_layout ->addWidget( swMeshDetails );

	QVBoxLayout *v_layout = new QVBoxLayout( );
	v_layout ->addLayout( h_layout );
	v_layout ->addWidget( dbbConfirmation );

	setLayout( v_layout );
	setWindowTitle( tr("New Mesh") );
    resize( 500, 290 );

	ReadSettings( );
}

fNewMeshDialog::~fNewMeshDialog(  ) {
	WriteSettings( );
}


QGroupBox *fNewMeshDialog::CreateMeshTypeGroupBox( ) {
	QGroupBox *gbox			= new QGroupBox( tr("Mesh type") );

	QRadioButton *rad_truss	= new QRadioButton( tr("Truss mesh") );
	QRadioButton *rad_cont	= new QRadioButton( tr("Continuum mesh") );
	bgMeshType				= new QButtonGroup( );
	bgMeshType	->addButton( rad_truss, MT_TRUSS );
	bgMeshType	->addButton( rad_cont,  MT_CONTINUUM );
	rad_truss	->setChecked( true );

	QVBoxLayout *vbox		= new QVBoxLayout;
	vbox ->addSpacing( 3 );
	vbox ->addWidget( rad_truss );
	vbox ->addWidget( rad_cont );
	vbox ->addStretch( 1 );

	gbox ->setLayout( vbox );

	return gbox;
}


QWidget *fNewMeshDialog::CreateTrussMeshPanel(  ) {
	QWidget *widget					= new QWidget( );

	// Create all elements
	leTrussMeshSizeX				= new QLineEdit( );
	leTrussMeshSizeY				= new QLineEdit( );
	sbTrussMeshNodeNumX				= new QSpinBox( );
	sbTrussMeshNodeNumY				= new QSpinBox( );
	cbTrussMeshConnecType			= new QComboBox( );
	sbAllConnectSize				= new QSpinBox( );

	// Set initial values
	leTrussMeshSizeX ->setText( tr("10.0") );
	leTrussMeshSizeY ->setText( tr("10.0") );
	sbTrussMeshNodeNumX ->setValue( 11 );
	sbTrussMeshNodeNumY ->setValue( 11 );
	sbTrussMeshNodeNumX ->setRange( 1, 1000000 );
	sbTrussMeshNodeNumY ->setRange( 1, 1000000 );
	sbAllConnectSize	->setRange( 0, 100 );
	sbAllConnectSize	->setSuffix( "%");

	// Setup validators
	leTrussMeshSizeX ->setValidator( new QDoubleValidator( leTrussMeshSizeX ) );
	leTrussMeshSizeY ->setValidator( new QDoubleValidator( leTrussMeshSizeY ) );

	cbTrussMeshConnecType ->addItem( tr("Neighbor nodes") );
	cbTrussMeshConnecType ->addItem( tr("All nodes") );

	// Create mesh size group 
	QGroupBox *		gbox_mesh_size		= new QGroupBox( tr("Structure size") );
	QFormLayout *	mesh_size_layout	= new QFormLayout( );
	mesh_size_layout ->addRow( tr("Width:"), leTrussMeshSizeX );
	mesh_size_layout ->addRow( tr("Height:"), leTrussMeshSizeY );
	gbox_mesh_size ->setLayout( mesh_size_layout );

	// Create nodes group
	QGroupBox *		gbox_mesh_nodes		= new QGroupBox( tr("Number of nodes") );
	QFormLayout *	mesh_node_layout	= new QFormLayout( );
	mesh_node_layout ->addRow( tr("X direction:"), sbTrussMeshNodeNumX );
	mesh_node_layout ->addRow( tr("Y direction:"), sbTrussMeshNodeNumY );
	gbox_mesh_nodes ->setLayout( mesh_node_layout );


	QLabel *l_connect = new QLabel(tr("Connectivity:"));
	l_connect ->setBuddy( cbTrussMeshConnecType );

	lConSize = new QLabel(tr("Within specific radius"));
	lConSize ->setBuddy( sbAllConnectSize );

	QGridLayout *gl_con_type = new QGridLayout;
	gl_con_type ->addWidget( l_connect, 0, 0 );
	gl_con_type ->addWidget( cbTrussMeshConnecType, 0, 1 );
	gl_con_type ->addWidget( lConSize, 1, 0 );
	gl_con_type ->addWidget( sbAllConnectSize, 1, 1 );

	QVBoxLayout *v_layout	= new QVBoxLayout( );
	v_layout ->addWidget( gbox_mesh_size );
	v_layout ->addWidget( gbox_mesh_nodes );
	v_layout ->addSpacing( 8 );
	v_layout ->addLayout( gl_con_type );
	v_layout ->addStretch( 1 );
	v_layout ->setMargin( 0 );

	widget ->setLayout( v_layout );

	connect( cbTrussMeshConnecType, SIGNAL( currentIndexChanged ( int ) ), this, SLOT( OnConnectivityChanged( int ) ) );
	sbAllConnectSize ->setVisible( false );
	lConSize ->setVisible( false );

	return widget;
}


QWidget *fNewMeshDialog::CreateContinuumMeshPanel(  ) {
	QWidget *widget			= new QWidget( );

	// Create all elements
	leQuardMeshSizeX				= new QLineEdit( );
	leQuardMeshSizeY				= new QLineEdit( );
	sbQuardMeshElemNumX				= new QSpinBox( );
	sbQuardMeshElemNumY				= new QSpinBox( );

	// Set initial values
	leQuardMeshSizeX ->setText( tr( "10.0" ) );
	leQuardMeshSizeY ->setText( tr( "10.0" ) );
	sbQuardMeshElemNumX ->setValue( 10 );
	sbQuardMeshElemNumY ->setValue( 10 );
	sbQuardMeshElemNumX ->setRange( 1, 1000000 );
	sbQuardMeshElemNumY ->setRange( 1, 1000000 );


	// Setup validators
	leTrussMeshSizeX ->setValidator( new QDoubleValidator( leTrussMeshSizeX ) );
	leTrussMeshSizeY ->setValidator( new QDoubleValidator( leTrussMeshSizeY ) );

	// Create mesh size group 
	QGroupBox *		gbox_mesh_size		= new QGroupBox( tr("Structure size") );
	QFormLayout *	mesh_size_layout	= new QFormLayout( );
	mesh_size_layout ->addRow( tr("Width:"),  leQuardMeshSizeX );
	mesh_size_layout ->addRow( tr("Height:"), leQuardMeshSizeY );
	gbox_mesh_size ->setLayout( mesh_size_layout );

	// Create nodes group
	QGroupBox *		gbox_mesh_nodes		= new QGroupBox( tr("Number of elements") );
	QFormLayout *	mesh_node_layout	= new QFormLayout( );
	mesh_node_layout ->addRow( tr("X direction:"), sbQuardMeshElemNumX );
	mesh_node_layout ->addRow( tr("Y direction:"), sbQuardMeshElemNumY );
	gbox_mesh_nodes ->setLayout( mesh_node_layout );

	QVBoxLayout *v_layout	= new QVBoxLayout( );
	v_layout ->addWidget( gbox_mesh_size );
	v_layout ->addWidget( gbox_mesh_nodes );
	v_layout ->addStretch( 1 );
	v_layout ->setMargin( 0 );

	widget ->setLayout( v_layout );

	return widget;
}


void fNewMeshDialog::ChangeMeshTypePanel( int id  ) {
	swMeshDetails ->setCurrentIndex( id );
	if ( id != 0 ) {
		dbbConfirmation ->button( QDialogButtonBox::Ok ) ->setEnabled( true );
	} else {
		dbbConfirmation ->button( QDialogButtonBox::Ok ) ->setEnabled( true );
	}
}

void fNewMeshDialog::OnConnectivityChanged( int i ) {
	if ( i == 0 ) {
		sbAllConnectSize ->setVisible( false );
		lConSize ->setVisible( false );
	} else {
		sbAllConnectSize ->setVisible( true );
		lConSize ->setVisible( true );
	}
}



void fNewMeshDialog::ReadSettings( ) {
	// Get application settings
	QSettings * settings = app ->GetSettings( );
	// Create group
	settings ->beginGroup( "new_mesh_dialog" );
		settings ->beginGroup( "truss_mesh" );
			leTrussMeshSizeX		->setText( settings ->value( "size_x", 100.0 ).toString( ) );
			leTrussMeshSizeY		->setText( settings ->value( "size_y", 10.0 ).toString( ) );
			sbTrussMeshNodeNumX		->setValue( settings ->value( "node_num_x", 51 ).toInt( ) );
			sbTrussMeshNodeNumY		->setValue( settings ->value( "node_num_y", 6 ).toInt( ) );
			cbTrussMeshConnecType	->setCurrentIndex( settings ->value( "connectivity", 0 ).toInt( ) );
			sbAllConnectSize		->setValue( settings ->value( "connect_all_to_all_size", 10 ).toInt( ) );
		settings ->endGroup( );

		settings ->beginGroup( "quad_mesh" );
			leQuardMeshSizeX		->setText( settings ->value( "size_x", 100.0 ).toString( ) );
			leQuardMeshSizeY		->setText( settings ->value( "size_y", 10.0 ).toString( ) );
			sbQuardMeshElemNumX		->setValue( settings ->value( "elem_num_x", 50 ).toInt( ) );
			sbQuardMeshElemNumY		->setValue( settings ->value( "elem_num_y", 5 ).toInt( ) );
		settings ->endGroup( );

		QString structure_type = settings ->value( "structure_type", "truss" ).toString( );
		if ( structure_type == tr("truss") ) {
			bgMeshType ->button( MT_TRUSS ) ->setChecked( true );
			ChangeMeshTypePanel( MT_TRUSS );
		} else {
			bgMeshType ->button( MT_CONTINUUM ) ->setChecked( true );
			ChangeMeshTypePanel( MT_CONTINUUM );
		}

	settings ->endGroup( );

}

void fNewMeshDialog::WriteSettings( ) {
	QSettings * settings = app ->GetSettings( );

	settings ->beginGroup( "new_mesh_dialog" );
		settings ->beginGroup( "truss_mesh" );
			settings ->setValue( "size_x", leTrussMeshSizeX ->text( ).toDouble( ) );
			settings ->setValue( "size_y", leTrussMeshSizeY ->text( ).toDouble( ) );
			settings ->setValue( "node_num_x", sbTrussMeshNodeNumX ->value( ) );
			settings ->setValue( "node_num_y", sbTrussMeshNodeNumY ->value( ) );
			settings ->setValue( "connectivity", cbTrussMeshConnecType ->currentIndex( ) );
			settings ->setValue( "connect_all_to_all_size", sbAllConnectSize ->value( ) );
		settings ->endGroup( );

		settings ->beginGroup( "quad_mesh" );
			settings ->setValue( "size_x", leQuardMeshSizeX ->text( ).toDouble( ) );
			settings ->setValue( "size_y", leQuardMeshSizeY ->text( ).toDouble( ) );
			settings ->setValue( "elem_num_x", sbQuardMeshElemNumX ->value( ) );
			settings ->setValue( "elem_num_y", sbQuardMeshElemNumY ->value( ) );
		settings ->endGroup( );

		bool truss_structure = bgMeshType ->button( MT_TRUSS ) ->isChecked( );
		if ( truss_structure ) {
			settings ->setValue( "structure_type", "truss" );
		} else {
			settings ->setValue( "structure_type", "continuum" );
		}

	settings ->endGroup( );
}


fMeshType_e fNewMeshDialog::ReadMeshType( ) {
	switch ( bgMeshType ->checkedId( ) ) {
		case 0: {
			return MT_TRUSS;
		}

		case 1: {
			return MT_CONTINUUM;
		}
	}

	return MT_UNKNOWN;
}


double fNewMeshDialog::ReadTrussMeshSizeX( ) {
	return leTrussMeshSizeX ->text( ).toDouble( );
}


double fNewMeshDialog::ReadTrussMeshSizeY( ) {
	return leTrussMeshSizeY ->text( ).toDouble( );
}


int fNewMeshDialog::ReadTrussMeshNodeNumX( ) {
	return sbTrussMeshNodeNumX ->value( );
}


int fNewMeshDialog::ReadTrussMeshNodeNumY( ) {
	return sbTrussMeshNodeNumY ->value( );
}


int fNewMeshDialog::ReadTrussMeshConnectivitySize( ) {
	return sbAllConnectSize ->value( );
}





fTrussMeshConnectivity_e fNewMeshDialog::ReadTrussMeshConnectivity( ) {
	switch ( cbTrussMeshConnecType ->currentIndex( ) ) {
		case 0: {
			return TMC_NEIGHBOR_NODES;
		}

		case 1: {
			return TMC_ALL_NODES;
		}
	}

	return TMC_UNKNOWN;
}




double fNewMeshDialog::ReadQuardMeshSizeX( ) {
	return leQuardMeshSizeX ->text( ).toDouble( );
}


double fNewMeshDialog::ReadQuardMeshSizeY( ) {
	return leQuardMeshSizeY ->text( ).toDouble( );
}


int fNewMeshDialog::ReadQuardMeshElemNumX( ) {
	return sbQuardMeshElemNumX ->value( );
}


int fNewMeshDialog::ReadQuardMeshElemNumY( ) {
	return sbQuardMeshElemNumY ->value( );
}
