#include "../precompiled.h"

fRemoveTrussElementsWithMaterialDialog::fRemoveTrussElementsWithMaterialDialog( fTrussMaterialList *materials, QWidget *parent ) : QDialog( parent ) {
	this ->setWindowTitle( tr("Select material") );

	QVBoxLayout *vbl_main	= new QVBoxLayout( );

	lvMaterials				= new QListView( );
	lvMaterials ->setSelectionMode( QAbstractItemView::SingleSelection );
	lvMaterials ->setModel( materials );
	// Unselect everyone by default
	int id = 0;
	QItemSelectionModel *select_model = lvMaterials ->selectionModel( );
	QModelIndex index = materials ->index( id, 0, QModelIndex( ) );
	select_model ->select( index, QItemSelectionModel::ClearAndSelect );
	lvMaterials ->scrollTo( index );


	QDialogButtonBox *dbb_confirm = new QDialogButtonBox( QDialogButtonBox::Ok | QDialogButtonBox::Cancel );
    connect( dbb_confirm, SIGNAL(accepted()), this, SLOT(accept()) );
    connect( dbb_confirm, SIGNAL(rejected()), this, SLOT(reject()) );

	vbl_main ->addWidget( lvMaterials );
	vbl_main ->addWidget( dbb_confirm );
	setLayout( vbl_main );


	connect( lvMaterials, SIGNAL( doubleClicked( const QModelIndex & ) ), this, SLOT( OnMaterialDoubleClick( const QModelIndex & ) ) );
}


fRemoveTrussElementsWithMaterialDialog::~fRemoveTrussElementsWithMaterialDialog(  ) {

}


int fRemoveTrussElementsWithMaterialDialog::ReadMaterialId( ) {
	QItemSelectionModel *select_model = lvMaterials ->selectionModel( );
	if ( !select_model ->hasSelection( ) ) {
		return -1;
	}
	QModelIndexList indexes = select_model ->selectedIndexes( );
	return indexes[0].row( );
}



void fRemoveTrussElementsWithMaterialDialog::OnMaterialDoubleClick( const QModelIndex & ) {
	this ->accept( );
}





fEditTrussMaterialDialog::fEditTrussMaterialDialog( QString &name, const QColor &color, float area, float youngs, QWidget *parent ) : QDialog( parent ) {
	// Truss mesh panel and its contents
	lColor						= new fClickableLabel( );
	leName						= new QLineEdit( );
	leArea						= new QLineEdit( );
	leYoungs					= new QLineEdit( );

	mColor = color;
	lColor ->setPalette( QPalette(color) );
    lColor ->setAutoFillBackground (true );
	lColor ->setMouseTracking( true );

	leName		->setText( name );
	leArea		->setText( QString::number( area ) );
	leYoungs	->setText( QString::number( youngs ) );

	leArea		->setValidator( new QDoubleValidator( leArea ) );
	leYoungs	->setValidator( new QDoubleValidator( leYoungs ) );

	QVBoxLayout *	vbl_main	= new QVBoxLayout( );
	QFormLayout *	fl_main		= new QFormLayout( );
	fl_main ->addRow( tr("Name:"), leName );
	fl_main ->addRow( tr("Color:"), lColor );
	fl_main ->addRow( tr("Area:"), leArea );
	fl_main ->addRow( tr("Young's modulus:"), leYoungs );

	QDialogButtonBox *dbb_confirm = new QDialogButtonBox( QDialogButtonBox::Ok | QDialogButtonBox::Cancel );
    connect( dbb_confirm, SIGNAL(accepted()), this, SLOT(accept()) );
    connect( dbb_confirm, SIGNAL(rejected()), this, SLOT(reject()) );

	vbl_main ->addLayout( fl_main );
	vbl_main ->addWidget( dbb_confirm );
	setLayout( vbl_main );

	connect( lColor, SIGNAL( clicked( ) ), this, SLOT( OnColorChangeClick( ) ) );
}


fEditTrussMaterialDialog::~fEditTrussMaterialDialog(  ) {
	
}


QString fEditTrussMaterialDialog::ReadName( ) {
	return leName ->text( );
}


QColor fEditTrussMaterialDialog::ReadColor( ) {
	return mColor;
}


float fEditTrussMaterialDialog::ReadArea( ) {
	return leArea ->text( ).toFloat( );
}


float fEditTrussMaterialDialog::ReadYoungsModulus( ) {
	return leYoungs ->text( ).toFloat( );
}


void fEditTrussMaterialDialog::OnColorChangeClick( ) {
	QColor color = QColorDialog::getColor( mColor, this );
	if ( color.isValid( ) ) {
		mColor = color;
        lColor ->setPalette( QPalette( mColor ) );
        lColor ->setAutoFillBackground( true );
	}
}












fTrussMaterialsPanel::fTrussMaterialsPanel( fTrussMaterialList *mats, QWidget *parent ) : QWidget( parent ) {
	materials					= NULL;

	// Create layouts
	QVBoxLayout *vbl_main		= new QVBoxLayout( );
	QHBoxLayout *hbl_add_remove = new QHBoxLayout( );
	QFormLayout *fl_props		= new QFormLayout( );

	// Create all necessary objects
	// 1) List view
	lvMaterials			= new QListView( );
	vbl_main ->addWidget( lvMaterials );

	// 2) Add/Remove buttons
	btnAddMaterial		= new QPushButton( tr("Add") );
	btnRemoveMaterial	= new QPushButton( tr("Remove") );
	hbl_add_remove ->addWidget( btnAddMaterial );
	hbl_add_remove ->addStretch( 1 );
	hbl_add_remove ->addWidget( btnRemoveMaterial );
	vbl_main ->addLayout( hbl_add_remove );

	leArea				= new QLineEdit( );
	leYoungsModulus		= new QLineEdit( );
	fl_props ->addRow( tr("Area:"), leArea );
	fl_props ->addRow( tr("Young's Modulus:"), leYoungsModulus );
	vbl_main ->addSpacing( 5 );
	vbl_main ->addLayout( fl_props );

	// Assign the main layout
	this ->setLayout( vbl_main );

	// Setup selection mode
	lvMaterials ->setSelectionMode( QAbstractItemView::SingleSelection );

	SetMaterialList( mats );

	// Setup validators
	leArea			->setValidator( new QDoubleValidator( leArea ) );
	leYoungsModulus ->setValidator( new QDoubleValidator( leYoungsModulus ) );

	// Create sinals/slots
	connect( btnAddMaterial, SIGNAL( clicked( ) ), this, SLOT( OnAddMaterial( ) ) );
	connect( btnRemoveMaterial, SIGNAL( clicked( ) ), this, SLOT( OnRemoveMaterial( ) ) );

	connect( leArea, SIGNAL( textEdited( const QString & ) ), this, SLOT( OnAreaEdited( const QString & ) ) );
	connect( leYoungsModulus, SIGNAL( textEdited( const QString & ) ), this, SLOT( OnYoungsEdited( const QString & ) ) );

	connect( lvMaterials ->selectionModel( ), SIGNAL( selectionChanged( const QItemSelection &, const QItemSelection &) ), this, SLOT( OnTrussMaterialSelected( const QItemSelection &, const QItemSelection & ) ) );
	connect( lvMaterials, SIGNAL( doubleClicked( const QModelIndex & ) ), this, SLOT( OnEditMaterial( const QModelIndex & ) ) );
}


fTrussMaterialsPanel::~fTrussMaterialsPanel(  ) {
	
}


void fTrussMaterialsPanel::SetMaterialList( fTrussMaterialList *mat ) {
	if ( materials ) {
		disconnect( materials, SIGNAL( rowsAboutToBeInserted ( const QModelIndex &, int, int ) ), this, SLOT( OnRowsAboutToBeInserted( const QModelIndex &, int, int ) ) );
		disconnect( materials, SIGNAL( rowsAboutToBeRemoved ( const QModelIndex &, int, int ) ), this, SLOT( OnRowsAboutToBeRemoved( const QModelIndex &, int, int ) ) );
	}

	materials = mat;

	QItemSelectionModel *prev_m = lvMaterials ->selectionModel( );
	lvMaterials ->setModel( materials );
	if ( prev_m ) delete prev_m;

	// Unselect everyone by default
	UpdateSelection( -1 );

	if ( mat ) {
		connect( mat, SIGNAL( rowsAboutToBeInserted ( const QModelIndex &, int, int ) ), this, SLOT( OnRowsAboutToBeInserted( const QModelIndex &, int, int ) ) );
		connect( mat, SIGNAL( rowsAboutToBeRemoved ( const QModelIndex &, int, int ) ), this, SLOT( OnRowsAboutToBeRemoved( const QModelIndex &, int, int ) ) );
	}
}


void fTrussMaterialsPanel::UpdateSelection( int id ) {
	QItemSelectionModel *select_model = lvMaterials ->selectionModel( );

	QModelIndex index = materials ->index( id, 0, QModelIndex( ) );
	select_model ->select( index, QItemSelectionModel::ClearAndSelect );

	lvMaterials ->scrollTo( index );
}


int fTrussMaterialsPanel::GetSelectedMaterialId( ) {
	QItemSelectionModel *select_model = lvMaterials ->selectionModel( );

	if ( !select_model ->hasSelection( ) ) {
		return -1;
	}

	QModelIndexList indexes = select_model ->selectedIndexes( );
	return indexes[0].row( );
}


void fTrussMaterialsPanel::OnTrussMaterialSelected( const QItemSelection &selected, const QItemSelection &deselected ) {
	if ( selected.size( ) <= 0 ) {
		leArea			->setText( tr("") );
		leYoungsModulus ->setText( tr("") );
		return;
	}
	int id		= selected.indexes( ).at( 0 ).row( );
	if ( !deselected.isEmpty( ) ) {
		int id_prev = deselected.indexes( ).at( 0 ).row( );
	}
	float a = materials ->GetMaterial( id ) ->GetArea( );
	float y = materials ->GetMaterial( id ) ->GetYoungsModulus( );

	leArea			->setText( QString::number( a ) );
	leYoungsModulus ->setText( QString::number( y ) );

	fTrussMesh *tmesh = static_cast<fTrussMesh *> ( app ->GetMesh( ) );
	tmesh ->SetBrushMaterialId( id );

	emit MaterialSelected( id );
}


void fTrussMaterialsPanel::OnRowsAboutToBeInserted( const QModelIndex & parent, int start, int end ) {
	UpdateSelection( start );
}


void fTrussMaterialsPanel::OnRowsAboutToBeRemoved( const QModelIndex & parent, int start, int end ) {
	if ( start < 1 ) {
		start = start + 1;
	}
	UpdateSelection( start - 1 );
}


void fTrussMaterialsPanel::OnAreaEdited( const QString &a ) {
	int id = GetSelectedMaterialId( );
	if ( id < 0 ) {
		return;
	}
	materials ->GetMaterial( id ) ->SetArea( a.toFloat( ) );
}


void fTrussMaterialsPanel::OnYoungsEdited( const QString &y ) {
	int id = GetSelectedMaterialId( );
	if ( id < 0 ) {
		return;
	}
	materials ->GetMaterial( id ) ->SetYoungsModulus( y.toFloat( ) );
}


void fTrussMaterialsPanel::OnColorChanged( QColor &clr ) {
	int id = GetSelectedMaterialId( );
	if ( id < 0 ) {
		return;
	}
	materials ->GetMaterial( id ) ->SetColor( clr );
}


void fTrussMaterialsPanel::OnNameChanged( QString &name ) {
	int id = GetSelectedMaterialId( );
	if ( id < 0 ) {
		return;
	}
	materials ->GetMaterial( id ) ->SetName( name );
}


void fTrussMaterialsPanel::OnRemoveMaterial( ) {
	// The mesh should be adjusted after removing materials
	fTrussMesh *tmesh = static_cast<fTrussMesh *> ( app ->GetMesh( ) );
	int id = GetSelectedMaterialId( );

	if ( id < 0 ) {
		id = materials ->rowCount( ) - 1;
	}
	if ( id < 0 || materials ->rowCount( ) == 1 ) {
		return;
	}


	tmesh ->OnMaterialRemoved( id );
	materials ->RemoveMaterial( id );

	// The mesh should be adjusted after removing materials
	//tmesh ->UpdateElementMateials( );

	// Update canvas
	app ->GetMainWindow( ) ->GetCanvas( ) ->Update( );
}


void fTrussMaterialsPanel::OnAddMaterial( ) {
	int		mat_num = materials ->rowCount( );
	QString name	= QString( tr("Material ") ) + QString::number( mat_num + 1 );

	unsigned char r = 255 * float( rand( ) ) / float( RAND_MAX );
	unsigned char g = 255 * float( rand( ) ) / float( RAND_MAX );
	unsigned char b = 255 * float( rand( ) ) / float( RAND_MAX );

	fEditTrussMaterialDialog *dlg = new fEditTrussMaterialDialog( name, QColor( r, g, b ), 1.0f, 1.0f, app ->GetMainWindow( ) );

	int res = dlg ->exec( );

	if ( res ) {
		// Create material
		fTrussMaterial *mat = new fTrussMaterial( dlg ->ReadName( ), dlg ->ReadColor( ), dlg ->ReadArea( ), dlg ->ReadYoungsModulus( ) );
		materials ->AddMaterial( mat );

		UpdateSelection( materials ->rowCount( ) - 1 );
	}

	delete dlg;

	// Update canvas
	app ->GetMainWindow( ) ->GetCanvas( ) ->Update( );
}


void fTrussMaterialsPanel::OnEditMaterial( const QModelIndex &index ) {
	int id = index.row( );
	fTrussMaterial *mat = materials ->GetMaterial( id );

	float	a		= mat ->GetArea( );
	QString name	= mat ->GetName( );
	float	y		= mat ->GetYoungsModulus( );
	QColor	color	= mat ->GetColor( );

	fEditTrussMaterialDialog *dlg = new fEditTrussMaterialDialog( name, color, a, y, app ->GetMainWindow( ) );
	int res = dlg ->exec( );
	if ( res ) {
		// Update material
		mat ->SetName( dlg ->ReadName( ) );
		mat ->SetArea( dlg ->ReadArea( ) );
		mat ->SetYoungsModulus( dlg ->ReadYoungsModulus( ) );
		mat ->SetColor( dlg ->ReadColor( ) );

		// Update edit boxes as well
		leArea			->setText( QString::number( dlg ->ReadArea( ) ) );
		leYoungsModulus ->setText( QString::number( dlg ->ReadYoungsModulus( ) ) );
	}
	delete dlg;

	// Update canvas
	app ->GetMainWindow( ) ->GetCanvas( ) ->Update( );
}


