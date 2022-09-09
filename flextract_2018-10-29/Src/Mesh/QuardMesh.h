#ifndef __QUARD_MESH_H__
#define __QUARD_MESH_H__


#include <QtGui/QtGui>

#include "Mesh.h"

#include "../Base/List.h"
#include "../Base/Vector.h"
#include "../Base/Bounds.h"



enum fQuardMeshEditOperation_e {
	QME_SELECTION							= -1,

	QME_PAINT_ELEM_REMOVE					= 0,
	QME_PAINT_ELEM_SET_MATERIAL				= 1,

	QME_PAINT_NODE_MOVE						= 2,
	QME_PAINT_NODE_ADD_REMOVE_BNDCOND		= 3,
	QME_PAINT_NODE_ADD_REMOVE_LOAD			= 4,

	QME_PAINT_FACE_ADD_REMOVE_LOAD			= 5,
};


class fSurfLoad : QObject {
	Q_OBJECT

public:
										fSurfLoad( );
										fSurfLoad( const QString &name, const QColor &clr, float val );
										~fSurfLoad( );

	QString								GetName( ) const						{ return objectName( ); };
	QColor								GetColor( )	const						{ return color; };
	float								GetVal( ) const							{ return val; };

	void								SetName( const QString &name )			{ this ->setObjectName( name ); };
	void								SetColor( const QColor &clr )			{ this ->color = clr; };
	void								SetVal( float val )						{ this ->val = val; };

private:
	float								val;

	QColor								color;
};


class fSurfLoadList : public QAbstractListModel {
	Q_OBJECT

public:
										fSurfLoadList( QObject *parent = 0 );
										~fSurfLoadList( );

	QVariant							data( const QModelIndex & index, int role = Qt::DisplayRole ) const;
	Qt::ItemFlags						flags( const QModelIndex & index ) const;
	int									rowCount( const QModelIndex & parent = QModelIndex( ) ) const;

	void								AddSurfLoad( fSurfLoad *load );
	void								RemoveSurfLoad( int id );

	fSurfLoad *							GetSurfLoad( int id ) const						{ return loads.at( id ); };
	int									GetNum( ) const									{ return loads.size( ); };

private:
	QList<fSurfLoad *>					loads;

};





class fQuardMaterial : public QObject {
	Q_OBJECT

public:
										fQuardMaterial( );
										fQuardMaterial( const QString &name, const QColor &clr, float A, float Ex, float poisson, float dens,
														bool isotropic = true, float Ey = 0.0f, float Gxy = 0.0f );
										~fQuardMaterial( );

	QString								GetName( ) const					{ return objectName( ); };
	QColor								GetColor( )	const					{ return color; };
	float								GetThickness( ) const				{ return A; };
	float								GetEx( ) const						{ return Ex; };
	float								GetEy( ) const						{ return Ey; };
	float								GetGxy( ) const						{ return Gxy; };
	float								GetPoisson( ) const					{ return poisson; };
	float								GetDens( ) const					{ return dens; };
	bool								GetIsotropic( ) const				{ return isotropic; };

	void								SetName( const QString &name )		{ this ->setObjectName( name ); };
	void								SetColor( const QColor &clr )		{ this ->color = clr; };
	void								SetThickness( float A )				{ this ->A = A; };
	void								SetEx( float Ex )					{ this ->Ex = Ex; };
	void								SetEy( float Ey )					{ this ->Ey = Ey; };
	void								SetGxy( float Gxy )					{ this ->Gxy = Gxy; };
	void								SetPoisson( float poisson )			{ this ->poisson = poisson; };
	void								SetDens( float dens )				{ this ->dens = dens; };
	void								SetIsotropic( bool iso )			{ this ->isotropic = iso; };

private:
	float								A;
	float								Ex;
	float								Ey;
	float								Gxy;
	float								poisson;
	float								dens;

	bool								isotropic;

	QColor								color;
};



class fQuardMaterialList : public QAbstractListModel {
	Q_OBJECT

public:
										fQuardMaterialList( QObject *parent = 0 );
										~fQuardMaterialList( );

	QVariant							data( const QModelIndex & index, int role = Qt::DisplayRole ) const;
	Qt::ItemFlags						flags( const QModelIndex & index ) const;
	int									rowCount( const QModelIndex & parent = QModelIndex() ) const;

	void								AddMaterial( fQuardMaterial *mat );
	void								RemoveMaterial( int id );

	fQuardMaterial *					GetMaterial( int id ) const { return materials.at( id ); };
	int									GetNum( ) const				{ return materials.size( ); };

private:
	QList<fQuardMaterial *>				materials;

};




class fQuardMesh : public fMesh {
public:
										fQuardMesh( float x, float y, int nx, int ny );
	virtual								~fQuardMesh( );

	virtual fMeshType_e					GetType( ) const											{ return MT_CONTINUUM; };
	virtual void						Draw( fCamera2 *camera );

	virtual fBounds2					GetBounds( );

	fQuardMeshEditOperation_e			GetCurrentEditOperation( ) const							{ return editOperation; };
	void								SetCurrentEditOperation( fQuardMeshEditOperation_e edit )	{ editOperation = edit; };

	fQuardMaterialList *				GetMaterialList( )											{ return materialList; };
	fNodeDisplacementList *				GetNodeDisplacementList( )									{ return nodeDisplacementList; };
	fNodeLoadList *						GetNodeLoadList( )											{ return nodeLoadList; };
	fSurfLoadList *						GetSurfLoadList( )											{ return surfLoadList; };

	virtual QString						GetFileFilters( );
	virtual void						Save( QString &file_name, QString &filter_name );

	void								OnMaterialRemoved( int id );
	void								OnNodeDisplacementRemoved( int id );
	void								OnNodeLoadRemoved( int id );
	void								OnSurfLoadRemoved( int id );

	void								SetBrushNodeDisplacementId( int id );
	void								SetBrushNodeLoadId( int id );
	void								SetBrushMaterialId( int id );
	void								SetBrushSurfLoadId( int id );

	void								ApplyMaterialForElementsInsideBrush			( fCamera2 *camera );
	void								StartMoveNodesInsideBrush					( fCamera2 *camera );
	void								MoveNodesInsideBrush						( fCamera2 *camera );
	void								StopMoveNodesInsideBrush					( fCamera2 *camera );
	void								RemoveElementsInsideBrush					( fCamera2 *camera );

	void								SetNodeDisplacementForNodesInsideBrush		( fCamera2 *camera );
	void								RemoveNodeDisplacementForNodesInsideBrush	( fCamera2 *camera );
	void								SetNodeLoadForNodesInsideBrush				( fCamera2 *camera );
	void								RemoveNodeLoadForNodesInsideBrush			( fCamera2 *camera );

	void								SetSurfLoadForFacesInsideBrush				( fCamera2 *camera );
	void								RemoveSurfLoadForFacesInsideBrush			( fCamera2 *camera );

	void								DisableElement				( int e );
	void								RemoveElementsWithMaterialId( int id );

	void								OnMousePress	( QMouseEvent *event, fCamera2 *camera );
	void								OnMouseRelease	( QMouseEvent *event, fCamera2 *camera );
	void								OnMouseMove		( QMouseEvent *event, fCamera2 *camera );


	QColor								GetDefaultMat1Color( )						{ return defaultMat1Color; };
	QColor								GetDefaultMat2Color( )						{ return defaultMat2Color; };
	QColor								GetDefaultLoadColor( )						{ return defaultLoadColor; };
	QColor								GetDefaultSurfLoadColor( )					{ return defaultSurfLoadColor; };
	QColor								GetDefaultDispColor( )						{ return defaultDispColor; };

private:
	void								ReadSettings( );
	void								WriteSettings( );

	fList<fVec2>						nodePosition;
	fList<fSelectStatus_e>				nodeSelectStatus;
	fList<fList<int> >					nodeElemIds;		// List of all surrounding elements
	fList<int>							nodeElemNum;
	fList<int>							nodeDispId;
	fList<int>							nodeLoadId;
	fList<bool>							nodeEnabled;

	// Temporary lists for moving nodes
	fList<int>							moveNodeIds;		// Ids of the grabbed nodes (they are going to move)
	fList<float>						moveNodeFrac;
	fVec2								prevBrushPos;

	fList<int>							elemNodeId[4];
	fList<int>							elemFaceId[4];
	fList<int>							elemMaterialId;
	fList<bool>							elemEnabled;

	fList<int>							faceNodeId[2];
	fList<fList<int> >					faceElemIds;		// List of all surrounding elements (2)
	fList<fList<int> >					faceElemPosIds;		// Relative position of the face relative to element
	fList<int>							faceElemNum;
	fList<int>							faceLoadId;
	fList<bool>							faceEnabled;

	fBounds2							bounds;

	fQuardMaterialList *				materialList;
	int									brushMaterialId;

	fNodeDisplacementList *				nodeDisplacementList;
	int									brushNodeDisplacementId;

	fSurfLoadList *						surfLoadList;
	int									brushSurfLoadId;

	fNodeLoadList *						nodeLoadList;
	int									brushNodeLoadId;

	fQuardMeshEditOperation_e			editOperation;

	fVec2								mblPressPos;
	fVec2								mbrPressPos;
	bool								mblPressed;
	bool								mbrPressed;

	int									nodeDispIconSize;		// Icon size for the nodes with given displacement
	int									nodeDispLineWidth;
	int									nodeLoadIconSize;		// Icon size for the nodes with given load

	int									nodeSize;
	QColor								nodeColor;
	bool								drawNodes;

	int									faceLineWidth;
	QColor								faceColor;
	bool								drawFaces;

	QColor								defaultMat1Color;
	QColor								defaultMat2Color;
	QColor								defaultLoadColor;
	QColor								defaultSurfLoadColor;
	QColor								defaultDispColor;
};


#endif//__QUARD_MESH_H__