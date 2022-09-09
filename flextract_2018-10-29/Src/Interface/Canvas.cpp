#include "../precompiled.h"




fCanvas::fCanvas( ) {
	setFocusPolicy( Qt::StrongFocus );

	spacePressed			= false;

	leftMouseBtnPressed		= false;
	rightMouseBtnPressed	= false;
	mouseInside				= true;
	drawBrush				= false;
	

	ReadSettings( );

	mousePos				= QPoint( 0, 0 );
	setMouseTracking( true );

	// Initialize cursors
	setCursor( Qt::ArrowCursor );
	pixmapAim				= QPixmap( ":/Cursors/cur_aim.png" );
	pixmapBrush				= QPixmap( app ->GetBrush( ) ->GetMaxDiameter( ) + 4, app ->GetBrush( ) ->GetMaxDiameter( ) + 4 );
	cursorAim				= QCursor( pixmapAim );
	// Update cursors
	OnBrushSizeChange( app ->GetBrush( ) ->GetDiameter( ) );

	mouseZoomFrom			= 1.0f;
	mouseZoomTo				= 1.0f;
	mouseZoomSpace			= 0.001f;
	mouseZoomUpdatePeriod	= 100;
	mouseZoomUpdateFreq		= 10;

	backgroundColor			= QColor( 138, 139, 141 );
	cursorColor				= QColor( 250, 252, 255 );

	connect( &mouseZoomTimer, SIGNAL( timeout( ) ), this, SLOT( OnMouseZoomTimer( ) ) );
	connect( app ->GetBrush( ), SIGNAL( sizeChanged( int ) ), this, SLOT( OnBrushSizeChange( int ) ) );
}


fCanvas::~fCanvas( ) {
	WriteSettings( );
}


void fCanvas::initializeGL( ) {
	QColor c = backgroundColor;
	glClearColor( float(c.red( )) / 255.0f, float(c.green( )) / 255.0f, float(c.blue( )) / 255.0f, 0.0f );
	glShadeModel( GL_FLAT );
}


void fCanvas::paintGL( ) {
	// Crear back buffer
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

	// Setup camera
	fBounds2 proj	= camera.GetProjection( );
	float	 zoom	= camera.GetZoom( );

	glMatrixMode( GL_PROJECTION );
	glLoadIdentity( );
	gluOrtho2D( proj[0].x, proj[1].x, proj[0].y, proj[1].y );
	glMatrixMode( GL_MODELVIEW );
	glLoadIdentity( );

	// Draw the mesh
	if ( app ->GetMesh( ) ) {
		app ->GetMesh( ) ->Draw( &camera );

		// Display additional information if mesh is active
		glMatrixMode( GL_PROJECTION );
		glLoadIdentity( );
		gluOrtho2D( 0, this ->width( ), 0, this ->height( ) );
		glMatrixMode( GL_MODELVIEW );
		glLoadIdentity( );

		QFont			font( "CourierNew", 10 );
		QFontMetrics	fm( font );

		fVec2 mouse_model_pos = camera.GetModelPositionAt( mousePos.x( ), mousePos.y( ) );

		QString	str1 = "x: " + QString::number( mouse_model_pos.x, 'g', 3 );
		QString	str2 = "y: " + QString::number( mouse_model_pos.y, 'g', 3 );
		QString str = str2;

		int pixelsWide = fm.width( str );
		int pixelsHigh = fm.height( );

		glColor4f( 1.0f, 1.0f, 1.0f, 1.0f );
		renderText( 0 + 10, 3 + 1*pixelsHigh + 0, str1, font, 0 );
		renderText( 0 + 10, 3 + 2*pixelsHigh + 0, str2, font, 0 );


		// Draw cursor
		if ( softwareCursor && mouseInside && drawBrush ) {
			float	d	= app ->GetBrush( ) ->GetDiameter( );
			QPointF	c	= app ->GetBrush( ) ->GetPosition( );
			float	r	= d / 2.0f;

			if ( d > 12 ) {
				glMatrixMode( GL_PROJECTION );
				glLoadIdentity( );
				gluOrtho2D( 0, this ->width( ), 0, this ->height( ) );
				glMatrixMode( GL_MODELVIEW );
				glLoadIdentity( );

				int		n	= 100;
				float	da	= 2.0f * M_PI / float( n );

				glColor3ub( 255, 255, 255 );
				glBegin( GL_LINE_LOOP );
				for ( int i = 0; i < n; i++ ) {
					float a = float( i ) * da;
					glVertex2f( c.x() + r*cos( a ) + 0.5, this ->height( ) - c.y() + r*sin( a ) - 0.5 );
				}
				glEnd( );
			}
		}
	}
}


void fCanvas::DrawCircle( fVec2 &c, float r ) {
	int		n	= 100;
	float	da	= 2.0 * M_PI / float( n );

	glBegin( GL_LINE_LOOP );
	for ( int i = 0; i < n; i++ ) {
		float a = float(i) * da;
		glVertex2f( c.x + r*cos( a ), c.y + r*sin( a ) );
	}
	glEnd( );
}


void fCanvas::resizeGL( int width, int height ) {
	camera.UpdateViewport( width, height );
	glViewport( 0, 0, width, height );
}


void fCanvas::SetupView( fBounds2 &bounds ) {
	float	w1 = float( this ->width( ) );
	float	h1 = float( this ->height( ) );
	float	a1 = w1 / h1;

	float	w2 = bounds[1].x - bounds[0].x;
	float	h2 = bounds[1].y - bounds[0].y;
	float	a2 = w2 / h2;

	fBounds2 bnd;
	if ( a1 > a2 ) {
		bnd[0].y = bounds[0].y;
		bnd[1].y = bounds[1].y;

		bnd[0].x = ( bounds[0].x + bounds[1].x ) / 2.0f - ( bounds[1].x - bounds[0].x ) / 2.0f * (a1 / a2);
		bnd[1].x = ( bounds[0].x + bounds[1].x ) / 2.0f + ( bounds[1].x - bounds[0].x ) / 2.0f * (a1 / a2);
	} else {
		bnd[0].x = bounds[0].x;
		bnd[1].x = bounds[1].x;

		bnd[0].y = ( bounds[0].y + bounds[1].y ) / 2.0f - ( bounds[1].y - bounds[0].y ) / 2.0f * (a2 / a1);
		bnd[1].y = ( bounds[0].y + bounds[1].y ) / 2.0f + ( bounds[1].y - bounds[0].y ) / 2.0f * (a2 / a1);
	}

	//camera.ResetZoom( );
	camera.SetupViewport( this ->width( ), this ->height( ), bnd );
	mouseZoomFrom	= 1.0f;
	mouseZoomTo		= 1.0f;
	UpdateCursorShape( );
}


void fCanvas::UpdateCursorShape( ) {
	if ( app ->GetStatus( ) == APPS_PREPARE ) {
		setCursor( Qt::ArrowCursor );
		drawBrush = false;
		return;
	} 
	if ( spacePressed ) {
		if ( leftMouseBtnPressed ) {
			setCursor( Qt::ClosedHandCursor );
		} else {
			setCursor( Qt::OpenHandCursor );
		}
		drawBrush = false;
	} else {
		fMeshType_e mtype = app ->GetMesh( ) ->GetType( );
		switch ( mtype ) {
			case MT_TRUSS : {
				fTrussMesh *tmesh = static_cast<fTrussMesh *> ( app ->GetMesh( ) );
				if ( tmesh ->GetCurrentEditOperation( ) == TME_SELECTION ) {
					setCursor( cursorAim );
					drawBrush = false;
				} else {
					if ( !softwareCursor ) {
						setCursor( cursorBrush );
					} else {
						setCursor( cursorAim );
					}
					drawBrush = true;
				}
				break;
			}
			case MT_CONTINUUM : {
				fQuardMesh *tmesh = static_cast<fQuardMesh *> ( app ->GetMesh( ) );
				if ( tmesh ->GetCurrentEditOperation( ) == QME_SELECTION ) {
					setCursor( cursorAim );
					drawBrush = false;
				} else {
					if ( !softwareCursor ) {
						setCursor( cursorBrush );
					} else {
						setCursor( cursorAim );
					}
					drawBrush = true;
				}
				break;
			}
		}
	}

	if ( softwareCursor ) {
		updateGL( );
	}
}


void fCanvas::keyPressEvent( QKeyEvent *event ) {
	switch ( event ->key( ) ) {
		case Qt::Key_Space: {
			if ( event ->isAutoRepeat( ) ) {
				break;
			}
			spacePressed = true;
			UpdateCursorShape( );
			break;
		}
		default: {
			QWidget::keyPressEvent(event);
		}
    }
}


void fCanvas::keyReleaseEvent( QKeyEvent *event ) {
	switch ( event ->key( ) ) {
		case Qt::Key_Space: {
			if ( event ->isAutoRepeat( ) ) {
				break;
			}
			spacePressed = false;
			UpdateCursorShape( );
			break;
		}
		default: {
			QWidget::keyReleaseEvent( event );
		}
    }
}


void fCanvas::mousePressEvent( QMouseEvent *event ) {
	switch ( event ->button( ) ) {
		case Qt::LeftButton: {
			leftMouseBtnPressed = true;
			UpdateCursorShape( );
			break;
		}
		case Qt::RightButton: {
			rightMouseBtnPressed = true;
			UpdateCursorShape( );
			break;
		}
		default: {
			QWidget::mousePressEvent( event );
		}
    }

	event ->pos( ).x( );
	app ->GetBrush( ) ->SetPosition( QPoint( event ->pos( ) ) );
	
	// If there is a model, let it handle the event as well
	if ( app ->GetStatus( ) != APPS_PREPARE && !spacePressed ) {
		app ->GetMesh( ) ->OnMousePress( event, &camera );
	}

	// Redraw the canvas
	updateGL( );
}


void fCanvas::mouseReleaseEvent( QMouseEvent *event ) {
	switch ( event ->button( ) ) {
		case Qt::LeftButton: {
			leftMouseBtnPressed = false;
			UpdateCursorShape( );
			break;
		}
		case Qt::RightButton: {
			rightMouseBtnPressed = false;
			UpdateCursorShape( );
			break;
		}
		default: {
			QWidget::mousePressEvent( event );
		}
    }

	// If there is a model, let it handle the event as well
	if ( app ->GetStatus( ) != APPS_PREPARE && !spacePressed ) {
		app ->GetMesh( ) ->OnMouseRelease( event, &camera  );
	}

	// Redraw the canvas
	updateGL( );
}


void fCanvas::leaveEvent( QEvent * event ) {
	mouseInside = false;

	// Redraw the canvas
	updateGL( );
}


void fCanvas::enterEvent( QEvent * event ) {
	mouseInside = true;

	// Redraw the canvas
	//updateGL( );
}

void fCanvas::mouseMoveEvent( QMouseEvent *event ) {
	int x = event ->x( );
	int y = event ->y( );

	int dx = x - mousePos.x( );
	int dy = y - mousePos.y( );

	if ( leftMouseBtnPressed ) {
		if ( spacePressed ) {
  			fVec2 vec( -dx * camera.GetAspect( ), dy * camera.GetAspect( ) );
			vec /= camera.GetZoom( );
			camera.Translate( vec );
		}
	} else {
		
	}


	app ->GetBrush( ) ->SetPosition( QPoint( x, y ) );
	mousePos.setX( x );
	mousePos.setY( y );

	// If there is a model, let it handle the event as well
	if ( app ->GetStatus( ) != APPS_PREPARE && !spacePressed ) {
		app ->GetMesh( ) ->OnMouseMove( event, &camera  );
	}

	// Redraw the canvas
	if ( !mouseZoomTimer.isActive( ) ) {
		updateGL( );
	}
}


void fCanvas::wheelEvent( QWheelEvent *event ) {
	if ( app ->GetStatus( ) == APPS_PREPARE ) {
		return;
	}
	if ( spacePressed ) {
		float delta			= event ->delta( );
		mouseZoomFrom		= camera.GetZoom( );

		if ( delta > 0 ) {
			mouseZoomTo *= ( 1.0f + delta * mouseZoomSpace );
		} else {
			mouseZoomTo /= ( 1.0f - delta * mouseZoomSpace );
		}
		
		mouseZoomTime.start( );
		mouseZoomTimer.start( mouseZoomUpdateFreq );
	} else {
		int d0				= app ->GetBrush( ) ->GetDiameter( );
		int q				= event ->delta( ) > 0 ? 1 : -1;
		int delta			= float( event ->delta( ) ) / 1000.0f * float( d0 ) + q;
		app ->GetBrush( ) ->SetDiameter( d0 + delta );
	}

	updateGL( );
}


void fCanvas::OnMouseZoomTimer( ) {
	// Calculate current zoom
	float dt			= mouseZoomTime.elapsed( );
	float mouseZoomCurr	= mouseZoomFrom + ( mouseZoomTo - mouseZoomFrom ) / float( mouseZoomUpdatePeriod ) * dt;
	
	if ( (mouseZoomCurr >= mouseZoomTo) && (mouseZoomTo >= mouseZoomFrom) ) {
		mouseZoomCurr = mouseZoomTo;
		mouseZoomTimer.stop( );
	}

	if ( (mouseZoomCurr <= mouseZoomTo) && (mouseZoomTo <= mouseZoomFrom) ) {
		mouseZoomCurr = mouseZoomTo;
		mouseZoomTimer.stop( );
	}

	// Coordinate
	fVec2 pos_to = camera.GetModelPositionAt( mousePos.x( ), mousePos.y( ) );
	camera.Zoom( mouseZoomCurr, pos_to );

	// Redraw the canvas
	updateGL( );
}


void fCanvas::OnBrushSizeChange( int d ) {
	int		r			= d / 2;
	int		pixmap_size = pixmapBrush.size( ).width( );

	if ( app ->GetBrush( ) ->GetDiameter( ) < 12 ) {
		cursorBrush = cursorAim;
	} else {
		if ( !softwareCursor ) {
			pixmapBrush.fill( Qt::transparent );

			QPainter p( &pixmapBrush );
			QPen pen;
			pen.setWidth( 3 );
			pen.setColor( QColor(0, 0, 0, 50) );
			p.setPen( pen );
			p.drawEllipse( QPointF( float(pixmap_size)/2.0f, float(pixmap_size)/2.0f ), r, r );
			p.setPen( Qt::white );
			p.drawEllipse( QPointF( float(pixmap_size)/2.0f, float(pixmap_size)/2.0f ), r, r );
			p.drawPixmap( QPointF( float(pixmap_size)/2.0f - pixmapAim.size( ).width( )/2, float(pixmap_size)/2.0f - pixmapAim.size( ).height( )/2 ), pixmapAim );
			cursorBrush = QCursor( pixmapBrush, -1, -1 );
		}
	}

	UpdateCursorShape( );
}



void fCanvas::ReadSettings( ) {
	// Get application settings
	QSettings * settings = app ->GetSettings( );

	settings ->beginGroup( "canvas" );
		softwareCursor	= settings ->value( "software_cursor", 1 ).toBool( );
	settings ->endGroup( );
}


void fCanvas::WriteSettings( ) {
	QSettings * settings = app ->GetSettings( );

	settings ->beginGroup( "canvas" );
		settings ->setValue( "software_cursor", softwareCursor );
	settings ->endGroup( );
}