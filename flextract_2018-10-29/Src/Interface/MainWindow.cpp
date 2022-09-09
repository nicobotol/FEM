#include "../precompiled.h"


fMainWindow::fMainWindow( QWidget *parent ) : QMainWindow( parent ) {
	canvas						= new fCanvas( );
	navigator					= new QWidget( );

	tmpTrussMaterials			= NULL;
	tndpNodeDisps				= NULL;
	tnlpNodeLoads				= NULL;
	tnlpNodeLoads				= NULL;

	qmpQuardMaterials			= NULL;
	qndpNodeDisps				= NULL;
	qnlpNodeLoads				= NULL;
	qflpFaceLoads				= NULL;

	navigator	->setMinimumSize( 220, 100 );
	canvas		->setMinimumSize( 220, 100 );

	// Calculate the default position and size of the main window
	int		def_win_width		= 800;
	int		def_win_height		= 500;
	QPoint	def_pos				= QPoint( app ->desktop( ) ->availableGeometry( ).width( )/2, app ->desktop( ) ->availableGeometry( ).height( )/2 );
			def_pos				= def_pos - QPoint( def_win_width / 2, def_win_height / 2 );

	// Move and resize the main window
	this ->move( def_pos );
	this ->resize( def_win_width, def_win_height );

	dwNavigator	= new QDockWidget(tr("Navigator"));
	dwNavigator ->setWidget( navigator );
	this ->addDockWidget( Qt::RightDockWidgetArea, dwNavigator );

	// Set canvas as central widget
	this ->setCentralWidget( canvas );

	CreateActions( );
	CreateMenus( );
	CreateToolBars( );
	AdjustMenus( );

	// Insert status bar widgets
	/*
	lElemNum	= new QLabel( "Number of elements: ", statusBar() );
	lNodeNum	= new QLabel( "Number of nodes: ", statusBar() );
	lTotalVol	= new QLabel( "Total volume: ", statusBar() );
	statusBar( ) ->addPermanentWidget( lElemNum, 1 );
	statusBar( ) ->addPermanentWidget( lNodeNum, 1 );
	statusBar( ) ->addPermanentWidget( lTotalVol, 1 );
	*/

	statusBar( ) ->showMessage( tr("Ready"), 1000 );

	setTabPosition( Qt::AllDockWidgetAreas, QTabWidget::North );

	ReadSettings( );
}


fMainWindow::~fMainWindow( ) {
	
}


void fMainWindow::CreateActions( ) {
	actNewMesh = new QAction( tr("&New Mesh"), this );
	actNewMesh ->setShortcuts( QKeySequence::New );
	actNewMesh ->setStatusTip( tr("Create new mesh") );
	connect( actNewMesh, SIGNAL( triggered( ) ), this, SLOT( OnNewMesh( ) ) );

	actSaveMesh = new QAction( tr("&Save Mesh"), this );
	actSaveMesh	->setShortcuts( QKeySequence::Save );
	connect( actSaveMesh, SIGNAL( triggered( ) ), this, SLOT( OnSaveMesh( ) ) );

	actExit = new QAction( tr("E&xit"), this );
	actExit	->setShortcut( tr("Ctrl+Q") );
	actExit	->setStatusTip( tr("Exit the program") );
	connect( actExit, SIGNAL( triggered( ) ), this, SLOT( OnExit( ) ) );

	actAbout = new QAction( tr("&About"), this );
	actAbout ->setStatusTip( tr("About FlexStruct") );
	connect( actAbout, SIGNAL( triggered( ) ), this, SLOT( OnAbout( ) ) );

	actCenterMesh = new QAction( tr("&Fit to Screen"), this );
	actCenterMesh ->setShortcut( tr("Ctrl+0") );
	actCenterMesh ->setStatusTip( tr("Fit the mesh to the screen") );
	connect( actCenterMesh, SIGNAL( triggered( ) ), this, SLOT( OnCenterMesh( ) ) );


	// Edit truss actions ---------------------------------------------------------
	QAction *act;

	// Does nothing
	act = new QAction( tr("Selection Mode"), this );
	act ->setCheckable( true );
	actTrussEdit.selection = act;
	connect( act, SIGNAL( triggered( ) ), this, SLOT( OnTrussEditSelection( ) ) );

	// Apply materials to elements
	act = new QAction( tr("Set Element Materials"), this );
	act ->setStatusTip( tr("Apply material to mesh elements") );
	act ->setCheckable( true );
	actTrussEdit.paintElemSetMaterial = act;
	connect( act, SIGNAL( triggered( ) ), this, SLOT( OnTrussEditSetElementMaterials( ) ) );

	// 
	act = new QAction( tr("Set Nodal Displacements"), this );
	act ->setCheckable( true );
	actTrussEdit.paintNodeSetDisplacements = act;
	connect( act, SIGNAL( triggered( ) ), this, SLOT( OnTrussEditSetNodalDiplacements( ) ) );

	// 
	act = new QAction( tr("Set Nodal Loads"), this );
	act ->setCheckable( true );
	actTrussEdit.paintNodeSetForces = act;
	connect( act, SIGNAL( triggered( ) ), this, SLOT( OnTrussEditSetNodalForces( ) ) );

	// Add/remove truss element
	act = new QAction( tr("Remove Elements"), this );
	act ->setStatusTip( tr("Remove elements from mesh") );
	act ->setCheckable( true );
	actTrussEdit.paintElemRemove = act;
	connect( act, SIGNAL( triggered( ) ), this, SLOT( OnTrussEditRemoveElements( ) ) );

	// Add/remove nodes
	act = new QAction( tr("Remove Nodes"), this );
	act ->setStatusTip( tr("Remove nodes with all adjacent elements from mesh") );
	act ->setCheckable( true );
	actTrussEdit.paintNodeRemove = act;
	connect( act, SIGNAL( triggered( ) ), this, SLOT( OnTrussEditRemoveNodes( ) ) );

	// Move (adjust node positions)
	act = new QAction( tr("Move Nodes"), this );
	act ->setStatusTip( tr("Edit node positions") );
	act ->setCheckable( true );
	actTrussEdit.paintNodeMove = act;
	connect( act, SIGNAL( triggered( ) ), this, SLOT( OnTrussEditMoveNodes( ) ) );

	act = new QAction( tr("Remove Elements With Material..."), this );
	act ->setStatusTip( tr("Remove elements with specific material from mesh") );
	act ->setCheckable( false );
	actTrussEdit.removeElementsWithMaterial = act;
	connect( act, SIGNAL( triggered( ) ), this, SLOT( OnTrussEditRemoveElementsWithSpecificMaterial( ) ) );


	// Create action group
	QActionGroup * act_g = new QActionGroup( this );
	act_g ->addAction( actTrussEdit.selection );
	act_g ->addAction( actTrussEdit.paintElemSetMaterial );
	act_g ->addAction( actTrussEdit.paintNodeSetDisplacements );
	act_g ->addAction( actTrussEdit.paintNodeSetForces );
	act_g ->addAction( actTrussEdit.paintElemRemove );
	act_g ->addAction( actTrussEdit.paintNodeRemove );
	act_g ->addAction( actTrussEdit.paintNodeMove );
	actTrussEdit.group = act_g;

	// Set initially active action
	actTrussEdit.selection ->setChecked( true );

	// Connect actions
	connect( actTrussEdit.group, SIGNAL( triggered( QAction * ) ), this, SLOT( OnTrussEditChange( QAction * ) ) );


	// Edit quard actions ---------------------------------------------------------
	// Does nothing
	act = new QAction( tr("Selection Mode"), this );
	act ->setShortcut( tr("F2") );
	act ->setCheckable( true );
	actQuardEdit.selection = act;
	connect( act, SIGNAL( triggered( ) ), this, SLOT( OnQuardEditSelection( ) ) );

	// Apply materials to elements
	act = new QAction( tr("Set Element Materials"), this );
	act ->setStatusTip( tr("Apply material to mesh elements") );
	act ->setShortcut( tr("F5") );
	act ->setCheckable( true );
	actQuardEdit.paintElemSetMaterial = act;
	connect( act, SIGNAL( triggered( ) ), this, SLOT( OnQuardEditSetElementMaterials( ) ) );

	// 
	act = new QAction( tr("Set Nodal Displacements"), this );
	act ->setShortcut( tr("F6") );
	act ->setCheckable( true );
	actQuardEdit.paintNodeSetDisplacements = act;
	connect( act, SIGNAL( triggered( ) ), this, SLOT( OnQuardEditSetNodalDiplacements( ) ) );

	// 
	act = new QAction( tr("Set Nodal Loads"), this );
	act ->setShortcut( tr("F7") );
	act ->setCheckable( true );
	actQuardEdit.paintNodeSetForces = act;
	connect( act, SIGNAL( triggered( ) ), this, SLOT( OnQuardEditSetNodalForces( ) ) );

	// 
	act = new QAction( tr("Set Surface Loads"), this );
	act ->setShortcut( tr("F8") );
	act ->setCheckable( true );
	actQuardEdit.paintFaceSetForces = act;
	connect( act, SIGNAL( triggered( ) ), this, SLOT( OnQuardEditSetFaceForces( ) ) );
	

	// Add/remove element
	act = new QAction( tr("Remove Elements"), this );
	act ->setStatusTip( tr("Remove elements from mesh") );
	act ->setShortcut( tr("F9") );
	act ->setCheckable( true );
	actQuardEdit.paintElemRemove = act;
	connect( act, SIGNAL( triggered( ) ), this, SLOT( OnQuardEditRemoveElements( ) ) );

	// Move (adjust node positions)
	act = new QAction( tr("Move Nodes"), this );
	act ->setStatusTip( tr("Edit node positions") );
	act ->setShortcut( tr("F4") );
	act ->setCheckable( true );
	actQuardEdit.paintNodeMove = act;
	connect( act, SIGNAL( triggered( ) ), this, SLOT( OnQuardEditMoveNodes( ) ) );

	act = new QAction( tr("Remove Elements With Material..."), this );
	act ->setStatusTip( tr("Remove elements with specific material from mesh") );
	act ->setShortcut( tr("F10") );
	act ->setCheckable( false );
	actQuardEdit.removeElementsWithMaterial = act;
	connect( act, SIGNAL( triggered( ) ), this, SLOT( OnQuardEditRemoveElementsWithSpecificMaterial( ) ) );


	// Create action group
	act_g = new QActionGroup( this );
	act_g ->addAction( actQuardEdit.selection );
	act_g ->addAction( actQuardEdit.paintElemSetMaterial );
	act_g ->addAction( actQuardEdit.paintNodeSetDisplacements );
	act_g ->addAction( actQuardEdit.paintNodeSetForces );
	act_g ->addAction( actQuardEdit.paintElemRemove );
	act_g ->addAction( actQuardEdit.paintNodeMove );
	act_g ->addAction( actQuardEdit.paintFaceSetForces );
	actQuardEdit.group = act_g;

	// Set initially active action
	actQuardEdit.selection ->setChecked( true );

	// Connect actions
	connect( actQuardEdit.group, SIGNAL( triggered( QAction * ) ), this, SLOT( OnQuardEditChange( QAction * ) ) );

}


void fMainWindow::CreateMenus( ) {
	menuFile	= menuBar( ) ->addMenu( tr("&File") );
	menuEdit	= menuBar( ) ->addMenu( tr("&Edit") );
	menuView	= menuBar( ) ->addMenu( tr("&View") );
	menuWindow	= menuBar( ) ->addMenu( tr("&Window") );
	menuHelp	= menuBar( ) ->addMenu( tr("&Help") );
}


void fMainWindow::CreateToolBars( ) {
	// Brush toolbar
	tbBrush				= new QToolBar( );
	// Core widgets
	sbBrushSize			= new QSpinBox( );
	sBrushHardness		= new QSlider( Qt::Horizontal );
	cbInclusive			= new QCheckBox( tr("Inclusive") );
	// Labels
	QLabel * l_bsize	= new QLabel( );
	QLabel * l_bhard	= new QLabel( );
	lBrushHardness		= new QLabel( );
	l_bsize ->setText( tr("Brush size: ") );
	l_bhard ->setText( tr("Brush hardness: ") );

	QWidget *		w_tmp			= new QWidget( );
	QHBoxLayout *	hbl_brush_size	= new QHBoxLayout( );

	hbl_brush_size ->addWidget( l_bsize );
	hbl_brush_size ->addWidget( sbBrushSize );

	hbl_brush_size ->addSpacing( 5 );

	hbl_brush_size ->addWidget( l_bhard );
	hbl_brush_size ->addWidget( sBrushHardness );
	hbl_brush_size ->addWidget( lBrushHardness );

	hbl_brush_size ->addSpacing( 5 );

	hbl_brush_size ->addWidget( cbInclusive );
	hbl_brush_size ->addStretch( 1 );
	
	hbl_brush_size ->setContentsMargins ( 5, 3, 0, 3 );
	w_tmp ->setLayout( hbl_brush_size );

	tbBrush ->addWidget( w_tmp );

	addToolBar( Qt::TopToolBarArea, tbBrush );

	// Adjust parameters
	sbBrushSize ->setMinimumWidth( 50 );
	sbBrushSize ->setRange(	app ->GetBrush( ) ->GetMinDiameter( ), app ->GetBrush( ) ->GetMaxDiameter( ) );
	sbBrushSize ->setValue( app ->GetBrush( ) ->GetDiameter( ) );

	int hardness = app ->GetBrush( ) ->GetHardness( );
	sBrushHardness ->setRange( 0, 100 );
	sBrushHardness ->setSingleStep( 1 );
	sBrushHardness ->setValue( hardness );
	sBrushHardness ->setMaximumWidth( 101 );

	cbInclusive ->setChecked( app ->GetBrush( ) ->GetInclusiveState( ) );

	lBrushHardness ->setText( QString::number( hardness ) + "%" );

	// Actions
	connect( sbBrushSize, SIGNAL( valueChanged( int ) ), app ->GetBrush( ), SLOT( OnBrushSizeChange( int ) ) );
	connect( sBrushHardness, SIGNAL( valueChanged( int ) ), app ->GetBrush( ), SLOT( OnBrushHardnessChange( int ) ) );
	connect( cbInclusive, SIGNAL( toggled ( bool ) ), app ->GetBrush( ), SLOT( OnInclusiveStateChange( bool ) ) );

	connect( app ->GetBrush( ), SIGNAL( sizeChanged( int ) ), this, SLOT( OnBrushSizeChange( int ) ) );
	connect( app ->GetBrush( ), SIGNAL( hardnessChanged( int ) ), this, SLOT( OnBrushHardnessChange( int ) ) );
	connect( app ->GetBrush( ), SIGNAL( inclusiveStateChanged( bool ) ), this, SLOT( OnInclusiveStateChange( bool ) ) );
}


void fMainWindow::AdjustMenus( ) {
	AdjustFileMenu( );
	AdjustEditMenu( );
	AdjustViewMenu( );
	AdjustWindowMenu( );
	AdjustHelpMenu( );

	AdjustToolBars( );
}


void fMainWindow::AdjustFileMenu( ) {
	// Obtain status
	fAppStatus_e app_status = app ->GetStatus( );

	menuFile ->addAction( actNewMesh );
	menuFile ->addAction( actSaveMesh );
	menuFile ->addSeparator( );
	menuFile ->addAction( actExit );

	// Setup menus when no mesh is active
	if ( app_status == APPS_PREPARE ) {
		actSaveMesh ->setEnabled( false );
	} else {
		actSaveMesh ->setEnabled( true );
	}
}


void fMainWindow::AdjustEditMenu( ) {
	// Obtain status
	fAppStatus_e app_status = app ->GetStatus( );

	if ( app_status == APPS_EDIT_MESH ) {
		if ( app ->GetMesh( ) ->GetType( ) == MT_TRUSS ) {
			menuEdit ->clear( );
			menuEdit ->addAction( actTrussEdit.selection );
			menuEdit ->addSeparator( );
			menuEdit ->addAction( actTrussEdit.paintNodeMove );
			menuEdit ->addSeparator( );
			menuEdit ->addAction( actTrussEdit.paintElemSetMaterial );
			menuEdit ->addAction( actTrussEdit.paintNodeSetDisplacements );
			menuEdit ->addAction( actTrussEdit.paintNodeSetForces );
			menuEdit ->addSeparator( );
			menuEdit ->addAction( actTrussEdit.paintNodeRemove );
			menuEdit ->addAction( actTrussEdit.paintElemRemove );
			menuEdit ->addAction( actTrussEdit.removeElementsWithMaterial );
		} else if ( app ->GetMesh( ) ->GetType( ) == MT_CONTINUUM ) {
			menuEdit ->clear( );
			menuEdit ->addAction( actQuardEdit.selection );
			menuEdit ->addSeparator( );
			menuEdit ->addAction( actQuardEdit.paintNodeMove );
			menuEdit ->addSeparator( );
			menuEdit ->addAction( actQuardEdit.paintElemSetMaterial );
			menuEdit ->addAction( actQuardEdit.paintNodeSetDisplacements );
			menuEdit ->addAction( actQuardEdit.paintNodeSetForces );
			menuEdit ->addAction( actQuardEdit.paintFaceSetForces );
			menuEdit ->addSeparator( );
			menuEdit ->addAction( actQuardEdit.paintElemRemove );
			menuEdit ->addAction( actQuardEdit.removeElementsWithMaterial );
		}
	} else {
		
	}
}


void fMainWindow::AdjustViewMenu( ) {
	// Obtain status
	fAppStatus_e app_status = app ->GetStatus( );

	menuView ->addAction( actCenterMesh );

	// Setup menus when no mesh is active
	if ( app_status == APPS_PREPARE ) {
		actCenterMesh ->setEnabled( false );
	} else {
		actCenterMesh ->setEnabled( true );
	}
}


void fMainWindow::AdjustToolBars( )  {
	
}


void fMainWindow::AdjustWindowMenu( ) {
	// Obtain status
	fAppStatus_e	app_status	= app ->GetStatus( );

	menuWindow ->addAction( dwNavigator ->toggleViewAction( ) );

	if ( app_status == APPS_PREPARE ) {

	} else {
		fMeshType_e	mesh_type = app ->GetMesh( ) ->GetType( );
		if ( mesh_type == MT_TRUSS ) {
			menuWindow ->addAction( dwTrussMaterials ->toggleViewAction( ) );
			menuWindow ->addAction( dwTrussNodeDisps ->toggleViewAction( ) );
			menuWindow ->addAction( dwTrussNodeLoads ->toggleViewAction( ) );
		} else if ( mesh_type == MT_CONTINUUM ) {
			menuWindow ->addAction( dwQuardMaterials ->toggleViewAction( ) );
			menuWindow ->addAction( dwQuardNodeDisps ->toggleViewAction( ) );
			menuWindow ->addAction( dwQuardNodeLoads ->toggleViewAction( ) );
			menuWindow ->addAction( dwQuardFaceLoads ->toggleViewAction( ) );
		}
	}
}


void fMainWindow::RemoveMeshPanels( ) {
	// Materials
	if ( tmpTrussMaterials ) {
		disconnect( tmpTrussMaterials, 0, 0, 0 );
		this ->removeDockWidget( dwTrussMaterials );
		delete dwTrussMaterials;
		tmpTrussMaterials = NULL;
	}

	// Displacements
	if ( tndpNodeDisps ) {
		disconnect( tndpNodeDisps, 0, 0, 0 );
		this ->removeDockWidget( dwTrussNodeDisps );
		delete dwTrussNodeDisps;
		tndpNodeDisps = NULL;
	}

	// Loads
	if ( tnlpNodeLoads ) {
		disconnect( tnlpNodeLoads, 0, 0, 0 );
		this ->removeDockWidget( dwTrussNodeLoads );
		delete dwTrussNodeLoads;
		tnlpNodeLoads = NULL;
	}

	// Materials
	if ( qmpQuardMaterials ) {
		disconnect( qmpQuardMaterials, 0, 0, 0 );
		this ->removeDockWidget( dwQuardMaterials );
		delete dwQuardMaterials;
		qmpQuardMaterials = NULL;
	}

	// Displacements
	if ( qndpNodeDisps ) {
		disconnect( qndpNodeDisps, 0, 0, 0 );
		this ->removeDockWidget( dwQuardNodeDisps );
		delete dwQuardNodeDisps;
		qndpNodeDisps = NULL;
	}

	// Loads
	if ( qnlpNodeLoads ) {
		disconnect( qnlpNodeLoads, 0, 0, 0 );
		this ->removeDockWidget( dwQuardNodeLoads );
		delete dwQuardNodeLoads;
		qnlpNodeLoads = NULL;
	}

	// Face Loads
	if ( qflpFaceLoads ) {
		disconnect( qflpFaceLoads, 0, 0, 0 );
		this ->removeDockWidget( dwQuardFaceLoads );
		delete dwQuardFaceLoads;
		qflpFaceLoads = NULL;
	}
}


void fMainWindow::CreateTrussPanels( fTrussMesh *mesh ) {
	// Materials
	if ( tmpTrussMaterials ) {
		disconnect( tmpTrussMaterials, 0, 0, 0 );
		this ->removeDockWidget( dwTrussMaterials );
		delete dwTrussMaterials;
	}

	tmpTrussMaterials	= new fTrussMaterialsPanel( mesh ->GetMaterialList( ) );
	dwTrussMaterials	= new QDockWidget( tr("Materials") );
	dwTrussMaterials ->setWidget( tmpTrussMaterials );
	this ->addDockWidget( Qt::RightDockWidgetArea, dwTrussMaterials );
	setTabPosition( Qt::AllDockWidgetAreas, QTabWidget::North );
	connect( tmpTrussMaterials, SIGNAL( MaterialSelected( int ) ), this, SLOT( OnTrussMaterialSelected( int ) ) );

	// Displacements
	if ( tndpNodeDisps ) {
		disconnect( tndpNodeDisps, 0, 0, 0 );
		this ->removeDockWidget( dwTrussNodeDisps );
		delete dwTrussNodeDisps;
	}

	tndpNodeDisps		= new fTrussNodeDispsPanel( mesh ->GetNodeDisplacementList( ) );
	dwTrussNodeDisps	= new QDockWidget( tr("Nodal Displacements") );
	dwTrussNodeDisps ->setWidget( tndpNodeDisps );
	this ->addDockWidget( Qt::RightDockWidgetArea, dwTrussNodeDisps );
	setTabPosition( Qt::AllDockWidgetAreas, QTabWidget::North );
	connect( tndpNodeDisps, SIGNAL( DisplacementSelected( int ) ), this, SLOT( OnTrussNodeDispSelected( int ) ) );

	// Loads
	if ( tnlpNodeLoads ) {
		disconnect( tnlpNodeLoads, 0, 0, 0 );
		this ->removeDockWidget( dwTrussNodeLoads );
		delete dwTrussNodeLoads;
	}

	tnlpNodeLoads		= new fTrussNodeLoadsPanel( mesh ->GetNodeLoadList( ) );
	dwTrussNodeLoads	= new QDockWidget( tr("Nodal Loads") );
	dwTrussNodeLoads ->setWidget( tnlpNodeLoads );
	this ->addDockWidget( Qt::RightDockWidgetArea, dwTrussNodeLoads );
	setTabPosition( Qt::AllDockWidgetAreas, QTabWidget::North );
	connect( tnlpNodeLoads, SIGNAL( LoadSelected( int ) ), this, SLOT( OnTrussNodeLoadSelected( int ) ) );
}


void fMainWindow::CreateQuardPanels( fQuardMesh *mesh ) {
	// Materials
	if ( qmpQuardMaterials ) {
		disconnect( qmpQuardMaterials, 0, 0, 0 );
		this ->removeDockWidget( dwQuardMaterials );
		delete dwQuardMaterials;
	}

	qmpQuardMaterials	= new fQuardMaterialsPanel( mesh ->GetMaterialList( ) );
	dwQuardMaterials	= new QDockWidget( tr("Materials") );
	dwQuardMaterials ->setWidget( qmpQuardMaterials );
	this ->addDockWidget( Qt::RightDockWidgetArea, dwQuardMaterials );
	setTabPosition( Qt::AllDockWidgetAreas, QTabWidget::North );
	connect( qmpQuardMaterials, SIGNAL( MaterialSelected( int ) ), this, SLOT( OnQuardMaterialSelected( int ) ) );

	// Displacements
	if ( qndpNodeDisps ) {
		disconnect( qndpNodeDisps, 0, 0, 0 );
		this ->removeDockWidget( dwQuardNodeDisps );
		delete dwQuardNodeDisps;
	}

	qndpNodeDisps		= new fQuardNodeDispsPanel( mesh ->GetNodeDisplacementList( ) );
	dwQuardNodeDisps	= new QDockWidget( tr("Nodal Displacements") );
	dwQuardNodeDisps ->setWidget( qndpNodeDisps );
	this ->addDockWidget( Qt::RightDockWidgetArea, dwQuardNodeDisps );
	setTabPosition( Qt::AllDockWidgetAreas, QTabWidget::North );
	connect( qndpNodeDisps, SIGNAL( DisplacementSelected( int ) ), this, SLOT( OnQuardNodeDispSelected( int ) ) );

	// Loads
	if ( qnlpNodeLoads ) {
		disconnect( qnlpNodeLoads, 0, 0, 0 );
		this ->removeDockWidget( dwQuardNodeLoads );
		delete dwQuardNodeLoads;
	}

	qnlpNodeLoads		= new fQuardNodeLoadsPanel( mesh ->GetNodeLoadList( ) );
	dwQuardNodeLoads	= new QDockWidget( tr("Nodal Loads") );
	dwQuardNodeLoads ->setWidget( qnlpNodeLoads );
	this ->addDockWidget( Qt::RightDockWidgetArea, dwQuardNodeLoads );
	setTabPosition( Qt::AllDockWidgetAreas, QTabWidget::North );
	connect( qnlpNodeLoads, SIGNAL( LoadSelected( int ) ), this, SLOT( OnQuardNodeLoadSelected( int ) ) );


	// Face Loads
	if ( qflpFaceLoads ) {
		disconnect( qflpFaceLoads, 0, 0, 0 );
		this ->removeDockWidget( dwQuardFaceLoads );
		delete dwQuardFaceLoads;
	}

	qflpFaceLoads		= new fQuardSurfLoadsPanel( mesh ->GetSurfLoadList( ) );
	dwQuardFaceLoads	= new QDockWidget( tr("Surface Loads") );
	dwQuardFaceLoads ->setWidget( qflpFaceLoads );
	this ->addDockWidget( Qt::RightDockWidgetArea, dwQuardFaceLoads );
	setTabPosition( Qt::AllDockWidgetAreas, QTabWidget::North );
	connect( qflpFaceLoads, SIGNAL( LoadSelected( int ) ), this, SLOT( OnQuardFaceLoadSelected( int ) ) );

	tabifyDockWidget( dwQuardNodeLoads, dwQuardFaceLoads );
}



void fMainWindow::AdjustHelpMenu( ) {
	menuHelp ->addAction( actAbout );
}


void fMainWindow::OnNewMesh( ) {
	fMesh *mesh = NULL;

	// Save the previous dock state
	if ( app ->GetStatus( ) == APPS_EDIT_MESH ) {
		if ( app ->GetMesh( ) ->GetType( ) == MT_TRUSS ) {
			//trussMeshDockState = saveState( );
		} else if ( app ->GetMesh( ) ->GetType( ) == MT_CONTINUUM ) {
			//quardMeshDockState = saveState( );
		}
	}


	// Create and execute new mesh dialog
	fNewMeshDialog * dlg = new fNewMeshDialog( this );
	int result = dlg ->exec( );

	// Analyse the result
	if ( result ) {
		fMeshType_e	mesh_type	= dlg ->ReadMeshType( );
					mesh		= NULL;

		// Truss
		if ( mesh_type == MT_TRUSS ) {
			// Read params
			double						size_x		= dlg ->ReadTrussMeshSizeX( );
			double						size_y		= dlg ->ReadTrussMeshSizeY( );
			int							nx			= dlg ->ReadTrussMeshNodeNumX( );
			int							ny			= dlg ->ReadTrussMeshNodeNumY( );
			int							con_size	= dlg ->ReadTrussMeshConnectivitySize( );
			fTrussMeshConnectivity_e	con_type	= dlg ->ReadTrussMeshConnectivity( );

			// Create truss mesh
			fTrussMesh *truss_mesh		= new fTrussMesh( size_x, size_y, nx, ny, con_type, con_size );
			// Add materials
			fTrussMaterial *truss_mat1	= new fTrussMaterial( tr("Material 1"), truss_mesh ->GetDefaultMat1Color( ), 1.0f, 1.0f );
			fTrussMaterial *truss_mat2	= new fTrussMaterial( tr("Material 2"), truss_mesh ->GetDefaultMat2Color( ), 2.0f, 2.0f );
			truss_mesh ->GetMaterialList( ) ->AddMaterial( truss_mat1 );
			truss_mesh ->GetMaterialList( ) ->AddMaterial( truss_mat2 );
			// Add displacements
			fNodeDisplacement *truss_nd	= new fNodeDisplacement( tr("Displacement Group 1"), truss_mesh ->GetDefaultDispColor( ), ND_ALL, 0.0f, 0.0f );
			truss_mesh ->GetNodeDisplacementList( ) ->AddNodeDisplacement( truss_nd );
			// Add loads
			fNodeLoad *truss_nl	= new fNodeLoad( tr("Load Group 1"), truss_mesh ->GetDefaultLoadColor( ), 0.0f, -1.0f );
			truss_mesh ->GetNodeLoadList( ) ->AddNodeLoad( truss_nl );


			// Set the mesh
			mesh = truss_mesh;
			app ->SetMesh( truss_mesh );

			// Set current edit operation
			truss_mesh ->SetCurrentEditOperation( TME_SELECTION );
			actTrussEdit.selection ->setChecked( true );
			canvas ->UpdateCursorShape( );

			// Create truss control panels
			RemoveMeshPanels( );
			CreateTrussPanels( truss_mesh );

		// Continuum mesh
		} else if ( mesh_type == MT_CONTINUUM ) {
			// Read params
			double						size_x		= dlg ->ReadQuardMeshSizeX( );
			double						size_y		= dlg ->ReadQuardMeshSizeY( );
			int							ex			= dlg ->ReadQuardMeshElemNumX( );
			int							ey			= dlg ->ReadQuardMeshElemNumY( );

			// Create truss mesh
			fQuardMesh *quard_mesh		= new fQuardMesh( size_x, size_y, ex, ey );
			// Add materials
			fQuardMaterial *quard_mat1	= new fQuardMaterial( tr("Material 1"), quard_mesh ->GetDefaultMat1Color( ), 1.0f, 1.0f, 0.3f, 1000.0f, true, 0.0f, 0.0f );
			fQuardMaterial *quard_mat2	= new fQuardMaterial( tr("Material 2"), quard_mesh ->GetDefaultMat2Color( ), 2.0f, 2.0f, 0.3f, 2000.0f, true, 0.0f, 0.0f );
			quard_mesh ->GetMaterialList( ) ->AddMaterial( quard_mat1 );
			quard_mesh ->GetMaterialList( ) ->AddMaterial( quard_mat2 );
			// Add displacements
			fNodeDisplacement *quard_nd	= new fNodeDisplacement( tr("Displacement Group 1"), quard_mesh ->GetDefaultDispColor( ), ND_ALL, 0.0f, 0.0f );
			quard_mesh ->GetNodeDisplacementList( ) ->AddNodeDisplacement( quard_nd );
			// Add node loads
			fNodeLoad *quard_nl	= new fNodeLoad( tr("Load Group 1"), quard_mesh ->GetDefaultLoadColor( ), 0.0f, -1.0f );
			quard_mesh ->GetNodeLoadList( ) ->AddNodeLoad( quard_nl );

			// Add face loads
			fSurfLoad *quard_sl	= new fSurfLoad( tr("Surface Load Group 1"), quard_mesh ->GetDefaultSurfLoadColor( ), 1.0f );
			quard_mesh ->GetSurfLoadList( ) ->AddSurfLoad( quard_sl );


			// Set the mesh
			mesh = quard_mesh;
			app ->SetMesh( quard_mesh );

			// Set current edit operation
			quard_mesh ->SetCurrentEditOperation( QME_SELECTION );
			actQuardEdit.selection ->setChecked( true );
			canvas ->UpdateCursorShape( );

			// Create quard control panels
			RemoveMeshPanels( );
			CreateQuardPanels( quard_mesh );
		}

		if ( mesh ) {
			// Update the status of the application
			app ->SetStatus( APPS_EDIT_MESH );

			// And adjust menus due to that
			AdjustMenus( );

			// Setup the view of the canvas
			canvas ->SetupView( mesh ->GetBounds( ).ScaleSelf( 1.2f ) );
		}

		if ( dwNavigator ->toggleViewAction( ) ->isChecked( ) ) {
			dwNavigator ->toggleViewAction( ) ->trigger( );
		}
	}

	// Delete dialog
	delete dlg;


	// Restore dock state
	if ( mesh ) {
		if ( app ->GetMesh( ) ->GetType( ) == MT_TRUSS ) {
			//restoreState( trussMeshDockState );
		} else if ( app ->GetMesh( ) ->GetType( ) == MT_CONTINUUM ) {
			//restoreState( quardMeshDockState );
		}
	}
}



void fMainWindow::OnSaveMesh( ) {
	QString selected_filter;

	QString file_name;
	if ( app ->UseNativeDialogs( ) ) {
		file_name = QFileDialog::getSaveFileName( this, tr("Save Mesh"), saveFilePath + "/Untitled", app ->GetMesh( ) ->GetFileFilters( ), &selected_filter );
	} else {
		file_name = QFileDialog::getSaveFileName( this, tr("Save Mesh"), saveFilePath + "/Untitled", app ->GetMesh( ) ->GetFileFilters( ), &selected_filter, QFileDialog::DontUseNativeDialog );
	}

	if ( !file_name.isEmpty( ) ) {
		QFileInfo fi( file_name );
		saveFilePath = fi.path( );

		app ->GetMesh( ) ->Save( file_name, selected_filter );
	}
}


void fMainWindow::OnExit( ) {
	close( );
}


void fMainWindow::OnAbout( ) {
	QString about = QString( "<b>" ) + app ->GetName( ) + QString( "</b> " ) + app ->GetVersion( ) + QString( ". " ) +
		QString("Program for editing two-dimensional truss and continuum structures.\n"
				"Made by Yuriy Elesin.\n\n") +
		QString("<b>Controls</b>: Use <i>spacebar</i> together with mouse for navigation" );
	/*
	QMessageBox::about( this, tr("About"), 
		tr(	"<b>FlExtract v1.01.</b> "
			"Program for editing two-dimensional truss and continuum structures.\n"
			"Made by Yuriy Elesin"
			) );
	*/
	QMessageBox::about( this, tr("About"), about );
}


void fMainWindow::closeEvent( QCloseEvent * event ) {
	WriteSettings( );
	event ->accept( );
}


void fMainWindow::ReadSettings( ) {
	// Get application settings
	QSettings * settings = app ->GetSettings( );

	settings ->beginGroup( "main_window" );
	QSize	size			= settings ->value( "size", QSize( 800, 500 ) ).toSize( );
	QPoint	default_pos		= QPoint( app ->desktop( ) ->availableGeometry( ).width( )/2, app ->desktop( ) ->availableGeometry( ).height( )/2 );
			default_pos		= default_pos - QPoint( size.width( )/2, size.height( )/2 );
	QPoint	pos				= settings ->value( "position", default_pos ).toPoint( );
	bool	maximazed		= settings ->value( "maximized", true ).toBool( );
	saveFilePath			= settings ->value( "save_file_path", QString( ) ).toString( );
	quardMeshDockState		= settings ->value( "quard_dock_state", QByteArray( ) ).toByteArray( );
	trussMeshDockState		= settings ->value( "truss_dock_state", QByteArray( ) ).toByteArray( );
	settings ->endGroup( );

	// Apply settings
	if ( maximazed ) {
		showMaximized( );
	} else {
		resize( size );
		move( pos );
	}
}

void fMainWindow::WriteSettings( ) {
	QSettings * settings = app ->GetSettings( );

	settings ->beginGroup( "main_window" );
		settings ->setValue( "size", size( ) );
		settings ->setValue( "position", pos( ) );
		settings ->setValue( "maximized", this ->isMaximized( ) );
		settings ->setValue( "save_file_path", saveFilePath );
		settings ->setValue( "quard_dock_state", quardMeshDockState );
		settings ->setValue( "truss_dock_state", trussMeshDockState );
	settings ->endGroup( );
}


void fMainWindow::OnCenterMesh( ) {
	canvas ->SetupView( app ->GetMesh( ) ->GetBounds( ).ScaleSelf( 1.2f ) );
	canvas ->Update( );
}


void fMainWindow::OnBrushSizeChange( int i ) {
	sbBrushSize ->setValue( i );
}


void fMainWindow::OnBrushHardnessChange( int i ) {
	sBrushHardness ->setValue( i );
	lBrushHardness ->setText( QString::number( i ) + "%" );
}


void fMainWindow::OnInclusiveStateChange( bool state ) {
	cbInclusive ->setChecked( state );
}


/* ================================================================================
									Truss
================================================================================ */
void fMainWindow::OnTrussEditSelection( ) {
	fTrussMesh *tmesh = static_cast<fTrussMesh *> ( app ->GetMesh( ) );
	tmesh ->SetCurrentEditOperation( TME_SELECTION );
}


void fMainWindow::OnTrussEditSetElementMaterials( ) {
	fTrussMesh *tmesh = static_cast<fTrussMesh *> ( app ->GetMesh( ) );
	tmesh ->SetCurrentEditOperation( TME_PAINT_ELEM_SET_MATERIAL );

	tmpTrussMaterials ->UpdateSelection( 0 );
}


void fMainWindow::OnTrussEditSetNodalDiplacements( ) {
	fTrussMesh *tmesh = static_cast<fTrussMesh *> ( app ->GetMesh( ) );
	tmesh ->SetCurrentEditOperation( TME_PAINT_NODE_ADD_REMOVE_BNDCOND );

	tndpNodeDisps ->UpdateSelection( 0 );
}


void fMainWindow::OnTrussEditSetNodalForces( ) {
	fTrussMesh *tmesh = static_cast<fTrussMesh *> ( app ->GetMesh( ) );
	tmesh ->SetCurrentEditOperation( TME_PAINT_NODE_ADD_REMOVE_LOAD );

	tnlpNodeLoads ->UpdateSelection( 0 );
}


void fMainWindow::OnTrussEditRemoveElements( ) {
	fTrussMesh *tmesh = static_cast<fTrussMesh *> ( app ->GetMesh( ) );
	tmesh ->SetCurrentEditOperation( TME_PAINT_ELEM_REMOVE );
}


void fMainWindow::OnTrussEditRemoveElementsWithSpecificMaterial( ) {
	fTrussMesh *tmesh = static_cast<fTrussMesh *> ( app ->GetMesh( ) );

	// Show the dialog with material selection
	fRemoveTrussElementsWithMaterialDialog * dlg = new fRemoveTrussElementsWithMaterialDialog( tmesh ->GetMaterialList( ), this );
	int result = dlg ->exec( );

	// Analyse the result
	if ( result ) {
		int mat_id = dlg ->ReadMaterialId( );
		// Delete the elements with this material
		tmesh ->RemoveElementsWithMaterialId( mat_id );
	}
}


void fMainWindow::OnTrussEditRemoveNodes( ) {
	fTrussMesh *tmesh = static_cast<fTrussMesh *> ( app ->GetMesh( ) );
	tmesh ->SetCurrentEditOperation( TME_PAINT_NODE_REMOVE );
}


void fMainWindow::OnTrussEditMoveNodes( ) {
	fTrussMesh *tmesh = static_cast<fTrussMesh *> ( app ->GetMesh( ) );
	tmesh ->SetCurrentEditOperation( TME_PAINT_NODE_MOVE );
}


void fMainWindow::OnTrussEditChange( QAction *action ) {
	if ( action != actTrussEdit.paintElemSetMaterial ) {
		tmpTrussMaterials ->UpdateSelection( -1 );
	}

	if ( action != actTrussEdit.paintNodeSetDisplacements ) {
		tndpNodeDisps ->UpdateSelection( -1 );
	}

	if ( action != actTrussEdit.paintNodeSetForces ) {
		tnlpNodeLoads ->UpdateSelection( -1 );
	}

	canvas ->UpdateCursorShape( );
}


void fMainWindow::OnTrussMaterialSelected( int id ) {
	fTrussMesh *tmesh = static_cast<fTrussMesh *> ( app ->GetMesh( ) );
	tmesh ->SetCurrentEditOperation( TME_PAINT_ELEM_SET_MATERIAL );
	actTrussEdit.paintElemSetMaterial ->setChecked( true );

	tndpNodeDisps ->UpdateSelection( -1 );
	tnlpNodeLoads ->UpdateSelection( -1 );

	canvas ->UpdateCursorShape( );
}


void fMainWindow::OnTrussNodeDispSelected( int id ) {
	fTrussMesh *tmesh = static_cast<fTrussMesh *> ( app ->GetMesh( ) );
	tmesh ->SetCurrentEditOperation( TME_PAINT_NODE_ADD_REMOVE_BNDCOND );
	actTrussEdit.paintNodeSetDisplacements ->setChecked( true );

	tmpTrussMaterials ->UpdateSelection( -1 );
	tnlpNodeLoads ->UpdateSelection( -1 );

	canvas ->UpdateCursorShape( );
}


void fMainWindow::OnTrussNodeLoadSelected( int id ) {
	fTrussMesh *tmesh = static_cast<fTrussMesh *> ( app ->GetMesh( ) );
	tmesh ->SetCurrentEditOperation( TME_PAINT_NODE_ADD_REMOVE_LOAD );
	actTrussEdit.paintNodeSetForces ->setChecked( true );

	tmpTrussMaterials ->UpdateSelection( -1 );
	tndpNodeDisps ->UpdateSelection( -1 );

	canvas ->UpdateCursorShape( );
}




/* ================================================================================
									Quard
================================================================================ */
void fMainWindow::OnQuardEditSelection( ) {
	fQuardMesh *qmesh = static_cast<fQuardMesh *> ( app ->GetMesh( ) );
	qmesh ->SetCurrentEditOperation( QME_SELECTION );
}


void fMainWindow::OnQuardEditSetElementMaterials( ) {
	fQuardMesh *qmesh = static_cast<fQuardMesh *> ( app ->GetMesh( ) );
	qmesh ->SetCurrentEditOperation( QME_PAINT_ELEM_SET_MATERIAL );

	qmpQuardMaterials ->UpdateSelection( 0 );
}


void fMainWindow::OnQuardEditSetNodalDiplacements( ) {
	fQuardMesh *qmesh = static_cast<fQuardMesh *> ( app ->GetMesh( ) );
	qmesh ->SetCurrentEditOperation( QME_PAINT_NODE_ADD_REMOVE_BNDCOND );

	qndpNodeDisps ->UpdateSelection( 0 );
}


void fMainWindow::OnQuardEditSetNodalForces( ) {
	fQuardMesh *qmesh = static_cast<fQuardMesh *> ( app ->GetMesh( ) );
	qmesh ->SetCurrentEditOperation( QME_PAINT_NODE_ADD_REMOVE_LOAD );

	qnlpNodeLoads ->UpdateSelection( 0 );
}


void fMainWindow::OnQuardEditSetFaceForces( ) {
	fQuardMesh *qmesh = static_cast<fQuardMesh *> ( app ->GetMesh( ) );
	qmesh ->SetCurrentEditOperation( QME_PAINT_FACE_ADD_REMOVE_LOAD );

	qflpFaceLoads ->UpdateSelection( 0 );
}


void fMainWindow::OnQuardEditRemoveElements( ) {
	fQuardMesh *qmesh = static_cast<fQuardMesh *> ( app ->GetMesh( ) );
	qmesh ->SetCurrentEditOperation( QME_PAINT_ELEM_REMOVE );
}


void fMainWindow::OnQuardEditRemoveElementsWithSpecificMaterial( ) {
	fQuardMesh *qmesh = static_cast<fQuardMesh *> ( app ->GetMesh( ) );

	// Show the dialog with material selection
	fRemoveQuardElementsWithMaterialDialog * dlg = new fRemoveQuardElementsWithMaterialDialog( qmesh ->GetMaterialList( ), this );
	int result = dlg ->exec( );

	// Analyse the result
	if ( result ) {
		int mat_id = dlg ->ReadMaterialId( );
		// Delete the elements with this material
		qmesh ->RemoveElementsWithMaterialId( mat_id );
	}
}


void fMainWindow::OnQuardEditMoveNodes( ) {
	fQuardMesh *qmesh = static_cast<fQuardMesh *> ( app ->GetMesh( ) );
	qmesh ->SetCurrentEditOperation( QME_PAINT_NODE_MOVE );
}


void fMainWindow::OnQuardEditChange( QAction *action ) {
	if ( action != actQuardEdit.paintElemSetMaterial ) {
		qmpQuardMaterials ->UpdateSelection( -1 );
	}

	if ( action != actQuardEdit.paintNodeSetDisplacements ) {
		qndpNodeDisps ->UpdateSelection( -1 );
	}

	if ( action != actQuardEdit.paintNodeSetForces ) {
		qnlpNodeLoads ->UpdateSelection( -1 );
	}

	if ( action != actQuardEdit.paintFaceSetForces ) {
		qflpFaceLoads ->UpdateSelection( -1 );
	}

	canvas ->UpdateCursorShape( );
}


void fMainWindow::OnQuardMaterialSelected( int id ) {
	fQuardMesh *tmesh = static_cast<fQuardMesh *> ( app ->GetMesh( ) );
	tmesh ->SetCurrentEditOperation( QME_PAINT_ELEM_SET_MATERIAL );
	actQuardEdit.paintElemSetMaterial ->setChecked( true );

	qndpNodeDisps ->UpdateSelection( -1 );
	qnlpNodeLoads ->UpdateSelection( -1 );
	qflpFaceLoads ->UpdateSelection( -1 );

	canvas ->UpdateCursorShape( );
}


void fMainWindow::OnQuardNodeDispSelected( int id ) {
	fQuardMesh *tmesh = static_cast<fQuardMesh *> ( app ->GetMesh( ) );
	tmesh ->SetCurrentEditOperation( QME_PAINT_NODE_ADD_REMOVE_BNDCOND );
	actQuardEdit.paintNodeSetDisplacements ->setChecked( true );

	qmpQuardMaterials ->UpdateSelection( -1 );
	qnlpNodeLoads ->UpdateSelection( -1 );
	qflpFaceLoads ->UpdateSelection( -1 );

	canvas ->UpdateCursorShape( );
}


void fMainWindow::OnQuardNodeLoadSelected( int id ) {
	fQuardMesh *tmesh = static_cast<fQuardMesh *> ( app ->GetMesh( ) );
	tmesh ->SetCurrentEditOperation( QME_PAINT_NODE_ADD_REMOVE_LOAD );
	actQuardEdit.paintNodeSetForces ->setChecked( true );

	qmpQuardMaterials ->UpdateSelection( -1 );
	qndpNodeDisps ->UpdateSelection( -1 );
	qflpFaceLoads ->UpdateSelection( -1 );

	canvas ->UpdateCursorShape( );
}


void fMainWindow::OnQuardFaceLoadSelected( int id ) {
	fQuardMesh *tmesh = static_cast<fQuardMesh *> ( app ->GetMesh( ) );
	tmesh ->SetCurrentEditOperation( QME_PAINT_FACE_ADD_REMOVE_LOAD );
	actQuardEdit.paintFaceSetForces ->setChecked( true );

	qmpQuardMaterials ->UpdateSelection( -1 );
	qndpNodeDisps ->UpdateSelection( -1 );
	qnlpNodeLoads ->UpdateSelection( -1 );

	canvas ->UpdateCursorShape( );
}