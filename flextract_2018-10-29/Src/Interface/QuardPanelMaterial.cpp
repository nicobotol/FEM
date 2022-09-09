#include "../precompiled.h"


fRemoveQuardElementsWithMaterialDialog::fRemoveQuardElementsWithMaterialDialog( fQuardMaterialList *materials, QWidget *parent ) : QDialog( parent ) {
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


fRemoveQuardElementsWithMaterialDialog::~fRemoveQuardElementsWithMaterialDialog(  ) {

}


int fRemoveQuardElementsWithMaterialDialog::ReadMaterialId( ) {
	QItemSelectionModel *select_model = lvMaterials ->selectionModel( );
	if ( !select_model ->hasSelection( ) ) {
		return -1;
	}
	QModelIndexList indexes = select_model ->selectedIndexes( );
	return indexes[0].row( );
}



void fRemoveQuardElementsWithMaterialDialog::OnMaterialDoubleClick( const QModelIndex & ) {
	this ->accept( );
}





fEditQuardMaterialDialog::fEditQuardMaterialDialog( QString &name, const QColor &color, float thickness, float ex, float poisson,
											float dens, bool isotropic, float ey, float gxy, QWidget *parent ) : QDialog( parent ) {
	// Quard mesh panel and its contents
	lColor						= new fClickableLabel( );
	leName						= new QLineEdit( );
	leThickness					= new QLineEdit( );
	leEx						= new QLineEdit( );
	leEy						= new QLineEdit( );
	leGxy						= new QLineEdit( );
	leDens						= new QLineEdit( );
	lePoisson					= new QLineEdit( );

	mColor = color;
	lColor ->setPalette( QPalette(color) );
    lColor ->setAutoFillBackground (true );
	lColor ->setMouseTracking( true );

	leName		->setText( name );
	leEx		->setText( QString::number( ex ) );
	leEy		->setText( QString::number( ey ) );
	leDens		->setText( QString::number( dens ) );
	leGxy		->setText( QString::number( gxy ) );
	lePoisson	->setText( QString::number( poisson ) );
	leThickness	->setText( QString::number( thickness ) );

	leThickness	->setValidator( new QDoubleValidator( leThickness ) );
	leEx		->setValidator( new QDoubleValidator( leEx ) );
	leEy		->setValidator( new QDoubleValidator( leEy ) );
	leGxy		->setValidator( new QDoubleValidator( leGxy ) );
	leDens		->setValidator( new QDoubleValidator( leDens ) );
	lePoisson	->setValidator( new QDoubleValidator( lePoisson ) );

	// Main layout
	QVBoxLayout *	vbl_main	= new QVBoxLayout( );

	// Name and color layout
	QFormLayout *	fl_main		= new QFormLayout( );
	fl_main ->addRow( tr("Name:"), leName );
	fl_main ->addRow( tr("Color:"), lColor );

	lThickness					= new QLabel( tr("Thickness: ") );
	lEx							= new QLabel( tr("Young's modulus X: ") );
	lEy							= new QLabel( tr("Young's modulus Y: ") );
	lGxy						= new QLabel( tr("Shear modulus XY: ") );
	lDens						= new QLabel( tr("Density: ") );
	lPoisson					= new QLabel( tr("Poisson's ratio: ") );

	lThickness	->setBuddy( leThickness );
	lEx			->setBuddy( leEx );
	lEy			->setBuddy( leEy );
	lGxy		->setBuddy( leGxy );
	lDens		->setBuddy( leDens );
	lPoisson	->setBuddy( lePoisson );

	QGridLayout *gridLayout = new QGridLayout;
	gridLayout ->addWidget( lThickness,  0, 0);
	gridLayout ->addWidget( leThickness, 0, 1);
	gridLayout ->addWidget( lEx,  1, 0);
	gridLayout ->addWidget( leEx, 1, 1);
	gridLayout ->addWidget( lEy,  2, 0);
	gridLayout ->addWidget( leEy, 2, 1);
	gridLayout ->addWidget( lGxy,  3, 0);
	gridLayout ->addWidget( leGxy, 3, 1);
	gridLayout ->addWidget( lDens,  4, 0);
	gridLayout ->addWidget( leDens, 4, 1);
	gridLayout ->addWidget( lPoisson,  5, 0);
	gridLayout ->addWidget( lePoisson, 5, 1);

	QDialogButtonBox *dbb_confirm = new QDialogButtonBox( QDialogButtonBox::Ok | QDialogButtonBox::Cancel );
    connect( dbb_confirm, SIGNAL(accepted()), this, SLOT(accept()) );
    connect( dbb_confirm, SIGNAL(rejected()), this, SLOT(reject()) );

	rbIsotropic		= new QRadioButton(tr("Isotropic"));
	rbAnisotropic	= new QRadioButton(tr("Anisotropic"));

	QHBoxLayout *hbox = new QHBoxLayout( );
	hbox ->addWidget( rbIsotropic );
	hbox ->addWidget( rbAnisotropic );
	hbox ->addStretch( 1 );

	vbl_main ->addLayout( fl_main );
	vbl_main ->addSpacing( 5 );
	vbl_main ->addLayout( hbox );
	vbl_main ->addSpacing( 5 );
	vbl_main ->addLayout( gridLayout );
	vbl_main ->addSpacing( 5 );
	vbl_main ->addStretch( 1 );
	vbl_main ->addWidget( dbb_confirm );
	setLayout( vbl_main );

	// Set Isotropic 
	if ( isotropic ) {
		leGxy		->hide( );
		leEy		->hide( );
		lGxy		->hide( );
		lEy			->hide( );
		lEx			->setText( tr("Young's modulus: ") );
	}


	connect( lColor, SIGNAL( clicked( ) ), this, SLOT( OnColorChangeClick( ) ) );
	connect( rbIsotropic, SIGNAL( toggled( bool ) ), this, SLOT( OnRBIsotropicClick( bool ) ) );
	connect( rbAnisotropic, SIGNAL( toggled( bool ) ), this, SLOT( OnRBAnisotropicClick( bool ) ) );


	if ( isotropic ) {
		rbIsotropic ->setChecked( true );
		OnRBIsotropicClick( true );
	} else {
		rbAnisotropic ->setChecked( true );
		OnRBAnisotropicClick( true );
	}
}


fEditQuardMaterialDialog::~fEditQuardMaterialDialog(  ) {
	
}


QString fEditQuardMaterialDialog::ReadName( ) {
	return leName ->text( );
}


QColor fEditQuardMaterialDialog::ReadColor( ) {
	return mColor;
}


float fEditQuardMaterialDialog::ReadThickness( ) {
	return leThickness ->text( ).toFloat( );
}


float fEditQuardMaterialDialog::ReadEx( ) {
	return leEx ->text( ).toFloat( );
}


float fEditQuardMaterialDialog::ReadEy( ) {
	return leEy ->text( ).toFloat( );
}


float fEditQuardMaterialDialog::ReadGxy( ) {
	return leGxy ->text( ).toFloat( );
}


float fEditQuardMaterialDialog::ReadDens( ) {
	return leDens ->text( ).toFloat( );
}


float fEditQuardMaterialDialog::ReadPoisson( ) {
	return lePoisson ->text( ).toFloat( );
}


bool fEditQuardMaterialDialog::ReadIsotropicFlag( ) {
	return rbIsotropic ->isChecked( );
}


void fEditQuardMaterialDialog::OnColorChangeClick( ) {
	QColor color = QColorDialog::getColor( mColor, this );
	if ( color.isValid( ) ) {
		mColor = color;
        lColor ->setPalette( QPalette( mColor ) );
        lColor ->setAutoFillBackground( true );
	}
}


void fEditQuardMaterialDialog::OnRBIsotropicClick( bool checked ) {
	if ( checked ) {
		leGxy		->hide( );
		leEy		->hide( );
		lGxy		->hide( );
		lEy			->hide( );
		lEx			->setText( tr("Young's modulus: ") );
	}
}


void fEditQuardMaterialDialog::OnRBAnisotropicClick( bool checked ) {
	if ( checked ) {
		leGxy		->show( );
		leEy		->show( );
		lGxy		->show( );
		lEy			->show( );
		lEx			->setText( tr("Young's modulus X: ") );
	}
}










fQuardMaterialsPanel::fQuardMaterialsPanel( fQuardMaterialList *mats, QWidget *parent ) : QWidget( parent ) {
	materials					= NULL;

	// Create layouts
	QVBoxLayout *vbl_main		= new QVBoxLayout( );
	QHBoxLayout *hbl_add_remove = new QHBoxLayout( );

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

	leThickness					= new QLineEdit( );
	leEx						= new QLineEdit( );
	leEy						= new QLineEdit( );
	leGxy						= new QLineEdit( );
	leDens						= new QLineEdit( );
	lePoisson					= new QLineEdit( );

	lThickness					= new QLabel( tr("Thickness: ") );
	lEx							= new QLabel( tr("Young's modulus X: ") );
	lEy							= new QLabel( tr("Young's modulus Y: ") );
	lGxy						= new QLabel( tr("Shear modulus XY: ") );
	lDens						= new QLabel( tr("Density: ") );
	lPoisson					= new QLabel( tr("Poisson's ratio: ") );

	lThickness	->setBuddy( leThickness );
	lEx			->setBuddy( leEx );
	lEy			->setBuddy( leEy );
	lGxy		->setBuddy( leGxy );
	lDens		->setBuddy( leDens );
	lPoisson	->setBuddy( lePoisson );

	QGridLayout *gridLayout = new QGridLayout;
	gridLayout ->addWidget( lThickness,  0, 0);
	gridLayout ->addWidget( leThickness, 0, 1);
	gridLayout ->addWidget( lEx,  1, 0);
	gridLayout ->addWidget( leEx, 1, 1);
	gridLayout ->addWidget( lEy,  2, 0);
	gridLayout ->addWidget( leEy, 2, 1);
	gridLayout ->addWidget( lGxy,  3, 0);
	gridLayout ->addWidget( leGxy, 3, 1);
	gridLayout ->addWidget( lDens,  4, 0);
	gridLayout ->addWidget( leDens, 4, 1);
	gridLayout ->addWidget( lPoisson,  5, 0);
	gridLayout ->addWidget( lePoisson, 5, 1);
	

	rbIsotropic		= new QRadioButton(tr("Isotropic"));
	rbAnisotropic	= new QRadioButton(tr("Anisotropic"));

	QHBoxLayout *hbox = new QHBoxLayout( );
	hbox ->addWidget( rbIsotropic );
	hbox ->addWidget( rbAnisotropic );
	hbox ->addStretch( 1 );

	vbl_main ->addSpacing( 5 );
	vbl_main ->addLayout( hbox );
	vbl_main ->addSpacing( 5 );
	vbl_main ->addLayout( gridLayout );

	// Set Isotropic by default
	leGxy		->hide( );
	leEy		->hide( );
	lGxy		->hide( );
	lEy			->hide( );
	lEx			->setText( tr("Young's modulus: ") );
	rbIsotropic ->setChecked( true );

	// Assign the main layout
	this ->setLayout( vbl_main );

	// Setup selection mode
	lvMaterials ->setSelectionMode( QAbstractItemView::SingleSelection );

	SetMaterialList( mats );

	// Setup validators
	leThickness	->setValidator( new QDoubleValidator( leThickness ) );
	leEx		->setValidator( new QDoubleValidator( leEx ) );
	leEy		->setValidator( new QDoubleValidator( leEy ) );
	leGxy		->setValidator( new QDoubleValidator( leGxy ) );
	leDens		->setValidator( new QDoubleValidator( leDens ) );
	lePoisson	->setValidator( new QDoubleValidator( lePoisson ) );

	// Create sinals/slots
	connect( btnAddMaterial, SIGNAL( clicked( ) ), this, SLOT( OnAddMaterial( ) ) );
	connect( btnRemoveMaterial, SIGNAL( clicked( ) ), this, SLOT( OnRemoveMaterial( ) ) );

	connect( leThickness, SIGNAL( textEdited( const QString & ) ), this, SLOT( OnThicknessEdited( const QString & ) ) );
	connect( leEx, SIGNAL( textEdited( const QString & ) ), this, SLOT( OnExEdited( const QString & ) ) );
	connect( leEy, SIGNAL( textEdited( const QString & ) ), this, SLOT( OnEyEdited( const QString & ) ) );
	connect( leGxy, SIGNAL( textEdited( const QString & ) ), this, SLOT( OnGxyEdited( const QString & ) ) );
	connect( leDens, SIGNAL( textEdited( const QString & ) ), this, SLOT( OnDensEdited( const QString & ) ) );
	connect( lePoisson, SIGNAL( textEdited( const QString & ) ), this, SLOT( OnPoissonEdited( const QString & ) ) );

	connect( lvMaterials ->selectionModel( ), SIGNAL( selectionChanged( const QItemSelection &, const QItemSelection &) ), this, SLOT( OnQuardMaterialSelected( const QItemSelection &, const QItemSelection & ) ) );
	connect( lvMaterials, SIGNAL( doubleClicked( const QModelIndex & ) ), this, SLOT( OnEditMaterial( const QModelIndex & ) ) );

	connect( rbIsotropic, SIGNAL( toggled( bool ) ), this, SLOT( OnRBIsotropicClick( bool ) ) );
	connect( rbAnisotropic, SIGNAL( toggled( bool ) ), this, SLOT( OnRBAnisotropicClick( bool ) ) );
}


fQuardMaterialsPanel::~fQuardMaterialsPanel(  ) {
	
}


void fQuardMaterialsPanel::SetMaterialList( fQuardMaterialList *mat ) {
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


void fQuardMaterialsPanel::UpdateSelection( int id ) {
	QItemSelectionModel *select_model = lvMaterials ->selectionModel( );

	QModelIndex index = materials ->index( id, 0, QModelIndex( ) );
	select_model ->select( index, QItemSelectionModel::ClearAndSelect );

	lvMaterials ->scrollTo( index );
}


int fQuardMaterialsPanel::GetSelectedMaterialId( ) {
	QItemSelectionModel *select_model = lvMaterials ->selectionModel( );

	if ( !select_model ->hasSelection( ) ) {
		return -1;
	}

	QModelIndexList indexes = select_model ->selectedIndexes( );
	return indexes[0].row( );
}


void fQuardMaterialsPanel::OnQuardMaterialSelected( const QItemSelection &selected, const QItemSelection &deselected ) {
	if ( selected.size( ) <= 0 ) {
		leEx		->setText( tr("") );
		leEy		->setText( tr("") );
		leDens		->setText( tr("") );
		leGxy		->setText( tr("") );
		lePoisson	->setText( tr("") );
		leThickness	->setText( tr("") );
		return;
	}
	int id		= selected.indexes( ).at( 0 ).row( );
	if ( !deselected.isEmpty( ) ) {
		int id_prev = deselected.indexes( ).at( 0 ).row( );
	}
	float ex		= materials ->GetMaterial( id ) ->GetEx( );
	float ey		= materials ->GetMaterial( id ) ->GetEy( );
	float dens		= materials ->GetMaterial( id ) ->GetDens( );
	float gxy		= materials ->GetMaterial( id ) ->GetGxy( );
	float poisson	= materials ->GetMaterial( id ) ->GetPoisson( );
	float thickness = materials ->GetMaterial( id ) ->GetThickness( );

	leEx		->setText( QString::number( ex ) );
	leEy		->setText( QString::number( ey ) );
	leDens		->setText( QString::number( dens ) );
	leGxy		->setText( QString::number( gxy ) );
	lePoisson	->setText( QString::number( poisson ) );
	leThickness	->setText( QString::number( thickness ) );


	bool isotropic  = materials ->GetMaterial( id ) ->GetIsotropic( );
	if ( isotropic ) {
		rbIsotropic ->setChecked( true );
		OnRBIsotropicClick( true );
	} else {
		rbAnisotropic ->setChecked( true );
		OnRBAnisotropicClick( true );
	}

	fQuardMesh *tmesh = static_cast<fQuardMesh *> ( app ->GetMesh( ) );
	tmesh ->SetBrushMaterialId( id );

	emit MaterialSelected( id );
}


void fQuardMaterialsPanel::OnRowsAboutToBeInserted( const QModelIndex & parent, int start, int end ) {
	UpdateSelection( start );
}


void fQuardMaterialsPanel::OnRowsAboutToBeRemoved( const QModelIndex & parent, int start, int end ) {
	if ( start < 1 ) {
		start = start + 1;
	}
	UpdateSelection( start - 1 );
}


void fQuardMaterialsPanel::OnThicknessEdited( const QString &a ) {
	int id = GetSelectedMaterialId( );
	if ( id < 0 ) {
		return;
	}
	materials ->GetMaterial( id ) ->SetThickness( a.toFloat( ) );
}


void fQuardMaterialsPanel::OnExEdited( const QString &a ) {
	int id = GetSelectedMaterialId( );
	if ( id < 0 ) {
		return;
	}
	materials ->GetMaterial( id ) ->SetEx( a.toFloat( ) );
}

void fQuardMaterialsPanel::OnEyEdited( const QString &a ) {
	int id = GetSelectedMaterialId( );
	if ( id < 0 ) {
		return;
	}
	materials ->GetMaterial( id ) ->SetEy( a.toFloat( ) );
}

void fQuardMaterialsPanel::OnGxyEdited( const QString &a ) {
	int id = GetSelectedMaterialId( );
	if ( id < 0 ) {
		return;
	}
	materials ->GetMaterial( id ) ->SetGxy( a.toFloat( ) );
}

void fQuardMaterialsPanel::OnPoissonEdited( const QString &a ) {
	int id = GetSelectedMaterialId( );
	if ( id < 0 ) {
		return;
	}
	materials ->GetMaterial( id ) ->SetPoisson( a.toFloat( ) );
}

void fQuardMaterialsPanel::OnDensEdited( const QString &a ) {
	int id = GetSelectedMaterialId( );
	if ( id < 0 ) {
		return;
	}
	materials ->GetMaterial( id ) ->SetDens( a.toFloat( ) );
}


void fQuardMaterialsPanel::OnColorChanged( QColor &clr ) {
	int id = GetSelectedMaterialId( );
	if ( id < 0 ) {
		return;
	}
	materials ->GetMaterial( id ) ->SetColor( clr );
}


void fQuardMaterialsPanel::OnNameChanged( QString &name ) {
	int id = GetSelectedMaterialId( );
	if ( id < 0 ) {
		return;
	}
	materials ->GetMaterial( id ) ->SetName( name );
}



void fQuardMaterialsPanel::OnRemoveMaterial( ) {
	// The mesh should be adjusted after removing materials
	fQuardMesh *tmesh = static_cast<fQuardMesh *> ( app ->GetMesh( ) );
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


void fQuardMaterialsPanel::OnAddMaterial( ) {
	int		mat_num = materials ->rowCount( );
	QString name	= QString( tr("Material ") ) + QString::number( mat_num + 1 );

	unsigned char r = 255 * float( rand( ) ) / float( RAND_MAX );
	unsigned char g = 255 * float( rand( ) ) / float( RAND_MAX );
	unsigned char b = 255 * float( rand( ) ) / float( RAND_MAX );

	fEditQuardMaterialDialog *dlg = new fEditQuardMaterialDialog( name, QColor( r, g, b ), 1.0f, 1.0f, 0.3f, 1000, true, 0.0f, 0.0f, app ->GetMainWindow( ) );

	int res = dlg ->exec( );

	if ( res ) {
		// Create material
		fQuardMaterial *mat = new fQuardMaterial(	dlg ->ReadName( ),
													dlg ->ReadColor( ), 
													dlg ->ReadThickness( ),
													dlg ->ReadEx( ),
													dlg ->ReadPoisson( ),
													dlg ->ReadDens( ),
													dlg ->ReadIsotropicFlag( ) 
												);
		materials ->AddMaterial( mat );

		UpdateSelection( materials ->rowCount( ) - 1 );
	}

	delete dlg;

	// Update canvas
	app ->GetMainWindow( ) ->GetCanvas( ) ->Update( );
}


void fQuardMaterialsPanel::OnEditMaterial( const QModelIndex &index ) {
	int id = index.row( );
	fQuardMaterial *mat = materials ->GetMaterial( id );

	QString name		= mat ->GetName( );
	QColor	color		= mat ->GetColor( );
	float	Ex			= mat ->GetEx( );
	float	Ey			= mat ->GetEy( );
	float	Gxy			= mat ->GetGxy( );
	float	Poisson		= mat ->GetPoisson( );
	float	Dens		= mat ->GetDens( );
	float	Thickness	= mat ->GetThickness( );
	bool	isotropic	= mat ->GetIsotropic( );

	fEditQuardMaterialDialog *dlg = new fEditQuardMaterialDialog( name, color, Thickness, Ex, Poisson, Dens, isotropic, Ey, Gxy, app ->GetMainWindow( ) );
	int res = dlg ->exec( );
	if ( res ) {
		// Update material
		mat ->SetName		( dlg ->ReadName( ) );
		mat ->SetColor		( dlg ->ReadColor( ) );
		mat ->SetEx			( dlg ->ReadEx( ) );
		mat ->SetEy			( dlg ->ReadEy( ) );
		mat ->SetGxy		( dlg ->ReadGxy( ) );
		mat ->SetPoisson	( dlg ->ReadPoisson( ) );
		mat ->SetDens		( dlg ->ReadDens( ) );
		mat ->SetThickness	( dlg ->ReadThickness( ) );
		mat ->SetIsotropic	( dlg ->ReadIsotropicFlag( ) );


		// Update edit boxes as well
		leEx		->setText( QString::number( dlg ->ReadEx( ) ) );
		leEy		->setText( QString::number( dlg ->ReadEy( ) ) );
		leDens		->setText( QString::number( dlg ->ReadDens( ) ) );
		leGxy		->setText( QString::number( dlg ->ReadGxy( ) ) );
		lePoisson	->setText( QString::number( dlg ->ReadPoisson( ) ) );
		leThickness	->setText( QString::number( dlg ->ReadThickness( ) ) );
		if ( dlg ->ReadIsotropicFlag( ) ) {
			rbIsotropic ->setChecked( true );
			OnRBIsotropicClick( true );
		} else {
			rbAnisotropic ->setChecked( true );
			OnRBAnisotropicClick( true );
		}
	}
	delete dlg;

	// Update canvas
	app ->GetMainWindow( ) ->GetCanvas( ) ->Update( );
}


void fQuardMaterialsPanel::OnRBIsotropicClick( bool checked ) {
	int id = GetSelectedMaterialId( );
	if ( id < 0 ) {
		return;
	}

	if ( checked ) {
		materials	->GetMaterial( id ) ->SetIsotropic( true );
		//leGxy		->setEnabled( false );
		//leEy		->setEnabled( false );
		leGxy		->hide( );
		leEy		->hide( );
		lGxy		->hide( );
		lEy			->hide( );
		lEx			->setText( tr("Young's modulus: ") );
	}
}


void fQuardMaterialsPanel::OnRBAnisotropicClick( bool checked ) {
	int id = GetSelectedMaterialId( );
	if ( id < 0 ) {
		return;
	}

	if ( checked ) {
		materials	->GetMaterial( id ) ->SetIsotropic( false );
		//leGxy		->setEnabled( true );
		//leEy		->setEnabled( true );
		leGxy		->show( );
		leEy		->show( );
		lGxy		->show( );
		lEy			->show( );
		lEx			->setText( tr("Young's modulus X: ") );
	}
}
