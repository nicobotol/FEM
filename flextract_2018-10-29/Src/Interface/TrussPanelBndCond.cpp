#include "../precompiled.h"


fEditTrussNodeDispDialog::fEditTrussNodeDispDialog( QString &name, const QColor &color, fNodeDisplacementType_e type, float dx, float dy, QWidget *parent) : QDialog( parent ) {
	// Truss mesh panel and its contents
	lColor						= new fClickableLabel( );
	leName						= new QLineEdit( );
	leDx						= new QLineEdit( );
	leDy						= new QLineEdit( );

	mColor = color;
	lColor ->setPalette( QPalette(color) );
    lColor ->setAutoFillBackground (true );
	lColor ->setMouseTracking( true );

	leName		->setText( name );
	leDx		->setText( QString::number( dx ) );
	leDy		->setText( QString::number( dy ) );

	leDx		->setValidator( new QDoubleValidator( leDx ) );
	leDy		->setValidator( new QDoubleValidator( leDy ) );

	QVBoxLayout *	vbl_main		= new QVBoxLayout( );
	QFormLayout *	fl_clr_name		= new QFormLayout( );
	fl_clr_name ->addRow( tr("Name:"), leName );
	fl_clr_name ->addRow( tr("Color:"), lColor );

	// Create group box for radio buttons and 2 line edits
	QGroupBox *		rb_group_box	= new QGroupBox(tr("Constraint type"));
	QVBoxLayout *	vbl_rad_group	= new QVBoxLayout( );

	rbDispX = new QRadioButton(tr("X"));
	rbDispY = new QRadioButton(tr("Y"));
	rbDispA = new QRadioButton(tr("All"));

	QHBoxLayout *hbox = new QHBoxLayout( );
	hbox ->addWidget( rbDispX );
	hbox ->addWidget( rbDispY );
	hbox ->addWidget( rbDispA );
	hbox ->addStretch( 1 );

	lDx			= new QLabel( tr("dx: ") );
	lDy			= new QLabel( tr("dy: ") );

	lDx			->setBuddy( leDx );
	lDy			->setBuddy( leDy );

	QGridLayout *gridLayout = new QGridLayout;
	gridLayout ->addWidget( lDx,  0, 0);
	gridLayout ->addWidget( leDx, 0, 1);
	gridLayout ->addWidget( lDy,  1, 0);
	gridLayout ->addWidget( leDy, 1, 1);

	vbl_rad_group ->addLayout( hbox );
	rb_group_box ->setLayout( vbl_rad_group );

	QDialogButtonBox *dbb_confirm = new QDialogButtonBox( QDialogButtonBox::Ok | QDialogButtonBox::Cancel );
    connect( dbb_confirm, SIGNAL(accepted()), this, SLOT(accept()) );
    connect( dbb_confirm, SIGNAL(rejected()), this, SLOT(reject()) );
	connect( lColor, SIGNAL( clicked( ) ), this, SLOT( OnColorChangeClick( ) ) );
	connect( rbDispX, SIGNAL( toggled( bool ) ), this, SLOT( OnRBDispXClick( bool ) ) );
	connect( rbDispY, SIGNAL( toggled( bool ) ), this, SLOT( OnRBDispYClick( bool ) ) );
	connect( rbDispA, SIGNAL( toggled( bool ) ), this, SLOT( OnRBDispAClick( bool ) ) );

	vbl_main ->addLayout( fl_clr_name );
	vbl_main ->addSpacing( 5 );
	vbl_main ->addWidget( rb_group_box );
	vbl_main ->addSpacing( 5 );
	vbl_main ->addLayout( gridLayout );
	vbl_main ->addSpacing( 5 );
	vbl_main ->addStretch( 1 );
	vbl_main ->addWidget( dbb_confirm );
	setLayout( vbl_main );

	rbDispA ->setChecked( true );
}


fEditTrussNodeDispDialog::~fEditTrussNodeDispDialog(  ) {
	
}



fNodeDisplacementType_e fEditTrussNodeDispDialog::ReadType( ) {
	if ( rbDispX ->isChecked( ) ) {
		return ND_X;
	} else if ( rbDispY ->isChecked( ) ) {
		return ND_Y;
	} else {
		return ND_ALL;
	}
}


QString fEditTrussNodeDispDialog::ReadName( ) {
	return leName ->text( );
}


QColor fEditTrussNodeDispDialog::ReadColor( ) {
	return mColor;
}


float fEditTrussNodeDispDialog::ReadDx( ) {
	return leDx ->text( ).toFloat( );
}


float fEditTrussNodeDispDialog::ReadDy( ) {
	return leDy ->text( ).toFloat( );
}


void fEditTrussNodeDispDialog::OnColorChangeClick( ) {
	QColor color = QColorDialog::getColor( mColor, this );
	if ( color.isValid( ) ) {
		mColor = color;
        lColor ->setPalette( QPalette( mColor ) );
        lColor ->setAutoFillBackground( true );
	}
}

void fEditTrussNodeDispDialog::OnRBDispXClick( bool checked ) {
	if ( checked ) {
		leDy ->hide( );
		lDy  ->hide( );
		leDx ->show( );
		lDx  ->show( );
	}
}


void fEditTrussNodeDispDialog::OnRBDispYClick( bool checked ) {
	if ( checked ) {
		leDx ->hide( );
		lDx  ->hide( );
		leDy ->show( );
		lDy  ->show( );
	}
}


void fEditTrussNodeDispDialog::OnRBDispAClick( bool checked ) {
	if ( checked ) {
		leDx ->show( );
		lDx  ->show( );
		leDy ->show( );
		lDy  ->show( );
	}
}








fEditTrussNodeLoadDialog::fEditTrussNodeLoadDialog( QString &name, const QColor &color, float fx, float fy, QWidget *parent) : QDialog( parent ) {
	// Truss mesh panel and its contents
	lColor						= new fClickableLabel( );
	leName						= new QLineEdit( );
	leFx						= new QLineEdit( );
	leFy						= new QLineEdit( );

	mColor = color;
	lColor ->setPalette( QPalette(color) );
    lColor ->setAutoFillBackground (true );
	lColor ->setMouseTracking( true );

	leName		->setText( name );
	leFx		->setText( QString::number( fx ) );
	leFy		->setText( QString::number( fy ) );

	leFx		->setValidator( new QDoubleValidator( leFx ) );
	leFy		->setValidator( new QDoubleValidator( leFy ) );

	QVBoxLayout *	vbl_main		= new QVBoxLayout( );
	QFormLayout *	fl_clr_name		= new QFormLayout( );
	fl_clr_name ->addRow( tr("Name:"), leName );
	fl_clr_name ->addRow( tr("Color:"), lColor );
	fl_clr_name ->addRow( tr("Fx:"), leFx );
	fl_clr_name ->addRow( tr("Fy:"), leFy);

	QDialogButtonBox *dbb_confirm = new QDialogButtonBox( QDialogButtonBox::Ok | QDialogButtonBox::Cancel );
    connect( dbb_confirm, SIGNAL(accepted()), this, SLOT(accept()) );
    connect( dbb_confirm, SIGNAL(rejected()), this, SLOT(reject()) );

	// Add all together
	vbl_main ->addLayout( fl_clr_name );
	vbl_main ->addSpacing( 5 );
	vbl_main ->addWidget( dbb_confirm );
	setLayout( vbl_main );

	connect( lColor, SIGNAL( clicked( ) ), this, SLOT( OnColorChangeClick( ) ) );
}


fEditTrussNodeLoadDialog::~fEditTrussNodeLoadDialog(  ) {
	
}


QString fEditTrussNodeLoadDialog::ReadName( ) {
	return leName ->text( );
}


QColor fEditTrussNodeLoadDialog::ReadColor( ) {
	return mColor;
}


float fEditTrussNodeLoadDialog::ReadFx( ) {
	return leFx ->text( ).toFloat( );
}


float fEditTrussNodeLoadDialog::ReadFy( ) {
	return leFy ->text( ).toFloat( );
}


void fEditTrussNodeLoadDialog::OnColorChangeClick( ) {
	QColor color = QColorDialog::getColor( mColor, this );
	if ( color.isValid( ) ) {
		mColor = color;
        lColor ->setPalette( QPalette( mColor ) );
        lColor ->setAutoFillBackground( true );
	}
}
























fTrussNodeDispsPanel::fTrussNodeDispsPanel( fNodeDisplacementList *disps, QWidget *parent ) : QWidget( parent ) {
	displacements				= NULL;

	leDx						= new QLineEdit( );
	leDy						= new QLineEdit( );

	// Create layouts
	QVBoxLayout *vbl_main		= new QVBoxLayout( );
	QHBoxLayout *hbl_add_remove = new QHBoxLayout( );

	// Create all necessary objects
	// 1) List view
	lvDisplacements					= new QListView( );
	vbl_main ->addWidget( lvDisplacements );

	// 2) Add/Remove buttons
	btnAddDisplacement			= new QPushButton( tr("Add") );
	btnRemoveDisplacement		= new QPushButton( tr("Remove") );
	hbl_add_remove ->addWidget( btnAddDisplacement );
	hbl_add_remove ->addStretch( 1 );
	hbl_add_remove ->addWidget( btnRemoveDisplacement );
	vbl_main ->addLayout( hbl_add_remove );

	// 3) Create group box for radio buttons and 2 line edits
	QGroupBox *		rb_group_box	= new QGroupBox(tr("Constraint type"));
	QVBoxLayout *	vbl_rad_group	= new QVBoxLayout( );

	rbDispX = new QRadioButton(tr("X"));
	rbDispY = new QRadioButton(tr("Y"));
	rbDispA = new QRadioButton(tr("All"));

	QHBoxLayout *hbox = new QHBoxLayout( );
	hbox ->addWidget( rbDispX );
	hbox ->addWidget( rbDispY );
	hbox ->addWidget( rbDispA );
	hbox ->addStretch( 1 );

	lDx			= new QLabel( tr("dx: ") );
	lDy			= new QLabel( tr("dy: ") );

	lDx			->setBuddy( leDx );
	lDy			->setBuddy( leDy );

	QGridLayout *gridLayout = new QGridLayout;
	gridLayout ->addWidget( lDx,  0, 0);
	gridLayout ->addWidget( leDx, 0, 1);
	gridLayout ->addWidget( lDy,  1, 0);
	gridLayout ->addWidget( leDy, 1, 1);

	vbl_rad_group ->addLayout( hbox );
	rb_group_box ->setLayout( vbl_rad_group );


	vbl_main ->addSpacing( 5 );
	vbl_main ->addWidget( rb_group_box );
	vbl_main ->addSpacing( 5 );
	vbl_main ->addLayout( gridLayout );
	// Assign the main layout
	this ->setLayout( vbl_main );

	// Setup selection mode
	lvDisplacements ->setSelectionMode( QAbstractItemView::SingleSelection );

	SetNodeDisplacementList( disps );

	// Setup validators
	leDx ->setValidator( new QDoubleValidator( leDx ) );
	leDy ->setValidator( new QDoubleValidator( leDy ) );

	// Create sinals/slots
	connect( btnAddDisplacement, SIGNAL( clicked( ) ), this, SLOT( OnAddDisplacement( ) ) );
	connect( btnRemoveDisplacement, SIGNAL( clicked( ) ), this, SLOT( OnRemoveDisplacement( ) ) );
	connect( leDx, SIGNAL( textEdited( const QString & ) ), this, SLOT( OnDxEdited( const QString & ) ) );
	connect( leDy, SIGNAL( textEdited( const QString & ) ), this, SLOT( OnDyEdited( const QString & ) ) );
	connect( lvDisplacements ->selectionModel( ), SIGNAL( selectionChanged( const QItemSelection &, const QItemSelection &) ), this, SLOT( OnTrussNodeDispSelected( const QItemSelection &, const QItemSelection & ) ) );
	connect( lvDisplacements, SIGNAL( doubleClicked( const QModelIndex & ) ), this, SLOT( OnEditDisplacement( const QModelIndex & ) ) );
	connect( rbDispX, SIGNAL( toggled( bool ) ), this, SLOT( OnRBDispXClick( bool ) ) );
	connect( rbDispY, SIGNAL( toggled( bool ) ), this, SLOT( OnRBDispYClick( bool ) ) );
	connect( rbDispA, SIGNAL( toggled( bool ) ), this, SLOT( OnRBDispAClick( bool ) ) );

	rbDispA ->setChecked( true );
}


fTrussNodeDispsPanel::~fTrussNodeDispsPanel(  ) {
	
}


void fTrussNodeDispsPanel::SetNodeDisplacementList( fNodeDisplacementList *disp ) {
	if ( disp ) {
		disconnect( displacements, SIGNAL( rowsAboutToBeInserted ( const QModelIndex &, int, int ) ), this, SLOT( OnRowsAboutToBeInserted( const QModelIndex &, int, int ) ) );
		disconnect( displacements, SIGNAL( rowsAboutToBeRemoved ( const QModelIndex &, int, int ) ), this, SLOT( OnRowsAboutToBeRemoved( const QModelIndex &, int, int ) ) );
	}

	displacements = disp;

	QItemSelectionModel *prev_d = lvDisplacements ->selectionModel( );
	lvDisplacements ->setModel( displacements );
	if ( prev_d ) delete prev_d;

	// Unselect everyone by default
	UpdateSelection( -1 );

	if ( disp ) {
		connect( disp, SIGNAL( rowsAboutToBeInserted ( const QModelIndex &, int, int ) ), this, SLOT( OnRowsAboutToBeInserted( const QModelIndex &, int, int ) ) );
		connect( disp, SIGNAL( rowsAboutToBeRemoved ( const QModelIndex &, int, int ) ), this, SLOT( OnRowsAboutToBeRemoved( const QModelIndex &, int, int ) ) );
	}
}


void fTrussNodeDispsPanel::UpdateSelection( int id ) {
	QItemSelectionModel *select_model = lvDisplacements ->selectionModel( );

	QModelIndex index = displacements ->index( id, 0, QModelIndex( ) );
	select_model ->select( index, QItemSelectionModel::ClearAndSelect );

	lvDisplacements ->scrollTo( index );
}


int fTrussNodeDispsPanel::GetSelectedDisplacementId( ) {
	QItemSelectionModel *select_model = lvDisplacements ->selectionModel( );

	if ( !select_model ->hasSelection( ) ) {
		return -1;
	}

	QModelIndexList indexes = select_model ->selectedIndexes( );
	return indexes[0].row( );
}


void fTrussNodeDispsPanel::OnTrussNodeDispSelected( const QItemSelection &selected, const QItemSelection &deselected ) {
	if ( selected.size( ) <= 0 ) {
		leDx ->setText( tr("") );
		leDy ->setText( tr("") );
		return;
	}
	int id		= selected.indexes( ).at( 0 ).row( );
	if ( !deselected.isEmpty( ) ) {
		int id_prev = deselected.indexes( ).at( 0 ).row( );
	}
	float					dx = displacements ->GetNodeDisplacement( id ) ->GetDx( );
	float					dy = displacements ->GetNodeDisplacement( id ) ->GetDy( );
	fNodeDisplacementType_e t  = displacements ->GetNodeDisplacement( id ) ->GetType( );

	leDx ->setText( QString::number( dx ) );
	leDy ->setText( QString::number( dy ) );
	switch ( t ) {
		case ND_X : {
			rbDispX ->setChecked( true );
			break;
		}
		case ND_Y : {
			rbDispY ->setChecked( true );
			break;
		}
		case ND_ALL : {
			rbDispA ->setChecked( true );
			break;
		}
	}

	fTrussMesh *tmesh = static_cast<fTrussMesh *> ( app ->GetMesh( ) );
	tmesh ->SetBrushNodeDisplacementId( id );

	emit DisplacementSelected( id );
}


void fTrussNodeDispsPanel::OnRowsAboutToBeInserted( const QModelIndex & parent, int start, int end ) {
	UpdateSelection( start );
}


void fTrussNodeDispsPanel::OnRowsAboutToBeRemoved( const QModelIndex & parent, int start, int end ) {
	if ( start < 1 ) {
		start = start + 1;
	}
	UpdateSelection( start - 1 );
}


void fTrussNodeDispsPanel::OnDxEdited( const QString &dx ) {
	int id = GetSelectedDisplacementId( );
	if ( id < 0 ) {
		return;
	}
	displacements ->GetNodeDisplacement( id ) ->SetDx( dx.toFloat( ) );
}


void fTrussNodeDispsPanel::OnDyEdited( const QString &dy ) {
	int id = GetSelectedDisplacementId( );
	if ( id < 0 ) {
		return;
	}
	displacements ->GetNodeDisplacement( id ) ->SetDy( dy.toFloat( ) );
}


void fTrussNodeDispsPanel::OnRBDispXClick( bool checked ) {
	if ( checked ) {
		leDy ->hide( );
		lDy  ->hide( );
		leDx ->show( );
		lDx  ->show( );
	}

	int id = GetSelectedDisplacementId( );
	if ( id < 0 ) {
		return;
	}
	displacements ->GetNodeDisplacement( id ) ->SetType( ND_X );
	// Update canvas
	app ->GetMainWindow( ) ->GetCanvas( ) ->Update( );
}


void fTrussNodeDispsPanel::OnRBDispYClick( bool checked ) {
	if ( checked ) {
		leDx ->hide( );
		lDx  ->hide( );
		leDy ->show( );
		lDy  ->show( );
	}
	
	int id = GetSelectedDisplacementId( );
	if ( id < 0 ) {
		return;
	}
	displacements ->GetNodeDisplacement( id ) ->SetType( ND_Y );
	// Update canvas
	app ->GetMainWindow( ) ->GetCanvas( ) ->Update( );
}


void fTrussNodeDispsPanel::OnRBDispAClick( bool checked ) {
	if ( checked ) {
		leDy ->show( );
		lDy  ->show( );
		leDx ->show( );
		lDx  ->show( );
	}
	
	int id = GetSelectedDisplacementId( );
	if ( id < 0 ) {
		return;
	}
	displacements ->GetNodeDisplacement( id ) ->SetType( ND_ALL );
	// Update canvas
	app ->GetMainWindow( ) ->GetCanvas( ) ->Update( );
}


void fTrussNodeDispsPanel::OnRemoveDisplacement( ) {
	// The mesh should be adjusted after removing materials
	fTrussMesh *tmesh = static_cast<fTrussMesh *> ( app ->GetMesh( ) );
	int id = GetSelectedDisplacementId( );

	if ( id < 0 ) {
		id = displacements ->rowCount( ) - 1;
	}
	if ( id < 0 || displacements ->rowCount( ) == 1 ) {
		return;
	}


	tmesh ->OnNodeDisplacementRemoved( id );
	displacements ->RemoveNodeDisplacement( id );

	// Update canvas
	app ->GetMainWindow( ) ->GetCanvas( ) ->Update( );
}


void fTrussNodeDispsPanel::OnAddDisplacement( ) {
	int		dis_num = displacements ->rowCount( );
	QString name	= QString( tr("Displacement Group ") ) + QString::number( dis_num + 1 );

	unsigned char r = 255 * float( rand( ) ) / float( RAND_MAX );
	unsigned char g = 255 * float( rand( ) ) / float( RAND_MAX );
	unsigned char b = 255 * float( rand( ) ) / float( RAND_MAX );

	fEditTrussNodeDispDialog *dlg = new fEditTrussNodeDispDialog( name, QColor( r, g, b ), ND_ALL, 0.0f, 0.0f, app ->GetMainWindow( ) );

	int res = dlg ->exec( );

	if ( res ) {
		// Create material
		fNodeDisplacement *dis = new fNodeDisplacement( dlg ->ReadName( ), dlg ->ReadColor( ), dlg ->ReadType( ), dlg ->ReadDx( ), dlg ->ReadDy( ) );
		displacements ->AddNodeDisplacement( dis );

		UpdateSelection( displacements ->rowCount( ) - 1 );
	}

	delete dlg;

	// Update canvas
	app ->GetMainWindow( ) ->GetCanvas( ) ->Update( );
}


void fTrussNodeDispsPanel::OnEditDisplacement( const QModelIndex &index ) {
	int id = index.row( );
	fNodeDisplacement *dis = displacements ->GetNodeDisplacement( id );

	QString					name	= dis ->GetName( );
	fNodeDisplacementType_e t		= dis ->GetType( );
	float					dx		= dis ->GetDx( );
	float					dy		= dis ->GetDy( );
	QColor					color	= dis ->GetColor( );

	fEditTrussNodeDispDialog *dlg = new fEditTrussNodeDispDialog( name, color, t, dx, dy, app ->GetMainWindow( ) );
	int res = dlg ->exec( );
	if ( res ) {
		// Update material
		dis ->SetName	( dlg ->ReadName( ) );
		dis ->SetColor	( dlg ->ReadColor( ) );
		dis ->SetDx		( dlg ->ReadDx( ) );
		dis ->SetDy		( dlg ->ReadDy( ) );
		dis ->SetType	( dlg ->ReadType( ) );

		// Update edit boxes as well
		leDx ->setText( QString::number( dlg ->ReadDx( ) ) );
		leDy ->setText( QString::number( dlg ->ReadDy( ) ) );

		switch ( dlg ->ReadType( ) ) {
			case ND_X : {
				rbDispX ->setChecked( true );
				break;
			}
			case ND_Y : {
				rbDispY ->setChecked( true );
				break;
			}
			case ND_ALL : {
				rbDispA ->setChecked( true );
				break;
			}
		}
	}
	delete dlg;

	// Update canvas
	app ->GetMainWindow( ) ->GetCanvas( ) ->Update( );
}

























fTrussNodeLoadsPanel::fTrussNodeLoadsPanel( fNodeLoadList *load, QWidget *parent ) : QWidget( parent ) {
	loads						= NULL;

	leFx						= new QLineEdit( );
	leFy						= new QLineEdit( );

	// Create layouts
	QVBoxLayout *vbl_main		= new QVBoxLayout( );
	QHBoxLayout *hbl_add_remove = new QHBoxLayout( );

	// Create all necessary objects
	// 1) List view
	lvLoads						= new QListView( );
	vbl_main ->addWidget( lvLoads );

	// 2) Add/Remove buttons
	btnAddLoad					= new QPushButton( tr("Add") );
	btnRemoveLoad				= new QPushButton( tr("Remove") );
	hbl_add_remove ->addWidget( btnAddLoad );
	hbl_add_remove ->addStretch( 1 );
	hbl_add_remove ->addWidget( btnRemoveLoad );
	vbl_main ->addLayout( hbl_add_remove );

	QFormLayout *fl_load		= new QFormLayout( );
	fl_load ->addRow( tr("Fx: "), leFx );
	fl_load ->addRow( tr("Fy: "), leFy );

	vbl_main ->addSpacing( 5 );
	vbl_main ->addLayout( fl_load );
	// Assign the main layout
	this ->setLayout( vbl_main );

	// Setup selection mode
	lvLoads ->setSelectionMode( QAbstractItemView::SingleSelection );

	SetNodeLoadList( load );

	// Setup validators
	leFx ->setValidator( new QDoubleValidator( leFx ) );
	leFy ->setValidator( new QDoubleValidator( leFy ) );

	// Create sinals/slots
	connect( btnAddLoad, SIGNAL( clicked( ) ), this, SLOT( OnAddLoad( ) ) );
	connect( btnRemoveLoad, SIGNAL( clicked( ) ), this, SLOT( OnRemoveLoad( ) ) );

	connect( leFx, SIGNAL( textEdited( const QString & ) ), this, SLOT( OnFxEdited( const QString & ) ) );
	connect( leFy, SIGNAL( textEdited( const QString & ) ), this, SLOT( OnFyEdited( const QString & ) ) );

	connect( lvLoads ->selectionModel( ), SIGNAL( selectionChanged( const QItemSelection &, const QItemSelection &) ), this, SLOT( OnTrussNodeLoadSelected( const QItemSelection &, const QItemSelection & ) ) );
	connect( lvLoads, SIGNAL( doubleClicked( const QModelIndex & ) ), this, SLOT( OnEditLoad( const QModelIndex & ) ) );
}


fTrussNodeLoadsPanel::~fTrussNodeLoadsPanel(  ) {
	
}


void fTrussNodeLoadsPanel::SetNodeLoadList( fNodeLoadList *load ) {
	if ( load ) {
		disconnect( loads, SIGNAL( rowsAboutToBeInserted ( const QModelIndex &, int, int ) ), this, SLOT( OnRowsAboutToBeInserted( const QModelIndex &, int, int ) ) );
		disconnect( loads, SIGNAL( rowsAboutToBeRemoved ( const QModelIndex &, int, int ) ), this, SLOT( OnRowsAboutToBeRemoved( const QModelIndex &, int, int ) ) );
	}

	loads = load;

	QItemSelectionModel *prev_d = lvLoads ->selectionModel( );
	lvLoads ->setModel( loads );
	if ( prev_d ) delete prev_d;

	// Unselect everyone by default
	UpdateSelection( -1 );

	if ( load ) {
		connect( load, SIGNAL( rowsAboutToBeInserted ( const QModelIndex &, int, int ) ), this, SLOT( OnRowsAboutToBeInserted( const QModelIndex &, int, int ) ) );
		connect( load, SIGNAL( rowsAboutToBeRemoved ( const QModelIndex &, int, int ) ), this, SLOT( OnRowsAboutToBeRemoved( const QModelIndex &, int, int ) ) );
	}
}


void fTrussNodeLoadsPanel::UpdateSelection( int id ) {
	QItemSelectionModel *select_model = lvLoads ->selectionModel( );

	QModelIndex index = loads ->index( id, 0, QModelIndex( ) );
	select_model ->select( index, QItemSelectionModel::ClearAndSelect );

	lvLoads ->scrollTo( index );
}


int fTrussNodeLoadsPanel::GetSelectedLoadId( ) {
	QItemSelectionModel *select_model = lvLoads ->selectionModel( );

	if ( !select_model ->hasSelection( ) ) {
		return -1;
	}

	QModelIndexList indexes = select_model ->selectedIndexes( );
	return indexes[0].row( );
}


void fTrussNodeLoadsPanel::OnTrussNodeLoadSelected( const QItemSelection &selected, const QItemSelection &deselected ) {
	if ( selected.size( ) <= 0 ) {
		leFx ->setText( tr("") );
		leFy ->setText( tr("") );
		return;
	}
	int id		= selected.indexes( ).at( 0 ).row( );
	if ( !deselected.isEmpty( ) ) {
		int id_prev = deselected.indexes( ).at( 0 ).row( );
	}
	float Fx = loads ->GetNodeLoad( id ) ->GetFx( );
	float Fy = loads ->GetNodeLoad( id ) ->GetFy( );

	leFx ->setText( QString::number( Fx ) );
	leFy ->setText( QString::number( Fy ) );

	fTrussMesh *tmesh = static_cast<fTrussMesh *> ( app ->GetMesh( ) );
	tmesh ->SetBrushNodeLoadId( id );

	emit LoadSelected( id );
}


void fTrussNodeLoadsPanel::OnRowsAboutToBeInserted( const QModelIndex & parent, int start, int end ) {
	UpdateSelection( start );
}


void fTrussNodeLoadsPanel::OnRowsAboutToBeRemoved( const QModelIndex & parent, int start, int end ) {
	if ( start < 1 ) {
		start = start + 1;
	}
	UpdateSelection( start - 1 );
}


void fTrussNodeLoadsPanel::OnFxEdited( const QString &Fx ) {
	int id = GetSelectedLoadId( );
	if ( id < 0 ) {
		return;
	}
	loads ->GetNodeLoad( id ) ->SetFx( Fx.toFloat( ) );
	app ->GetMainWindow( ) ->GetCanvas( ) ->Update( );
}


void fTrussNodeLoadsPanel::OnFyEdited( const QString &Fy ) {
	int id = GetSelectedLoadId( );
	if ( id < 0 ) {
		return;
	}
	loads ->GetNodeLoad( id ) ->SetFy( Fy.toFloat( ) );
	app ->GetMainWindow( ) ->GetCanvas( ) ->Update( );
}


void fTrussNodeLoadsPanel::OnRemoveLoad( ) {
	// The mesh should be adjusted after removing materials
	fTrussMesh *tmesh = static_cast<fTrussMesh *> ( app ->GetMesh( ) );
	int id = GetSelectedLoadId( );

	if ( id < 0 ) {
		id = loads ->rowCount( ) - 1;
	}
	if ( id < 0 || loads ->rowCount( ) == 1 ) {
		return;
	}


	tmesh ->OnNodeLoadRemoved( id );
	loads ->RemoveNodeLoad( id );

	// Update canvas
	app ->GetMainWindow( ) ->GetCanvas( ) ->Update( );
}


void fTrussNodeLoadsPanel::OnAddLoad( ) {
	int		dis_num = loads ->rowCount( );
	QString name	= QString( tr("Load Group ") ) + QString::number( dis_num + 1 );

	unsigned char r = 255 * float( rand( ) ) / float( RAND_MAX );
	unsigned char g = 255 * float( rand( ) ) / float( RAND_MAX );
	unsigned char b = 255 * float( rand( ) ) / float( RAND_MAX );

	fEditTrussNodeLoadDialog *dlg = new fEditTrussNodeLoadDialog( name, QColor( r, g, b ), 0.0f, -1.0f, app ->GetMainWindow( ) );

	int res = dlg ->exec( );

	if ( res ) {
		// Create material
		fNodeLoad *dis = new fNodeLoad( dlg ->ReadName( ), dlg ->ReadColor( ), dlg ->ReadFx( ), dlg ->ReadFy( ) );
		loads ->AddNodeLoad( dis );

		UpdateSelection( loads ->rowCount( ) - 1 );
	}

	delete dlg;

	// Update canvas
	app ->GetMainWindow( ) ->GetCanvas( ) ->Update( );
}


void fTrussNodeLoadsPanel::OnEditLoad( const QModelIndex &index ) {
	int id = index.row( );
	fNodeLoad *dis = loads ->GetNodeLoad( id );

	QString					name	= dis ->GetName( );
	float					Fx		= dis ->GetFx( );
	float					Fy		= dis ->GetFy( );
	QColor					color	= dis ->GetColor( );

	fEditTrussNodeLoadDialog *dlg = new fEditTrussNodeLoadDialog( name, color, Fx, Fy, app ->GetMainWindow( ) );
	int res = dlg ->exec( );
	if ( res ) {
		// Update material
		dis ->SetName	( dlg ->ReadName( ) );
		dis ->SetColor	( dlg ->ReadColor( ) );
		dis ->SetFx		( dlg ->ReadFx( ) );
		dis ->SetFy		( dlg ->ReadFy( ) );

		// Update edit boxes as well
		leFx ->setText( QString::number( dlg ->ReadFx( ) ) );
		leFy ->setText( QString::number( dlg ->ReadFy( ) ) );
	}
	delete dlg;

	// Update canvas
	app ->GetMainWindow( ) ->GetCanvas( ) ->Update( );
}