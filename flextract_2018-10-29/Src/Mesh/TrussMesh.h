#ifndef __TRUSS_MESH_H__
#define __TRUSS_MESH_H__


#include <QtGui/QtGui>

#include "Mesh.h"

#include "../Base/List.h"
#include "../Base/Vector.h"
#include "../Base/Bounds.h"



enum fTrussMeshConnectivity_e {
	TMC_UNKNOWN							= -1,
	TMC_ALL_NODES						= 0,
	TMC_NEIGHBOR_NODES					= 1,
};


enum fTrussMeshEditOperation_e {
	TME_SELECTION							= -1,

	TME_PAINT_ELEM_REMOVE					= 0,
	TME_PAINT_ELEM_SET_MATERIAL				= 1,
	TME_PAINT_NODE_MOVE						= 2,
	TME_PAINT_NODE_REMOVE					= 3,
	TME_PAINT_NODE_ADD_REMOVE_BNDCOND		= 4,
	TME_PAINT_NODE_ADD_REMOVE_LOAD			= 5,

};


class fTrussMaterial : public QObject {
	Q_OBJECT

public:
										fTrussMaterial( );
										fTrussMaterial( const QString &name, const QColor &clr, float A, float E );
										~fTrussMaterial( );

	QString								GetName( ) const					{ return objectName( ); };
	QColor								GetColor( )	const					{ return color; };
	float								GetArea( ) const					{ return A; };
	float								GetYoungsModulus( ) const			{ return E; };

	void								SetName( const QString &name )		{ this ->setObjectName( name ); };
	void								SetColor( const QColor &clr )		{ this ->color = clr; };
	void								SetArea( float A )					{ this ->A = A; };
	void								SetYoungsModulus( float E )			{ this ->E = E; };

private:
	float								A;
	float								E;

	QColor								color;
};



class fTrussMaterialList : public QAbstractListModel {
	Q_OBJECT

public:
										fTrussMaterialList( QObject *parent = 0 );
										~fTrussMaterialList( );

	QVariant							data( const QModelIndex & index, int role = Qt::DisplayRole ) const;
	Qt::ItemFlags						flags( const QModelIndex & index ) const;
	int									rowCount( const QModelIndex & parent = QModelIndex() ) const;

	void								AddMaterial( fTrussMaterial *mat );
	void								RemoveMaterial( int id );

	fTrussMaterial *					GetMaterial( int id ) const { return materials.at( id ); };
	int									GetNum( ) const				{ return materials.size( ); };

private:
	QList<fTrussMaterial *>				materials;

};




class fTrussMesh : public fMesh {
public:
										fTrussMesh( float x, float y, int nx, int ny, fTrussMeshConnectivity_e connectivity, int con_size );
	virtual								~fTrussMesh( );

	virtual fMeshType_e					GetType( ) const											{ return MT_TRUSS; };
	virtual void						Draw( fCamera2 *camera );

	virtual fBounds2					GetBounds( );

	fTrussMeshEditOperation_e			GetCurrentEditOperation( ) const							{ return editOperation; };
	void								SetCurrentEditOperation( fTrussMeshEditOperation_e edit )	{ editOperation = edit; };

	fTrussMaterialList *				GetMaterialList( )											{ return materialList; };
	fNodeDisplacementList *				GetNodeDisplacementList( )									{ return nodeDisplacementList; };
	fNodeLoadList *						GetNodeLoadList( )											{ return nodeLoadList; };

	virtual QString						GetFileFilters( );
	virtual void						Save( QString &file_name, QString &filter_name );

	void								OnMaterialRemoved( int id );
	void								OnNodeDisplacementRemoved( int id );
	void								OnNodeLoadRemoved( int id );

	void								SetBrushNodeDisplacementId( int id );
	void								SetBrushNodeLoadId( int id );
	void								SetBrushMaterialId( int id );

	void								ApplyMaterialForElementsInsideBrush			( fCamera2 *camera );
	void								StartMoveNodesInsideBrush					( fCamera2 *camera );
	void								MoveNodesInsideBrush						( fCamera2 *camera );
	void								StopMoveNodesInsideBrush					( fCamera2 *camera );
	void								RemoveNodesInsideBrush						( fCamera2 *camera );
	void								RemoveElementsInsideBrush					( fCamera2 *camera );

	void								SetNodeDisplacementForNodesInsideBrush		( fCamera2 *camera );
	void								RemoveNodeDisplacementForNodesInsideBrush	( fCamera2 *camera );
	void								SetNodeLoadForNodesInsideBrush				( fCamera2 *camera );
	void								RemoveNodeLoadForNodesInsideBrush			( fCamera2 *camera );

	void								DisableElement				( int e );
	void								RemoveElementsWithMaterialId( int id );

	void								OnMousePress	( QMouseEvent *event, fCamera2 *camera );
	void								OnMouseRelease	( QMouseEvent *event, fCamera2 *camera );
	void								OnMouseMove		( QMouseEvent *event, fCamera2 *camera );


	QColor								GetDefaultMat1Color( )						{ return defaultMat1Color; };
	QColor								GetDefaultMat2Color( )						{ return defaultMat2Color; };
	QColor								GetDefaultLoadColor( )						{ return defaultLoadColor; };
	QColor								GetDefaultDispColor( )						{ return defaultDispColor; };

private:
	void								ReadSettings( );
	void								WriteSettings( );

	fList<fVec2>						nodePosition;
	fList<fSelectStatus_e>				nodeSelectStatus;
	fList<fList<int> >					nodeElemIds;		// List of all surrounding elements
	fList<int>							nodeElemNum;		// List of surrounding elements
	fList<int>							nodeDispId;
	fList<int>							nodeLoadId;
	fList<bool>							nodeEnabled;

	// Temporary lists for moving nodes
	fList<int>							moveNodeIds;		// Ids of the grabbed nodes (they are going to move)
	fList<float>						moveNodeFrac;
	fVec2								prevBrushPos;

	fList<int>							elemNodeId1;
	fList<int>							elemNodeId2;
	fList<int>							elemMaterialId;
	fList<bool>							elemEnabled;

	fBounds2							bounds;

	fTrussMaterialList *				materialList;
	int									brushMaterialId;

	fNodeDisplacementList *				nodeDisplacementList;
	int									brushNodeDisplacementId;

	fNodeLoadList *						nodeLoadList;
	int									brushNodeLoadId;

	fTrussMeshEditOperation_e			editOperation;

	fVec2								mblPressPos;
	fVec2								mbrPressPos;
	bool								mblPressed;
	bool								mbrPressed;

	int									elemLineWidth;
	int									nodeDispIconSize;		// Icon size for the nodes with given displacement
	int									nodeDispLineWidth;
	int									nodeLoadIconSize;		// Icon size for the nodes with given load

	int									nodeSize;
	QColor								nodeColor;
	bool								drawNodes;

	QColor								defaultMat1Color;
	QColor								defaultMat2Color;
	QColor								defaultLoadColor;
	QColor								defaultDispColor;
};


#endif//__TRUSS_MESH_H__