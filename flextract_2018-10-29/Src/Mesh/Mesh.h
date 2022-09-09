#ifndef __MESH_H__
#define __MESH_H__


#include <QtGui/QtGui>



enum fMeshType_e {
	MT_UNKNOWN							= -1,
	MT_TRUSS							= 0,
	MT_CONTINUUM						= 1,
};



enum fSelectStatus_e {
	SS_FREE								= 0,
	SS_HOVERED							= 1,
	SS_GRABBED							= 2,
};






enum fNodeDisplacementType_e {
	ND_X								= 0,
	ND_Y,
	ND_ALL,
};


class fNodeDisplacement : QObject {
public:
										fNodeDisplacement( );
										fNodeDisplacement( const QString &name, const QColor &clr, fNodeDisplacementType_e type, float dx, float dy );
										~fNodeDisplacement( );

	QString								GetName( ) const						{ return objectName( ); };
	QColor								GetColor( )	const						{ return color; };
	float								GetDx( ) const							{ return dx; };
	float								GetDy( ) const							{ return dy; };
	fNodeDisplacementType_e				GetType( ) const						{ return type; };

	void								SetName( const QString &name )			{ this ->setObjectName( name ); };
	void								SetColor( const QColor &clr )			{ this ->color = clr; };
	void								SetType( fNodeDisplacementType_e t )	{ this ->type = t; };
	void								SetDx( float dx )						{ this ->dx = dx; };
	void								SetDy( float dy )						{ this ->dy = dy; };

private:
	float								dx;
	float								dy;

	QColor								color;
	fNodeDisplacementType_e				type;
};



class fNodeDisplacementList : public QAbstractListModel {
public:
										fNodeDisplacementList( QObject *parent = 0 );
										~fNodeDisplacementList( );

	QVariant							data( const QModelIndex & index, int role = Qt::DisplayRole ) const;
	Qt::ItemFlags						flags( const QModelIndex & index ) const;
	int									rowCount( const QModelIndex & parent = QModelIndex( ) ) const;

	void								AddNodeDisplacement( fNodeDisplacement *displ );
	void								RemoveNodeDisplacement( int id );

	fNodeDisplacement *					GetNodeDisplacement( int id ) const				{ return displacements.at( id ); };
	int									GetNum( ) const									{ return displacements.size( ); };

private:
	QList<fNodeDisplacement *>			displacements;

};






class fNodeLoad : QObject {
public:
										fNodeLoad( );
										fNodeLoad( const QString &name, const QColor &clr, float fx, float fy );
										~fNodeLoad( );

	QString								GetName( ) const						{ return objectName( ); };
	QColor								GetColor( )	const						{ return color; };
	float								GetFx( ) const							{ return fx; };
	float								GetFy( ) const							{ return fy; };

	void								SetName( const QString &name )			{ this ->setObjectName( name ); };
	void								SetColor( const QColor &clr )			{ this ->color = clr; };
	void								SetFx( float fx )						{ this ->fx = fx; };
	void								SetFy( float fy )						{ this ->fy = fy; };

private:
	float								fx;
	float								fy;

	QColor								color;
};



class fNodeLoadList : public QAbstractListModel {
public:
										fNodeLoadList( QObject *parent = 0 );
										~fNodeLoadList( );

	QVariant							data( const QModelIndex & index, int role = Qt::DisplayRole ) const;
	Qt::ItemFlags						flags( const QModelIndex & index ) const;
	int									rowCount( const QModelIndex & parent = QModelIndex( ) ) const;

	void								AddNodeLoad( fNodeLoad *load );
	void								RemoveNodeLoad( int id );

	fNodeLoad *							GetNodeLoad( int id ) const						{ return loads.at( id ); };
	int									GetNum( ) const									{ return loads.size( ); };

private:
	QList<fNodeLoad *>					loads;

};





class fBounds2;
class fCamera2;
class QObject;
class QColor;


class fMesh {
public:
										fMesh( );
	virtual								~fMesh( );

	virtual void						Draw( fCamera2 *camera )	= 0;
	virtual fMeshType_e					GetType( ) const			= 0;

	virtual fBounds2					GetBounds( )				= 0;

	virtual QString						GetFileFilters( )										= 0;
	virtual void						Save( QString &file_name, QString &filter_name )		= 0;

	virtual void						OnMousePress( QMouseEvent *event, fCamera2 *camera )	= 0;
	virtual void						OnMouseRelease( QMouseEvent *event, fCamera2 *camera )	= 0;
	virtual void						OnMouseMove( QMouseEvent *event, fCamera2 *camera )		= 0;

protected:
	
};


#endif//__MESH_H__
