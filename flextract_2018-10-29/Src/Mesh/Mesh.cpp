#include "../precompiled.h"



// Node displacements
fNodeDisplacement::fNodeDisplacement( ) {
	this ->dx			= 0.0f;
	this ->dy			= 0.0f;
	this ->type			= ND_ALL;
	this ->color		= Qt::white;
}


fNodeDisplacement::fNodeDisplacement( const QString &name, const QColor &clr, fNodeDisplacementType_e type, float dx, float dy ) {
	this ->setObjectName( name );
	this ->dx			= dx;
	this ->dy			= dy;
	this ->type			= type;
	this ->color		= clr;
}


fNodeDisplacement::~fNodeDisplacement( ) {
	
}


// Node loads
fNodeLoad::fNodeLoad( ) {
	this ->fx			= 0.0f;
	this ->fy			= 0.0f;
	this ->color		= Qt::white;
}


fNodeLoad::fNodeLoad( const QString &name, const QColor &clr, float fx, float fy ) {
	this ->setObjectName( name );
	this ->fx			= fx;
	this ->fy			= fy;
	this ->color		= clr;
}


fNodeLoad::~fNodeLoad( ) {
	
}



// Nodal diplacements list
fNodeDisplacementList::fNodeDisplacementList( QObject *parent ) : QAbstractListModel( parent ) {
	
}


fNodeDisplacementList::~fNodeDisplacementList(  ) {
	qDeleteAll( displacements );
}


QVariant fNodeDisplacementList::data( const QModelIndex &index, int role ) const {
	if ( ( role != Qt::DisplayRole ) && ( role != Qt::DecorationRole ) ) {
		return QVariant( );
	}
	
	// Find out whether it is a filter or file
	int id = index.row( );
	if ( id < 0 ) {
		return QVariant( );
	}

	// Color (decoration)
	if ( role == Qt::DecorationRole ) {
		return QVariant( displacements[id] ->GetColor( ) );
	}
	// Name (item's text)
	return QVariant( displacements[id] ->GetName( ) );
}


Qt::ItemFlags fNodeDisplacementList::flags( const QModelIndex & index ) const {
	Qt::ItemFlags flags = Qt::NoItemFlags;

	if ( index != QModelIndex( ) ) {
		flags |= Qt::ItemIsEnabled;
		flags |= Qt::ItemIsSelectable;
	}

	return flags;
}

int fNodeDisplacementList::rowCount( const QModelIndex & parent ) const {
	return displacements.size( );
}




void fNodeDisplacementList::AddNodeDisplacement( fNodeDisplacement *displ ) {
	// Let views know what we are doing
	beginInsertRows( QModelIndex( ), displacements.size( ), displacements.size( ) );
	displacements.append( displ );
	endInsertRows( );
}


void fNodeDisplacementList::RemoveNodeDisplacement( int id ) {
	// Make sure we are deleting the right element (which can be deleted)
	if ( ( id < 0 ) || ( displacements.size( ) == 0 ) || ( displacements.size( ) <= id ) ) {
		return;
	}
	
	if ( displacements.size( ) == id + 1 ) {
		beginRemoveRows( QModelIndex( ), id, id );
	} else {
		beginRemoveRows( QModelIndex( ), id + 1, id + 1 );
	}
	displacements.removeAt( id );
	endRemoveRows( );
}



// Nodal loads list
fNodeLoadList::fNodeLoadList( QObject *parent ) : QAbstractListModel( parent ) {
	
}


fNodeLoadList::~fNodeLoadList(  ) {
	qDeleteAll( loads );
}


QVariant fNodeLoadList::data( const QModelIndex &index, int role ) const {
	if ( ( role != Qt::DisplayRole ) && ( role != Qt::DecorationRole ) ) {
		return QVariant( );
	}
	
	// Find out whether it is a filter or file
	int id = index.row( );
	if ( id < 0 ) {
		return QVariant( );
	}

	// Color (decoration)
	if ( role == Qt::DecorationRole ) {
		return QVariant( loads[id] ->GetColor( ) );
	}
	// Name (item's text)
	return QVariant( loads[id] ->GetName( ) );
}


Qt::ItemFlags fNodeLoadList::flags( const QModelIndex & index ) const {
	Qt::ItemFlags flags = Qt::NoItemFlags;

	if ( index != QModelIndex( ) ) {
		flags |= Qt::ItemIsEnabled;
		flags |= Qt::ItemIsSelectable;
	}

	return flags;
}

int fNodeLoadList::rowCount( const QModelIndex & parent ) const {
	return loads.size( );
}




void fNodeLoadList::AddNodeLoad( fNodeLoad *load ) {
	// Let views know what we are doing
	beginInsertRows( QModelIndex( ), loads.size( ), loads.size( ) );
	loads.append( load );
	endInsertRows( );
}


void fNodeLoadList::RemoveNodeLoad( int id ) {
	// Make sure we are deleting the right element (which can be deleted)
	if ( ( id < 0 ) || ( loads.size( ) == 0 ) || ( loads.size( ) <= id ) ) {
		return;
	}
	
	if ( loads.size( ) == id + 1 ) {
		beginRemoveRows( QModelIndex( ), id, id );
	} else {
		beginRemoveRows( QModelIndex( ), id + 1, id + 1 );
	}
	loads.removeAt( id );
	endRemoveRows( );
}



fMesh::fMesh( ) {
	
}

fMesh::~fMesh( ) {
	
}


