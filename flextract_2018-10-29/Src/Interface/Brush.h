#ifndef __BRUSH_H__
#define __BRUSH_H__


#include <QtGui/QtGui>


class fBrush : public QObject {
	Q_OBJECT

public:
									fBrush( );
									~fBrush( );

	void							SetPosition( const QPoint &pos )			{ this ->pos = pos; }; // CLFE change
	void							SetColor( const QColor &color )			{ this ->color = color; }; // CLFE change

	void							SetDiameter( int diameter );
	void							SetMinDiameter( int min );
	void							SetMaxDiameter( int max );
	void							SetHardness( int hardness );
	void							SetInclusiveState( bool state );

	QPoint							GetPosition( )						{ return pos; };
	QColor							GetColor( )							{ return color; };
	int								GetDiameter( )						{ return diameter; };
	int 							GetMinDiameter( )					{ return minDiameter; };
	int 							GetMaxDiameter( )					{ return maxDiameter; };
	int								GetHardness( )						{ return hardness; };
	bool							GetInclusiveState( )				{ return inclusive; };

signals:
	void							sizeChanged( int );
	void							hardnessChanged( int );
	void							inclusiveStateChanged( bool );

public slots:
	void							OnBrushSizeChange( int );
	void							OnBrushHardnessChange( int );
	void							OnInclusiveStateChange( bool );

private:
	void							ReadSettings( );
	void							WriteSettings( );

	QColor							color;
	QPoint							pos;
	int								diameter;
	int								minDiameter;
	int								maxDiameter;
	int								hardness;		// 0 -- 100
	bool							inclusive;
};


#endif//__BRUSH_H__