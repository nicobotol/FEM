#ifndef FCANVAS_H
#define FCANVAS_H


#include <QtGui/QtGui>
#include <QtOpenGL/QGLWidget> // CLFE change

#include "Camera.h"


class fBounds;
class QTimer;
class QTime;


class fCanvas : public QGLWidget {
	Q_OBJECT

public:
									fCanvas( );
									~fCanvas( );

	void							SetupView( fBounds2 &bounds );

	void							Update( )								{ updateGL( ); };
	void							DrawCircle( fVec2 &pos, float r );

	void							UpdateCursorShape( );

protected:
	// Derived from the QGLWidget class
	void							initializeGL( );
	void							paintGL( );
	void							resizeGL( int width, int height );

	void							keyPressEvent( QKeyEvent *event );
	void							keyReleaseEvent( QKeyEvent *event );

	void							mousePressEvent( QMouseEvent *event );
	void							mouseReleaseEvent( QMouseEvent *event );
    void							mouseMoveEvent( QMouseEvent *event );
	void							wheelEvent( QWheelEvent *event );
	void							leaveEvent( QEvent * event );
	void							enterEvent( QEvent * event );

private slots:
	void							OnBrushSizeChange( int );
	void							OnMouseZoomTimer( );

private:
	void							ReadSettings( );
	void							WriteSettings( );

	fCamera2						camera;

	bool							spacePressed;
	bool							leftMouseBtnPressed;
	bool							rightMouseBtnPressed;

	QPoint							mousePos;

	// Related to smooth zooming
	QTimer							mouseZoomTimer;

	QTime							mouseZoomTime;
	int								mouseZoomTimeFrom;
	int								mouseZoomTimeCurr;
	int								mouseZoomTimeTo;

	float							mouseZoomFrom;
	float							mouseZoomTo;
	float							mouseZoomSpace;
	float							mouseZoomUpdatePeriod;
	float							mouseZoomUpdateFreq;

	// Brush size
	int								brushSize;

	QCursor							cursorAim;
	QCursor							cursorBrush;
	QPixmap							pixmapAim;
	QPixmap							pixmapBrush;

	// Colors
	QColor							backgroundColor;
	QColor							cursorColor;
	bool							softwareCursor;
	bool							mouseInside;
	bool							drawBrush;
};

#endif // FCANVAS_H
