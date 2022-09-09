#include "../precompiled.h"



// Element materials
fQuardMaterial::fQuardMaterial( ) : QObject( ) {
	this ->A			= 1.0f;
	this ->Ex			= Ex;
	this ->Ey			= 0.0f;
	this ->Gxy			= 0.0f;
	this ->dens			= 1000.0f;
	this ->poisson		= 0.3f;
	this ->color		= Qt::blue;

	this ->isotropic	= true;
}


fQuardMaterial::fQuardMaterial( const QString &name, const QColor &clr, float A, float Ex, float poisson, float dens, bool iso, float Ey, float Gxy ) : QObject( ) {
	this ->setObjectName( name );
	this ->color		= clr;
	this ->A			= A;
	this ->Ex			= Ex;
	this ->Ey			= Ey;
	this ->Gxy			= Gxy;
	this ->dens			= dens;
	this ->poisson		= poisson;

	this ->isotropic	= iso;

	if ( isotropic ) {
		this ->Ey		= 0.0f;
		this ->Gxy		= 0.0f;
	}
}


fQuardMaterial::~fQuardMaterial( ) {
	
}




// Quard material list
fQuardMaterialList::fQuardMaterialList( QObject *parent ) : QAbstractListModel( parent ) {
	
}


fQuardMaterialList::~fQuardMaterialList(  ) {
	qDeleteAll( materials );
}


QVariant fQuardMaterialList::data( const QModelIndex &index, int role ) const {
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
		return QVariant( materials[id] ->GetColor( ) );
	}
	// Name (item's text)
	return QVariant( materials[id] ->GetName( ) );
}


Qt::ItemFlags fQuardMaterialList::flags( const QModelIndex & index ) const {
	Qt::ItemFlags flags = Qt::NoItemFlags;

	if ( index != QModelIndex( ) ) {
		flags |= Qt::ItemIsEnabled;
		flags |= Qt::ItemIsSelectable;
	}

	return flags;
}


int fQuardMaterialList::rowCount( const QModelIndex & parent ) const {
	return materials.size( );
}


void fQuardMaterialList::AddMaterial( fQuardMaterial *mat ) {
	// Let views know what we are doing
	beginInsertRows( QModelIndex( ), materials.size( ), materials.size( ) );
	materials.append( mat );
	endInsertRows( );
}


void fQuardMaterialList::RemoveMaterial( int id ) {
	// Make sure we are deleting the right element (which can be deleted)
	if ( ( id < 0 ) || ( materials.size( ) == 0 ) || ( materials.size( ) <= id ) ) {
		return;
	}
	
	//beginRemoveRows( QModelIndex( ), id, id );
	if ( materials.size( ) == id + 1 ) {
		beginRemoveRows( QModelIndex( ), id, id );
	} else {
		beginRemoveRows( QModelIndex( ), id + 1, id + 1 );
	}
	//beginRemoveRows( QModelIndex( ), id, id );
	materials.removeAt( id );
	endRemoveRows( );
}


















// Node loads
fSurfLoad::fSurfLoad( ) : QObject( ) {
	this ->val			= 0.0f;
	this ->color		= Qt::white;
}


fSurfLoad::fSurfLoad( const QString &name, const QColor &clr, float val ) : QObject( ) {
	QObject::setObjectName( name );
	this ->val			= val;
	this ->color		= clr;
}


fSurfLoad::~fSurfLoad( ) {
	
}


// Nodal loads list
fSurfLoadList::fSurfLoadList( QObject *parent ) : QAbstractListModel( parent ) {
	
}


fSurfLoadList::~fSurfLoadList(  ) {
	qDeleteAll( loads );
}


QVariant fSurfLoadList::data( const QModelIndex &index, int role ) const {
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


Qt::ItemFlags fSurfLoadList::flags( const QModelIndex & index ) const {
	Qt::ItemFlags flags = Qt::NoItemFlags;

	if ( index != QModelIndex( ) ) {
		flags |= Qt::ItemIsEnabled;
		flags |= Qt::ItemIsSelectable;
	}

	return flags;
}

int fSurfLoadList::rowCount( const QModelIndex & parent ) const {
	return loads.size( );
}




void fSurfLoadList::AddSurfLoad( fSurfLoad *load ) {
	loads.size( );
	// Let views know what we are doing
	beginInsertRows( QModelIndex( ), loads.size( ), loads.size( ) );
	loads.append( load );
	endInsertRows( );
}


void fSurfLoadList::RemoveSurfLoad( int id ) {
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











fQuardMesh::fQuardMesh( float width, float height, int ex, int ey ) {
	editOperation			= QME_SELECTION;

	materialList			= NULL;
	brushMaterialId			= 0;
	nodeDisplacementList	= NULL;
	brushNodeDisplacementId = 0;
	nodeLoadList			= NULL;
	brushNodeLoadId			= 0;
	surfLoadList			= NULL;
	brushSurfLoadId			= 0;

	ReadSettings( );


	// Material list and co
	materialList			= new fQuardMaterialList( );
	nodeDisplacementList	= new fNodeDisplacementList( );
	nodeLoadList			= new fNodeLoadList( );
	surfLoadList			= new fSurfLoadList( );

	
	// Resize node arrays
	int nx			= ex + 1;
	int ny			= ey + 1;
	int node_num	= nx * ny;
	int elem_num	= ex * ey;
	int face_num	= node_num * 2 - nx - ny;

	nodePosition.SetNum( node_num );
	nodeSelectStatus.SetNum( node_num );
	nodeElemIds.SetNum( node_num );
	nodeElemNum.SetNum( node_num );
	nodeEnabled.SetNum( node_num );
	nodeEnabled.SetVal( true );

	faceElemIds.SetNum( face_num );
	faceElemPosIds.SetNum( face_num );
	faceElemNum.SetNum( face_num );
	faceEnabled.SetNum( face_num );
	faceEnabled.SetVal( true );

	faceLoadId.SetNum( face_num );
	faceLoadId.SetVal( -1 );

	// Nodal displacement and load ids
	nodeDispId.SetNum( node_num );
	nodeLoadId.SetNum( node_num );
	nodeDispId.SetVal( -1 );
	nodeLoadId.SetVal( -1 );

	// Shortcuts
	float dx = width  / float(nx - 1);
	float dy = height / float(ny - 1);

	// Initialize node positions
	for ( int i = 0; i < nx; i++ ) {
		for ( int j = 0; j < ny; j++ ) {
			int ind = i*ny + j;
			nodePosition[ind].x = float(i) * dx;
			nodePosition[ind].y = float(j) * dy;
		}
	}

	// Update the bounds
	bounds.FromPoints( nodePosition.Ptr( ), nodePosition.Num( ) );

	// Initialize elements
	{
		elemNodeId[0].SetNum( elem_num );
		elemNodeId[1].SetNum( elem_num );
		elemNodeId[2].SetNum( elem_num );
		elemNodeId[3].SetNum( elem_num );
		// For all nodes connect with the nodes, having higer id
		for ( int i = 0; i < ex; i++ ) {
			for ( int j = 0; j < ey; j++ ) {
				// Current element
				int eid = i*ey + j;
				// Neighboring nodes
				int id0 = (i + 0)*ny + (j + 0);
				int id1 = (i + 1)*ny + (j + 0);
				int id2 = (i + 1)*ny + (j + 1);
				int id3 = (i + 0)*ny + (j + 1);

				elemNodeId[0][eid] = id0;
				elemNodeId[1][eid] = id1;
				elemNodeId[2][eid] = id2;
				elemNodeId[3][eid] = id3;
			}
		}
	}
	

	
	// Initialize faces and connectivities
	{
		int face_num = 0;
		faceNodeId[0].SetGranularity( node_num * 2 );
		faceNodeId[1].SetGranularity( node_num * 2 );

		elemFaceId[0].SetNum( elem_num );
		elemFaceId[1].SetNum( elem_num );
		elemFaceId[2].SetNum( elem_num );
		elemFaceId[3].SetNum( elem_num );

		// For all nodes connect with the nodes, having higer id
		for ( int i = 0; i < nx; i++ ) {
			for ( int j = 0; j < ny; j++ ) {
				// Current node
				int id0 = i*ny + j;
				// Neighboring nodes
				int id1 = (i + 0)*ny + (j + 1);
				int id2 = (i + 1)*ny + (j + 0);

				// Neighboring elements
				int eid1 = (i - 1)*ey + (j - 1);
				int eid2 = (i + 0)*ey + (j - 1);
				int eid3 = (i + 0)*ey + (j + 0);
				int eid4 = (i - 1)*ey + (j + 0);


				// Check for validity of the surrounding nodes
				bool id1_valid = true;
				bool id2_valid = true;
				// Top
				if ( (j + 1) >= ny ) {
					id1_valid = false;
				}
				// Right
				if ( (i + 1) >= nx ) {
					id2_valid = false;
				}

				// Check for validity of the surrounding elements (relative to faces)
				bool eid1_valid_l = true;
				bool eid1_valid_r = true;
				bool eid2_valid_b = true;
				bool eid2_valid_t = true;
				// Right
				if ( i >= nx - 1 ) {
					eid1_valid_r = false;
				}
				// Left
				if ( i <= 0 ) {
					eid1_valid_l = false;
				}
				// Top
				if ( j >= ny - 1 ) {
					eid2_valid_t = false;
				}
				// Bottom
				if ( j <= 0 ) {
					eid2_valid_b = false;
				}

				// Surrounding elements
				bool eid_valid1 = true;
				bool eid_valid2 = true;
				bool eid_valid3 = true;
				bool eid_valid4 = true;
				// Left
				if ( i <= 0 ) {
					eid_valid1	= false;
					eid_valid4	= false;
				}
				// Right
				if ( i >= nx - 1 ) {
					eid_valid2	= false;
					eid_valid3	= false;
				}
				// Bottom
				if ( j <= 0 ) {
					eid_valid1	= false;
					eid_valid2	= false;
				}
				// Top
				if ( j >= ny - 1 ) {
					eid_valid3	= false;
					eid_valid4	= false;
				}
				
				
				// Append nodes
				if ( id1_valid ) {
					faceNodeId[0].Append( id0 );
					faceNodeId[1].Append( id1 );
					if ( eid1_valid_r ) {
						faceElemIds[face_num].Append( eid3 );
						faceElemPosIds[face_num].Append( 3 );
						elemFaceId[3][eid3] = face_num;
					}
					if ( eid1_valid_l ) {
						faceElemIds[face_num].Append( eid4 );
						faceElemPosIds[face_num].Append( 1 );
						elemFaceId[1][eid4] = face_num;
					}
					face_num++;
				}
				
				if ( id2_valid ) {
					faceNodeId[0].Append( id0 );
					faceNodeId[1].Append( id2 );
					if ( eid2_valid_b ) {
						faceElemIds[face_num].Append( eid2 );
						faceElemPosIds[face_num].Append( 2 );
						elemFaceId[2][eid2] = face_num;
					}
					if ( eid2_valid_t ) {
						faceElemIds[face_num].Append( eid3 );
						faceElemPosIds[face_num].Append( 0 );
						elemFaceId[0][eid3] = face_num;
					}
					face_num++;
				}
				
				// Append surrounding elements to nodes
				if ( eid_valid1 ) {
					nodeElemIds[id0].Append( eid1 );
				}
				if ( eid_valid2 ) {
					nodeElemIds[id0].Append( eid2 );
				}
				if ( eid_valid3 ) {
					nodeElemIds[id0].Append( eid3 );
				}
				if ( eid_valid4 ) {
					nodeElemIds[id0].Append( eid4 );
				}
				
			}
		}
		
	}

	for ( int i = 0; i < node_num; i++ ) {
		nodeElemNum[i] = nodeElemIds[i].Num( );
	}

	for ( int i = 0; i < face_num; i++ ) {
		faceElemNum[i] = faceElemIds[i].Num( );
	}

	
	elemMaterialId.SetNum( elem_num );
	elemMaterialId.SetVal( 0 );
	elemEnabled.SetNum( elem_num );
	elemEnabled.SetVal( true );
	
}


fQuardMesh::~fQuardMesh( ) {
	WriteSettings( );

	if ( materialList ) delete materialList;
	if ( nodeDisplacementList ) delete nodeDisplacementList;
	if ( nodeLoadList ) delete nodeLoadList;
	if ( surfLoadList ) delete surfLoadList;
}


void fQuardMesh::Draw( fCamera2 *camera ) {
	
	// Draw elements
	glBegin( GL_QUADS );
	for ( int i = 0; i < elemMaterialId.Num( ); i++ ) {
		if ( !elemEnabled[i] ) {
			continue;
		}
		QColor c = materialList ->GetMaterial( elemMaterialId[i] ) ->GetColor( );
		glColor4ub( c.red( ), c.green( ), c.blue( ), 0 );
		glVertex2fv( nodePosition[ elemNodeId[0][i] ].ToFloatPtr( ) );
		glVertex2fv( nodePosition[ elemNodeId[1][i] ].ToFloatPtr( ) );
		glVertex2fv( nodePosition[ elemNodeId[2][i] ].ToFloatPtr( ) );
		glVertex2fv( nodePosition[ elemNodeId[3][i] ].ToFloatPtr( ) );
	}
	glEnd( );

	// Draw faces
	if ( drawFaces ) {
		glLineWidth( faceLineWidth );
		QColor c = faceColor;
		glBegin( GL_LINES );
		glColor4ub( c.red( ), c.green( ), c.blue( ), 0 );
		for ( int i = 0; i < faceEnabled.Num( ); i++ ) {
			if ( !faceEnabled[i] ) {
				continue;
			}
			glVertex2fv( nodePosition[ faceNodeId[0][i] ].ToFloatPtr( ) );
			glVertex2fv( nodePosition[ faceNodeId[1][i] ].ToFloatPtr( ) );
		}
		glEnd( );
	}

	// Draw nodes
	if ( drawNodes ) {
		QColor c = nodeColor;
		glPointSize( nodeSize );
		if ( app ->IsPointColorFloatFormat( ) ) {
			glColor4f( float(c.red( ))/ 255.0f, float(c.green( ))/ 255.0f,  float(c.blue( ))/ 255.0f, 0.0f );
		} else {
			glColor4ub( c.red( ), c.green( ), c.blue( ), 0 );
		}
		glBegin( GL_POINTS );
		for ( int i = 0; i < nodePosition.Num( ); i++ ) {
			if ( !nodeEnabled[i] ) {
				continue;
			}
			glVertex2fv( nodePosition[i].ToFloatPtr( ) );
		}
		glEnd( );
	}


	// Draw node loads
	{
		QColor	c;
		float	fx, fy, dx, dy, a;
		float	size_half	= camera ->GetModelSize( nodeLoadIconSize ) * 0.5f;
		float	k			= 0.7;

		glBegin( GL_TRIANGLES );
		for ( int i = 0; i < nodePosition.Num( ); i++ ) {
			if ( ( nodeEnabled[i] == false ) || ( nodeLoadId[i] == -1 ) ) {
				continue;
			}
			c	= nodeLoadList ->GetNodeLoad( nodeLoadId[i] ) ->GetColor( );
			glColor4ub( c.red( ), c.green( ), c.blue( ), 0 );


			fx	= nodeLoadList ->GetNodeLoad( nodeLoadId[i] ) ->GetFx( );
			fy	= nodeLoadList ->GetNodeLoad( nodeLoadId[i] ) ->GetFy( );
			if ( fx == 0.0f && fy == 0.0f ) {
				continue;
			}
			a	= size_half / sqrt( fx*fx + fy*fy );
			dx	= a * fx;
			dy	= a * fy;

			float dx0	= k * dy;
			float dy0	= k * dx;

			glVertex2f( nodePosition[i].x - dx - dx0, nodePosition[i].y - dy + dy0 );
			glVertex2f( nodePosition[i].x - dx + dx0, nodePosition[i].y - dy - dy0 );
			glVertex2f( nodePosition[i].x + dx, nodePosition[i].y + dy );
		}
		glEnd( );
	}


	// Draw surface loads
	{
		QColor	c;
		float	fx, fy, dx, dy, a;
		float	size_half	= camera ->GetModelSize( nodeLoadIconSize ) * 0.5f;
		float	k			= 0.9;

		glBegin( GL_TRIANGLES );
		for ( int i = 0; i < faceEnabled.Num( ); i++ ) {
			if ( ( faceEnabled[i] == false ) || ( faceLoadId[i] == -1 ) ) {
				continue;
			}
			c	= surfLoadList ->GetSurfLoad( faceLoadId[i] ) ->GetColor( );
			glColor4ub( c.red( ), c.green( ), c.blue( ), 0 );


			float val	= surfLoadList ->GetSurfLoad( faceLoadId[i] ) ->GetVal( );
			if ( val == 0.0f ) {
				continue;
			}

			// Find center position
			int		e = faceElemIds[i][0];
			fVec2	c = 0.5f * ( nodePosition[ faceNodeId[0][i] ] + nodePosition[ faceNodeId[1][i] ] );
			fVec2	f = nodePosition[ faceNodeId[1][i] ] - nodePosition[ faceNodeId[0][i] ];
			fVec2	q = 0.25f *( nodePosition[ elemNodeId[0][e] ] + nodePosition[ elemNodeId[1][e] ] + 
								 nodePosition[ elemNodeId[2][e] ] + nodePosition[ elemNodeId[3][e] ] ) - nodePosition[ faceNodeId[0][i] ];

			// Find fx fy
			if ( f.x*q.y - f.y*q.x > 0 ) {
				fx = nodePosition[ faceNodeId[0][i] ].y - nodePosition[ faceNodeId[1][i] ].y;
				fy = nodePosition[ faceNodeId[1][i] ].x - nodePosition[ faceNodeId[0][i] ].x;
			} else {
				fx = nodePosition[ faceNodeId[1][i] ].y - nodePosition[ faceNodeId[0][i] ].y;
				fy = nodePosition[ faceNodeId[0][i] ].x - nodePosition[ faceNodeId[1][i] ].x;
			}

			if ( val < 0.0f ) {
				fx *= -1.0f;
				fy *= -1.0f;
			}

			a	= size_half / sqrt( fx*fx + fy*fy );
			dx	= a * fx;
			dy	= a * fy;

			float dx0	= k * dy;
			float dy0	= k * dx;

			glVertex2f( c.x - dx - dx0, c.y - dy + dy0 );
			glVertex2f( c.x - dx + dx0, c.y - dy - dy0 );
			glVertex2f( c.x + dx, c.y + dy );
		}
		glEnd( );
	}


	// Draw node displacements
	{
		QColor					c;
		fNodeDisplacementType_e	t;
		float size_half	= camera ->GetModelSize( nodeDispIconSize ) * 0.5f;
		float d			= camera ->GetModelSize( nodeDispLineWidth );

		glLineWidth( nodeDispLineWidth );
		glBegin( GL_LINES );
		for ( int i = 0; i < nodePosition.Num( ); i++ ) {
			if ( ( nodeEnabled[i] == false ) || ( nodeDispId[i] == -1 ) ) {
				continue;
			}
			c = nodeDisplacementList ->GetNodeDisplacement( nodeDispId[i] ) ->GetColor( );
			t = nodeDisplacementList ->GetNodeDisplacement( nodeDispId[i] ) ->GetType( );
			glColor4ub( c.red( ), c.green( ), c.blue( ), 0 );
			if ( t == ND_X ) {
				glVertex2f( nodePosition[i].x - d, nodePosition[i].y - size_half );
				glVertex2f( nodePosition[i].x - d, nodePosition[i].y + size_half );
				glVertex2f( nodePosition[i].x + d, nodePosition[i].y - size_half );
				glVertex2f( nodePosition[i].x + d, nodePosition[i].y + size_half );
			} else if ( t == ND_Y ) {
				glVertex2f( nodePosition[i].x - size_half, nodePosition[i].y - d );
				glVertex2f( nodePosition[i].x + size_half, nodePosition[i].y - d );
				glVertex2f( nodePosition[i].x - size_half, nodePosition[i].y + d );
				glVertex2f( nodePosition[i].x + size_half, nodePosition[i].y + d );
			} else if ( t == ND_ALL ) {
				glVertex2f( nodePosition[i].x - d, nodePosition[i].y - size_half );
				glVertex2f( nodePosition[i].x - d, nodePosition[i].y + size_half );
				glVertex2f( nodePosition[i].x + d, nodePosition[i].y - size_half );
				glVertex2f( nodePosition[i].x + d, nodePosition[i].y + size_half );

				glVertex2f( nodePosition[i].x - size_half, nodePosition[i].y - d );
				glVertex2f( nodePosition[i].x + size_half, nodePosition[i].y - d );
				glVertex2f( nodePosition[i].x - size_half, nodePosition[i].y + d );
				glVertex2f( nodePosition[i].x + size_half, nodePosition[i].y + d );
			}
		}
		glEnd( );
	}
}


void fQuardMesh::OnMaterialRemoved( int id ) {
	for ( int i = 0; i < elemMaterialId.Num( ); i++ ) {
		if ( elemMaterialId[i] > id ) {
			elemMaterialId[i] -= 1;
		}
	}
}



void fQuardMesh::OnNodeDisplacementRemoved( int id ) {
	for ( int i = 0; i < nodeDispId.Num( ); i++ ) {
		if ( nodeDispId[i] == id ) {
			nodeDispId[i] = -1;
		} else if ( nodeDispId[i] > id ) {
			nodeDispId[i] -= 1;
		}
	}
}


void fQuardMesh::OnNodeLoadRemoved( int id ) {
	for ( int i = 0; i < nodeLoadId.Num( ); i++ ) {
		if ( nodeLoadId[i] == id ) {
			nodeLoadId[i] = -1;
		} else if ( nodeLoadId[i] > id ) {
			nodeLoadId[i] -= 1;
		}
	}
}

void fQuardMesh::OnSurfLoadRemoved( int id ) {
	for ( int i = 0; i < faceLoadId.Num( ); i++ ) {
		if ( faceLoadId[i] == id ) {
			faceLoadId[i] = -1;
		} else if ( faceLoadId[i] > id ) {
			faceLoadId[i] -= 1;
		}
	}
}



void fQuardMesh::SetBrushMaterialId( int id ) {
	brushMaterialId = id;
}



void fQuardMesh::SetBrushNodeDisplacementId( int id ) {
	brushNodeDisplacementId = id;
}


void fQuardMesh::SetBrushNodeLoadId( int id ) {
	brushNodeLoadId = id;
}


void fQuardMesh::SetBrushSurfLoadId( int id ) {
	brushSurfLoadId = id;
}


void fQuardMesh::ApplyMaterialForElementsInsideBrush( fCamera2 *camera ) {
	QPoint	screen_pos	= app ->GetBrush( ) ->GetPosition( );
	bool	inclusive	= app ->GetBrush( ) ->GetInclusiveState( );
	fVec2	pos			= camera ->GetModelPositionAt( screen_pos.x( ), screen_pos.y( ) );
	float	rad			= camera ->GetModelSize( app ->GetBrush( ) ->GetDiameter( ) ) * 0.5f;

	if ( inclusive ) {
		float r_sq	= rad * rad;
		fVec2 c		= pos;
		for ( int e = 0; e < elemMaterialId.Num( ); e++ ) {
			// Make sure that the element is enabled
			if ( !elemEnabled[e] ) continue;

			fVec2 v1 = nodePosition[ elemNodeId[0][e] ] - c;
			fVec2 v2 = nodePosition[ elemNodeId[1][e] ] - c;
			fVec2 v3 = nodePosition[ elemNodeId[2][e] ] - c;
			fVec2 v4 = nodePosition[ elemNodeId[3][e] ] - c;
			if ( v1.LengthSqr( ) <= r_sq && v2.LengthSqr( ) <= r_sq && v3.LengthSqr( ) <= r_sq && v4.LengthSqr( ) <= r_sq ) {
				elemMaterialId[e] = brushMaterialId;
			}
		}
	} else {
		float r_sq	= rad * rad;
		fVec2 c		= pos;
		for ( int e = 0; e < elemMaterialId.Num( ); e++ ) {
			// Make sure that the element is enabled
			if ( !elemEnabled[e] ) continue;

			fVec2 p1 = nodePosition[ elemNodeId[0][e] ];
			fVec2 p2 = nodePosition[ elemNodeId[1][e] ];
			fVec2 p3 = nodePosition[ elemNodeId[2][e] ];
			fVec2 p4 = nodePosition[ elemNodeId[3][e] ];

			fVec2 v1 = p1 - c;
			fVec2 v2 = p2 - c;
			fVec2 v3 = p3 - c;
			fVec2 v4 = p4 - c;
			// Fast check 1
			if ( v1.LengthSqr( ) <= r_sq || v2.LengthSqr( ) <= r_sq || v3.LengthSqr( ) <= r_sq || v4.LengthSqr( ) <= r_sq ) {
				elemMaterialId[e] = brushMaterialId;
				continue;
			}
			// Fast check 2
			fVec2 qc = ( p1 + p2 + p3 + p4 ) * 0.25f;
			if ( ( qc - c ).LengthSqr( ) <= r_sq ) {
				elemMaterialId[e] = brushMaterialId;
				continue;
			}
			// Exact check
			if ( fMath::IntersectCircleSegment( pos, rad, p1, p2 ) ) {
				elemMaterialId[e] = brushMaterialId;
			} else if ( fMath::IntersectCircleSegment( pos, rad, p2, p3 ) ) {
				elemMaterialId[e] = brushMaterialId;
			} else if ( fMath::IntersectCircleSegment( pos, rad, p3, p4 ) ) {
				elemMaterialId[e] = brushMaterialId;
			} else if ( fMath::IntersectCircleSegment( pos, rad, p4, p1 ) ) {
				elemMaterialId[e] = brushMaterialId;
			}
		}
	}
}



void fQuardMesh::StartMoveNodesInsideBrush( fCamera2 *camera ) {
	QPoint	screen_pos	= app ->GetBrush( ) ->GetPosition( );
	fVec2	pos			= camera ->GetModelPositionAt( screen_pos.x( ), screen_pos.y( ) );
	float	hardness	= float( app ->GetBrush( ) ->GetHardness( ) ) / 100.0f; 
	float	rad_max		= camera ->GetModelSize( app ->GetBrush( ) ->GetDiameter( ) ) * 0.5f;
	float	rad_min		= rad_max * hardness;
	float	r_max_sq	= rad_max * rad_max;
	float	r_min_sq	= rad_min * rad_min;

	prevBrushPos		= pos;

	// Clear previously grabbed nodes
	moveNodeIds.Clear( );
	moveNodeIds.SetGranularity( 1024 );
	moveNodeFrac.Clear( );
	moveNodeFrac.SetGranularity( 1024 );

	// Find all nodes inside the brush
	for ( int i = 0; i < nodePosition.Num( ); i++ ) {
		// Make sure that the element is enabled
		if ( !nodeEnabled[i] ) {
			continue;
		}

		float d_sq	= ( pos - nodePosition[i] ).LengthSqr( );

		if ( d_sq > r_max_sq ) {
			continue;
		}

		moveNodeIds.Append( i );

		if ( d_sq <= r_min_sq ) {
			moveNodeFrac.Append( 1.0f );
		} else {
			if ( ( rad_max - rad_min ) / rad_max < 0.001f ) {
				moveNodeFrac.Append( 1.0f );
			} else {
				float d = sqrt( d_sq );
				moveNodeFrac.Append( ( rad_max - d ) / ( rad_max - rad_min ) );
			}
		}
	}
}


void fQuardMesh::MoveNodesInsideBrush( fCamera2 *camera ) {
	QPoint	screen_pos	= app ->GetBrush( ) ->GetPosition( );
	fVec2	pos			= camera ->GetModelPositionAt( screen_pos.x( ), screen_pos.y( ) );
	fVec2	shift		= pos - prevBrushPos;

	for ( int i = 0; i < moveNodeIds.Num( ); i++ ) {
		int id = moveNodeIds[i];
		nodePosition[id] += shift * moveNodeFrac[i];

	}	
	prevBrushPos		= pos;
}


void fQuardMesh::RemoveElementsInsideBrush( fCamera2 *camera ) {
	QPoint	screen_pos	= app ->GetBrush( ) ->GetPosition( );
	bool	inclusive	= app ->GetBrush( ) ->GetInclusiveState( );
	fVec2	pos			= camera ->GetModelPositionAt( screen_pos.x( ), screen_pos.y( ) );
	float	rad			= camera ->GetModelSize( app ->GetBrush( ) ->GetDiameter( ) ) * 0.5f;

	
	if ( inclusive ) {
		float r_sq	= rad * rad;
		fVec2 c		= pos;
		for ( int e = 0; e < elemMaterialId.Num( ); e++ ) {
			// Make sure that the element is enabled
			if ( !elemEnabled[e] ) continue;

			fVec2 v1 = nodePosition[ elemNodeId[0][e] ] - c;
			fVec2 v2 = nodePosition[ elemNodeId[1][e] ] - c;
			fVec2 v3 = nodePosition[ elemNodeId[2][e] ] - c;
			fVec2 v4 = nodePosition[ elemNodeId[3][e] ] - c;
			if ( v1.LengthSqr( ) <= r_sq && v2.LengthSqr( ) <= r_sq && v3.LengthSqr( ) <= r_sq && v4.LengthSqr( ) <= r_sq ) {
				DisableElement( e );
			}
		}
	} else {
		fBounds2 brush_bounds( pos - fVec2( rad, rad ), pos +  fVec2( rad, rad ) );
		float r_sq	= rad * rad;
		fVec2 c		= pos;
		for ( int e = 0; e < elemMaterialId.Num( ); e++ ) {
			// Make sure that the element is enabled
			if ( !elemEnabled[e] ) continue;

			fVec2 p1 = nodePosition[ elemNodeId[0][e] ];
			fVec2 p2 = nodePosition[ elemNodeId[1][e] ];
			fVec2 p3 = nodePosition[ elemNodeId[2][e] ];
			fVec2 p4 = nodePosition[ elemNodeId[3][e] ];

			fVec2 v1 = p1 - c;
			fVec2 v2 = p2 - c;
			fVec2 v3 = p3 - c;
			fVec2 v4 = p4 - c;
			// Fast check
			if ( v1.LengthSqr( ) <= r_sq || v2.LengthSqr( ) <= r_sq || v3.LengthSqr( ) <= r_sq || v4.LengthSqr( ) <= r_sq ) {
				DisableElement( e );
				continue;
			}
			// Fast check 2
			fVec2 qc = ( p1 + p2 + p3 + p4 ) * 0.25f;
			if ( ( qc - c ).LengthSqr( ) <= r_sq ) {
				DisableElement( e );
				continue;
			}
			// Exact check
			if ( fMath::IntersectCircleSegment( pos, rad, p1, p2 ) ) {
				DisableElement( e );
			} else if ( fMath::IntersectCircleSegment( pos, rad, p2, p3 ) ) {
				DisableElement( e );
			} else if ( fMath::IntersectCircleSegment( pos, rad, p3, p4 ) ) {
				DisableElement( e );
			} else if ( fMath::IntersectCircleSegment( pos, rad, p4, p1 ) ) {
				DisableElement( e );
			}
		}
	}
}


void fQuardMesh::SetNodeDisplacementForNodesInsideBrush( fCamera2 *camera ) {
	QPoint	screen_pos	= app ->GetBrush( ) ->GetPosition( );
	fVec2	pos			= camera ->GetModelPositionAt( screen_pos.x( ), screen_pos.y( ) );
	float	rad			= camera ->GetModelSize( app ->GetBrush( ) ->GetDiameter( ) ) * 0.5f;


	float r_sq	= rad * rad;
	// Find all nodes inside the brush
	for ( int i = 0; i < nodePosition.Num( ); i++ ) {
		// Make sure that the element is enabled
		if ( !nodeEnabled[i] ) {
			continue;
		}
		float d_sq	= ( pos - nodePosition[i] ).LengthSqr( );
		if ( d_sq <= r_sq ) {
			nodeDispId[i] = brushNodeDisplacementId;
		}
	}
}


void fQuardMesh::RemoveNodeDisplacementForNodesInsideBrush( fCamera2 *camera ) {
	QPoint	screen_pos	= app ->GetBrush( ) ->GetPosition( );
	fVec2	pos			= camera ->GetModelPositionAt( screen_pos.x( ), screen_pos.y( ) );
	float	rad			= camera ->GetModelSize( app ->GetBrush( ) ->GetDiameter( ) ) * 0.5f;


	float r_sq	= rad * rad;
	// Find all nodes inside the brush
	for ( int i = 0; i < nodePosition.Num( ); i++ ) {
		// Make sure that the element is enabled
		if ( !nodeEnabled[i] ) {
			continue;
		}
		float d_sq	= ( pos - nodePosition[i] ).LengthSqr( );
		if ( d_sq <= r_sq ) {
			nodeDispId[i] = -1;
		}
	}
}


void fQuardMesh::SetNodeLoadForNodesInsideBrush( fCamera2 *camera ) {
	QPoint	screen_pos	= app ->GetBrush( ) ->GetPosition( );
	fVec2	pos			= camera ->GetModelPositionAt( screen_pos.x( ), screen_pos.y( ) );
	float	rad			= camera ->GetModelSize( app ->GetBrush( ) ->GetDiameter( ) ) * 0.5f;


	float r_sq	= rad * rad;
	// Find all nodes inside the brush
	for ( int i = 0; i < nodePosition.Num( ); i++ ) {
		// Make sure that the element is enabled
		if ( !nodeEnabled[i] ) {
			continue;
		}
		float d_sq	= ( pos - nodePosition[i] ).LengthSqr( );
		if ( d_sq <= r_sq ) {
			nodeLoadId[i] = brushNodeLoadId;
		}
	}
}


void fQuardMesh::RemoveNodeLoadForNodesInsideBrush( fCamera2 *camera ) {
	QPoint	screen_pos	= app ->GetBrush( ) ->GetPosition( );
	fVec2	pos			= camera ->GetModelPositionAt( screen_pos.x( ), screen_pos.y( ) );
	float	rad			= camera ->GetModelSize( app ->GetBrush( ) ->GetDiameter( ) ) * 0.5f;


	float r_sq	= rad * rad;
	// Find all nodes inside the brush
	for ( int i = 0; i < nodePosition.Num( ); i++ ) {
		// Make sure that the element is enabled
		if ( !nodeEnabled[i] ) {
			continue;
		}
		float d_sq	= ( pos - nodePosition[i] ).LengthSqr( );
		if ( d_sq <= r_sq ) {
			nodeLoadId[i] = -1;
		}
	}
}


void fQuardMesh::SetSurfLoadForFacesInsideBrush( fCamera2 *camera ) {
	QPoint	screen_pos	= app ->GetBrush( ) ->GetPosition( );
	bool	inclusive	= app ->GetBrush( ) ->GetInclusiveState( );
	fVec2	pos			= camera ->GetModelPositionAt( screen_pos.x( ), screen_pos.y( ) );
	float	rad			= camera ->GetModelSize( app ->GetBrush( ) ->GetDiameter( ) ) * 0.5f;

	
	if ( inclusive ) {
		float r_sq	= rad * rad;
		fVec2 c		= pos;
		for ( int f = 0; f < faceEnabled.Num( ); f++ ) {
			// Make sure that the element is enabled
			if ( !faceEnabled[f] ) continue;
			if ( faceElemNum[f] != 1 ) continue;

			fVec2 v1 = nodePosition[ faceNodeId[0][f] ] - c;
			fVec2 v2 = nodePosition[ faceNodeId[1][f] ] - c;
			if ( v1.LengthSqr( ) <= r_sq && v2.LengthSqr( ) <= r_sq ) {
				faceLoadId[f] = brushSurfLoadId;
			}
		}
	} else {
		fBounds2 brush_bounds( pos - fVec2( rad, rad ), pos +  fVec2( rad, rad ) );
		for ( int f = 0; f < faceEnabled.Num( ); f++ ) {
			// Make sure that the element is enabled
			if ( !faceEnabled[f] ) continue;
			if ( faceElemNum[f] != 1 ) continue;

			fVec2 p1 = nodePosition[ faceNodeId[0][f] ];
			fVec2 p2 = nodePosition[ faceNodeId[1][f] ];
			
			// Quick check with bounding boxes
			float x_min = p1.x < p2.x ? p1.x : p2.x;
			float x_max = p1.x > p2.x ? p1.x : p2.x;
			float y_min = p1.y < p2.y ? p1.y : p2.y;
			float y_max = p1.y > p2.y ? p1.y : p2.y;
			if ( !brush_bounds.IntersectsBounds( fBounds2( fVec2( x_min, y_min ), fVec2( x_max, y_max ) ) ) ) {
				continue;
			}

			// Exact check
			if ( fMath::IntersectCircleSegment( pos, rad, p1, p2 ) ) {
				faceLoadId[f] = brushSurfLoadId;
			}
		}
	}
}


void fQuardMesh::RemoveSurfLoadForFacesInsideBrush( fCamera2 *camera ) {
	QPoint	screen_pos	= app ->GetBrush( ) ->GetPosition( );
	bool	inclusive	= app ->GetBrush( ) ->GetInclusiveState( );
	fVec2	pos			= camera ->GetModelPositionAt( screen_pos.x( ), screen_pos.y( ) );
	float	rad			= camera ->GetModelSize( app ->GetBrush( ) ->GetDiameter( ) ) * 0.5f;

	
	if ( inclusive ) {
		float r_sq	= rad * rad;
		fVec2 c		= pos;
		for ( int f = 0; f < faceEnabled.Num( ); f++ ) {
			// Make sure that the element is enabled
			if ( !faceEnabled[f] ) continue;

			fVec2 v1 = nodePosition[ faceNodeId[0][f] ] - c;
			fVec2 v2 = nodePosition[ faceNodeId[1][f] ] - c;
			if ( v1.LengthSqr( ) <= r_sq && v2.LengthSqr( ) <= r_sq ) {
				faceLoadId[f] = -1;
			}
		}
	} else {
		fBounds2 brush_bounds( pos - fVec2( rad, rad ), pos +  fVec2( rad, rad ) );
		for ( int f = 0; f < faceEnabled.Num( ); f++ ) {
			// Make sure that the element is enabled
			if ( !faceEnabled[f] ) continue;

			fVec2 p1 = nodePosition[ faceNodeId[0][f] ];
			fVec2 p2 = nodePosition[ faceNodeId[1][f] ];
			
			// Quick check with bounding boxes
			float x_min = p1.x < p2.x ? p1.x : p2.x;
			float x_max = p1.x > p2.x ? p1.x : p2.x;
			float y_min = p1.y < p2.y ? p1.y : p2.y;
			float y_max = p1.y > p2.y ? p1.y : p2.y;
			if ( !brush_bounds.IntersectsBounds( fBounds2( fVec2( x_min, y_min ), fVec2( x_max, y_max ) ) ) ) {
				continue;
			}

			// Exact check
			if ( fMath::IntersectCircleSegment( pos, rad, p1, p2 ) ) {
				faceLoadId[f] = -1;
			}
		}
	}
}





void fQuardMesh::DisableElement( int e ) {
	elemEnabled[e] = false;

	// Remove adjacent nodes if needed
	{
		int id1 = elemNodeId[0][e];
		int id2 = elemNodeId[1][e];
		int id3 = elemNodeId[2][e];
		int id4 = elemNodeId[3][e];
		nodeElemIds[id1].Remove( e );
		nodeElemIds[id2].Remove( e );
		nodeElemIds[id3].Remove( e );
		nodeElemIds[id4].Remove( e );

		nodeElemNum[id1] = nodeElemNum[id1] - 1;
		nodeElemNum[id2] = nodeElemNum[id2] - 1;
		nodeElemNum[id3] = nodeElemNum[id3] - 1;
		nodeElemNum[id4] = nodeElemNum[id4] - 1;

		if ( nodeElemNum[id1] == 0 ) {
			nodeElemIds[id1].Clear( );
			nodeEnabled[id1] = false;
		}
		if ( nodeElemNum[id2] == 0 ) {
			nodeElemIds[id2].Clear( );
			nodeEnabled[id2] = false;
		}
		if ( nodeElemNum[id3] == 0 ) {
			nodeElemIds[id3].Clear( );
			nodeEnabled[id3] = false;
		}
		if ( nodeElemNum[id4] == 0 ) {
			nodeElemIds[id4].Clear( );
			nodeEnabled[id4] = false;
		}
	}

	// Remove adjacent faces if needed
	{
		int id1 = elemFaceId[0][e];
		int id2 = elemFaceId[1][e];
		int id3 = elemFaceId[2][e];
		int id4 = elemFaceId[3][e];

		int eid1 = faceElemIds[id1].FindIndex( e );
		int eid2 = faceElemIds[id2].FindIndex( e );
		int eid3 = faceElemIds[id3].FindIndex( e );
		int eid4 = faceElemIds[id4].FindIndex( e );

		faceElemIds[id1].RemoveIndex( eid1 );
		faceElemIds[id2].RemoveIndex( eid2 );
		faceElemIds[id3].RemoveIndex( eid3 );
		faceElemIds[id4].RemoveIndex( eid4 );

		faceElemPosIds[id1].RemoveIndex( eid1 );
		faceElemPosIds[id2].RemoveIndex( eid2 );
		faceElemPosIds[id3].RemoveIndex( eid3 );
		faceElemPosIds[id4].RemoveIndex( eid4 );

		faceElemNum[id1] = faceElemNum[id1] - 1;
		faceElemNum[id2] = faceElemNum[id2] - 1;
		faceElemNum[id3] = faceElemNum[id3] - 1;
		faceElemNum[id4] = faceElemNum[id4] - 1;

		if ( faceElemNum[id1] == 0 ) {
			faceElemIds[id1].Clear( );
			faceElemPosIds[id1].Clear( );
			faceEnabled[id1] = false;
		}
		if ( faceElemNum[id2] == 0 ) {
			faceElemIds[id2].Clear( );
			faceElemPosIds[id2].Clear( );
			faceEnabled[id2] = false;
		}
		if ( faceElemNum[id3] == 0 ) {
			faceElemIds[id3].Clear( );
			faceElemPosIds[id3].Clear( );
			faceEnabled[id3] = false;
		}
		if ( faceElemNum[id4] == 0 ) {
			faceElemIds[id4].Clear( );
			faceElemPosIds[id4].Clear( );
			faceEnabled[id4] = false;
		}
	}
}



void fQuardMesh::RemoveElementsWithMaterialId( int id ) {
	for ( int e = 0; e < elemMaterialId.Num( ); e++ ) {
		// Make sure that the element is enabled
		if ( !elemEnabled[e] ) {
			continue;
		}

		if ( elemMaterialId[e] == id ) {
			DisableElement( e );
		}
	}
}


void fQuardMesh::StopMoveNodesInsideBrush( fCamera2 *camera ) {
	moveNodeIds.Clear( );
	moveNodeFrac.Clear( );
}


fBounds2 fQuardMesh::GetBounds( ) {
	// Update bounds based on the currently active nodes
	int		node_from = 0;
	fVec2	v_min( 0.0f, 0.0f );
	fVec2	v_max( 0.0f, 0.0f );

	// Find first active node
	for ( int i = 0; i < nodeEnabled.Num( ); i++ ) {
		if ( nodeEnabled[i] ) {
			node_from = i + 1;
			v_min = nodePosition[i];
			v_max = nodePosition[i];
			break;
		}
	}

	// Find v_min/v_max
	for ( int i = node_from; i < nodeEnabled.Num( ); i++ ) {
		if ( nodeEnabled[i] ) {
			if ( nodePosition[i].x > v_max.x ) {
				v_max.x = nodePosition[i].x;
			} else if ( nodePosition[i].x < v_min.x ) {
				v_min.x = nodePosition[i].x;
			}

			if ( nodePosition[i].y > v_max.y ) {
				v_max.y = nodePosition[i].y;
			} else if ( nodePosition[i].y < v_min.y ) {
				v_min.y = nodePosition[i].y;
			}
		}
	}

	fVec2 bnd_bl;
	fVec2 bnd_tr;
	if ( v_min.x == v_max.x ) {
		bnd_bl.x = v_min.x - 1.0f;
		bnd_tr.x = v_min.x + 1.0f;
	} else {
		bnd_bl.x = v_min.x;
		bnd_tr.x = v_max.x;
	}

	if ( v_min.y == v_max.y ) {
		bnd_bl.y = v_min.y - 1.0f;
		bnd_tr.y = v_min.y + 1.0f;
	} else {
		bnd_bl.y = v_min.y;
		bnd_tr.y = v_max.y;
	}

	bounds = fBounds2( bnd_bl, bnd_tr );
	return bounds;
}


void fQuardMesh::OnMousePress( QMouseEvent *event, fCamera2 *camera ) {
	switch ( editOperation ) {
		case QME_PAINT_ELEM_SET_MATERIAL: {
			if ( event ->buttons( ) & Qt::LeftButton ) {
				ApplyMaterialForElementsInsideBrush( camera );
			}
			break;
		}
		case QME_PAINT_NODE_MOVE: {
			if ( event ->buttons( ) & Qt::LeftButton ) {
				StartMoveNodesInsideBrush( camera );
			}
			break;
		}
		case QME_PAINT_ELEM_REMOVE: {
			if ( event ->buttons( ) & Qt::LeftButton ) { 
			RemoveElementsInsideBrush( camera );
			}
			break;
		}
		case QME_PAINT_NODE_ADD_REMOVE_BNDCOND: {
			if ( event ->buttons( ) & Qt::LeftButton ) {
				SetNodeDisplacementForNodesInsideBrush( camera );
			} else if ( event ->buttons( ) & Qt::RightButton ) {
				RemoveNodeDisplacementForNodesInsideBrush( camera );
			}
			break;
		}
		case QME_PAINT_NODE_ADD_REMOVE_LOAD: {
			if ( event ->buttons( ) & Qt::LeftButton ) {
				SetNodeLoadForNodesInsideBrush( camera );
			} else if ( event ->buttons( ) & Qt::RightButton ) {
				RemoveNodeLoadForNodesInsideBrush( camera );
			}
			break;
		}
		case QME_PAINT_FACE_ADD_REMOVE_LOAD: {
			if ( event ->buttons( ) & Qt::LeftButton ) {
				SetSurfLoadForFacesInsideBrush( camera );
			} else if ( event ->buttons( ) & Qt::RightButton ) {
				RemoveSurfLoadForFacesInsideBrush( camera );
			}
			break;
		}
	}
}


void fQuardMesh::OnMouseMove( QMouseEvent *event, fCamera2 *camera ) {
	switch ( editOperation ) {
		case QME_PAINT_ELEM_SET_MATERIAL: {
			if ( event ->buttons( ) & Qt::LeftButton ) {
				ApplyMaterialForElementsInsideBrush( camera );
			}
			break;
		}
		case QME_PAINT_NODE_MOVE: {
			if ( event ->buttons( ) & Qt::LeftButton ) {
				MoveNodesInsideBrush( camera );
			}
			break;
		}
		case QME_PAINT_ELEM_REMOVE: {
			if ( event ->buttons( ) & Qt::LeftButton ) {
				RemoveElementsInsideBrush( camera );
			}
			break;
		}
		case QME_PAINT_NODE_ADD_REMOVE_BNDCOND: {
			if ( event ->buttons( ) & Qt::LeftButton ) {
				SetNodeDisplacementForNodesInsideBrush( camera );
			} else if ( event ->buttons( ) & Qt::RightButton ) {
				RemoveNodeDisplacementForNodesInsideBrush( camera );
			}
			break;
		}
		case QME_PAINT_NODE_ADD_REMOVE_LOAD: {
			if ( event ->buttons( ) & Qt::LeftButton ) {
				SetNodeLoadForNodesInsideBrush( camera );
			} else if ( event ->buttons( ) & Qt::RightButton ) {
				RemoveNodeLoadForNodesInsideBrush( camera );
			}
			break;
		}						  
		case QME_PAINT_FACE_ADD_REMOVE_LOAD: {
			if ( event ->buttons( ) & Qt::LeftButton ) {
				SetSurfLoadForFacesInsideBrush( camera );
			} else if ( event ->buttons( ) & Qt::RightButton ) {
				RemoveSurfLoadForFacesInsideBrush( camera );
			}
			break;
		}
	}
}


void fQuardMesh::OnMouseRelease( QMouseEvent *event, fCamera2 *camera ) {
	switch ( editOperation ) {
		case QME_PAINT_ELEM_SET_MATERIAL: {
			break;
		}
		case QME_PAINT_NODE_MOVE: {
			break;
		}
		case QME_PAINT_ELEM_REMOVE: {
			break;
		}
		case QME_PAINT_NODE_ADD_REMOVE_BNDCOND: {
			break;
		}
		case QME_PAINT_NODE_ADD_REMOVE_LOAD: {
			break;
		}
		case QME_PAINT_FACE_ADD_REMOVE_LOAD: {
			break;
		}
	}
}


void fQuardMesh::ReadSettings( ) {
	// Get application settings
	QSettings * settings = app ->GetSettings( );

	settings ->beginGroup( "quad_mesh" );
		nodeDispIconSize	= settings ->value( "displacement_icon_size", 11 ).toInt( );
		nodeDispLineWidth	= settings ->value( "displacement_line_width", 1 ).toInt( );
		nodeLoadIconSize	= settings ->value( "load_icon_size", 10 ).toInt( );

		nodeSize			= settings ->value( "node_size", 1 ).toInt( );
		nodeColor			= settings ->value( "node_color", QColor( 255, 255, 255 ) ).value<QColor>( );
		drawNodes			= settings ->value( "draw_nodes", false ).toBool( );

		faceLineWidth		= settings ->value( "face_line_width", 1 ).toInt( );
		faceColor			= settings ->value( "face_color", QColor( 178, 178, 178 ) ).value<QColor>( );
		drawFaces			= settings ->value( "draw_faces", true ).toBool( );


		defaultMat1Color	= settings ->value( "default_material1_color", Qt::blue ).value<QColor>( );
		defaultMat2Color	= settings ->value( "default_material2_color", QColor(0, 80, 0) ).value<QColor>( );
		defaultLoadColor	= settings ->value( "default_load_color", Qt::black ).value<QColor>( );
		defaultSurfLoadColor= settings ->value( "default_surf_load_color", QColor(20, 30, 120) ).value<QColor>( );
		defaultDispColor	= settings ->value( "default_displacement_color", QColor(150, 0, 0) ).value<QColor>( );
	settings ->endGroup( );
}


void fQuardMesh::WriteSettings( ) {
	QSettings * settings = app ->GetSettings( );

	settings ->beginGroup( "quad_mesh" );
		settings ->setValue( "displacement_icon_size", nodeDispIconSize );
		settings ->setValue( "displacement_line_width", nodeDispLineWidth );
		settings ->setValue( "load_icon_size", nodeLoadIconSize );

		settings ->setValue( "node_size", nodeSize );
		settings ->setValue( "node_color", nodeColor );
		settings ->setValue( "draw_nodes", drawNodes );

		settings ->setValue( "face_line_width", faceLineWidth );
		settings ->setValue( "face_color", faceColor );
		settings ->setValue( "draw_faces", drawFaces );

		settings ->setValue( "default_material1_color", defaultMat1Color );
		settings ->setValue( "default_material2_color", defaultMat2Color );
		settings ->setValue( "default_load_color", defaultLoadColor );
		settings ->setValue( "default_displacement_color", defaultDispColor );
	settings ->endGroup( );
}


QString fQuardMesh::GetFileFilters( ) {
	return QString( "Ansys mesh file (*.dat)" );
}


void fQuardMesh::Save( QString &file_name, QString &filter_name ) {
	int save_format = 0;
	int node_num	= nodeEnabled.Num( );
	int elem_num	= elemEnabled.Num( );

	if ( filter_name == "Matlab file (*.dat)" ) {
		save_format = 0;
	} else {
		save_format = 0;
	}
	QFileInfo fi( file_name );
	if ( fi.suffix( ) != "dat" ) {
		file_name.append( ".dat" );
	}


	/* =================================================
					Compiling the mesh
	================================================= */

	// Run through all enabled nodes
	fList<int>			node_new_id;
	fList<int>			elem_new_id;
	fList<fVec2>		node_position;

	fList<int>			elem_node_id[4];
	fList<int>			elem_mater_id;

	fList<int>			node_load_id;
	fList<int>			node_disp_id;
	fList<int>			node_load_node_id;
	fList<int>			node_disp_node_id;

	fList<int>			f_elem_id;
	fList<int>			f_load_id;
	fList<int>			f_side_id;

	// Setup high granularity
	int granularity = 1024 * 64;
	node_position.SetGranularity( granularity );
	node_load_id.SetGranularity( granularity );
	node_disp_id.SetGranularity( granularity );
	node_disp_node_id.SetGranularity( granularity );
	node_load_node_id.SetGranularity( granularity );
	// New id of the node in the list of old nodes
	node_new_id.SetNum( node_num );
	node_new_id.SetVal( -1 );

	elem_node_id[0].SetGranularity( granularity );
	elem_node_id[1].SetGranularity( granularity );
	elem_node_id[2].SetGranularity( granularity );
	elem_node_id[3].SetGranularity( granularity );
	elem_mater_id.SetGranularity( granularity );
	// New id of the element in the list of old elements
	elem_new_id.SetNum( elem_num );
	elem_new_id.SetVal( -1 );


	// 1) Run through the nodes first
	for ( int i = 0; i < node_num; i++ ) {
		// 1) Check if the node is enabled
		if ( !nodeEnabled[i] ) {
			continue;
		}
		// Append node with its' position and save its id
		node_new_id[i] = node_position.Append( nodePosition[i] );

		// 3) Boundary displacements
		if ( nodeDispId[i] != -1 && nodeDispId[i] < nodeDisplacementList ->GetNum( ) ) {
			node_disp_id.Append( nodeDispId[i] );
			node_disp_node_id.Append( node_new_id[i] );
		}

		// 4) Boundary loads
		if ( nodeLoadId[i] != -1 && nodeLoadId[i] < nodeLoadList ->GetNum( ) ) {
			node_load_id.Append( nodeLoadId[i] );
			node_load_node_id.Append( node_new_id[i] );
		}
	}


	// 3) Run through the elements next
	int mat_id_max = 0;
	for ( int e = 0; e < elem_num; e++ ) {
		// 1) Check if the element is enabled
		if ( !elemEnabled[e] ) {
			continue;
		}
		// Append node with its' position and save its id
		int node_id0	= node_new_id[ elemNodeId[0][e] ];
		int node_id1	= node_new_id[ elemNodeId[1][e] ];
		int node_id2	= node_new_id[ elemNodeId[2][e] ];
		int node_id3	= node_new_id[ elemNodeId[3][e] ];
		int mat_id		= elemMaterialId[e];

		elem_node_id[0].Append( node_id0 );
		elem_node_id[1].Append( node_id1 );
		elem_node_id[2].Append( node_id2 );
		elem_node_id[3].Append( node_id3 );
		elem_new_id[e] = elem_mater_id.Append( mat_id );

		if ( mat_id > mat_id_max ) {
			mat_id_max = mat_id;
		}
	}
	int mat_num = mat_id_max + 1;


	// 2) Run through the faces
	f_elem_id.SetGranularity( granularity );
	f_load_id.SetGranularity( granularity );
	f_side_id.SetGranularity( granularity );
	for ( int f = 0; f < faceEnabled.Num( ); f++ ) {
		if ( ( faceEnabled[f] == false ) || ( faceLoadId[f] == -1 ) || ( faceElemNum[f] != 1 ) ) {
			continue;
		}

		// Element id
		int e_id = elem_new_id[ faceElemIds[f][0] ];
		int f_id = faceElemPosIds[f][0];

		f_elem_id.Append( e_id );
		f_load_id.Append( faceLoadId[f] );
		f_side_id.Append( f_id );
	}

	// Sort elements per material
	fList<fList<int> > elem_per_mat_id;
	elem_per_mat_id.SetNum( mat_num );
	for ( int i = 0; i < mat_num; i++ ) {
		elem_per_mat_id[i].SetGranularity( elem_num );
	}

	for ( int e = 0; e < elem_mater_id.Num( ); e++ ) {
		int mat_id = elem_mater_id[e];
		elem_per_mat_id[mat_id].Append( e );
	}


	/* =================================================
						Reordering
	================================================= */
	
	


	/* =================================================
							Saving
	================================================= */


	// Open the file
	FILE *file = fopen( file_name.toAscii( ).data( ), "w" );

	// Intro
	fprintf( file, "/TITLE, CONTINUUM GROUND STRUCTURE\n" );
	QString first_line = QString( "! Created with:       " ) + app ->GetName( ) + QString( " " ) + app ->GetVersion( ) + QString( "\n" );
	fprintf( file, first_line.toAscii( ).data( ) );
	fprintf( file, "! Element type:       PLANE42\n" );
	fprintf( file, "! Number of nodes:    %d\n", node_position.Num( ) );
	fprintf( file, "! Number of elements: %d\n\n", elem_mater_id.Num( ) );

	fprintf( file, "/PREP7\n" );
	fprintf( file, "! Element type: ET, type #, name\n" );
	fprintf( file, "ET, 1, PLANE42, , , 3\n" );

	fprintf( file, "\n" );

	
	// 1) Write nodes
	fprintf( file, "! Nodal coordinates: \'N\', node_id, x-coord, y-coord, z-coord\n" );
	for ( int i = 0; i < node_position.Num( ); i++ ) {
		fprintf( file, "N,\t%d,\t%g,\t%g,\t0\n", i + 1, node_position[i].x, node_position[i].y );
	}
	fprintf( file, "\n" );


	// 2) Write elements per material
	fprintf( file, "! Element connectivity: \'EN\', element_id,  node_ids\n" );
	fprintf( file, "! Material:             \'MP\', property_id, material_id, value \n" );
	fprintf( file, "! Custom property:      \'R\',  property_id, value \n" );

	for ( int m = 0; m < mat_num; m++ ) {
		fprintf( file, "! Material %d\n", m + 1 );
		{
			fprintf( file, "MP,\tEX,  \t%d,\t%g\n", m + 1, materialList ->GetMaterial( m ) ->GetEx( ) );
			fprintf( file, "MP,\tEY,  \t%d,\t%g\n", m + 1, materialList ->GetMaterial( m ) ->GetEy( ) );
			fprintf( file, "MP,\tGXY, \t%d,\t%g\n", m + 1, materialList ->GetMaterial( m ) ->GetGxy( ) );
			fprintf( file, "MP,\tPRXY,\t%d,\t%g\n", m + 1, materialList ->GetMaterial( m ) ->GetPoisson( ) );
			fprintf( file, "MP,\tDENS,\t%d,\t%g\n", m + 1, materialList ->GetMaterial( m ) ->GetDens( ) );
		}
		//fprintf( file, "R, \t1,\t%g\n",materialList ->GetMaterial( m ) ->GetThickness( ) );
		fprintf( file, "R, \t%d,\t%g\n", m + 1, materialList ->GetMaterial( m ) ->GetThickness( ) ); // CLFE change

		fprintf( file, "! Connectivity\n" );
		for ( int e = 0; e < elem_per_mat_id[m].Num( ); e++ ) {
			int id = elem_per_mat_id[m][e];
			//fprintf( file, "EN,\t%d\t%d\t%d\t%d\t%d\n", id + 1, 
                        fprintf( file, "EN,\t%d,\t%d,\t%d,\t%d,\t%d\n", id + 1, // CLFE change
                                elem_node_id[0][id] + 1, 
				elem_node_id[1][id] + 1,
				elem_node_id[2][id] + 1,
				elem_node_id[3][id] + 1 );
		}
	}
	fprintf( file, "\n" );


	// 3) Write nodal displacements
	fprintf( file, "! Nodal diplacement: \'D\', node_id, \'UX / UY\', value\n" );
	for ( int i = 0; i < node_disp_id.Num( ); i++ ) {
		fNodeDisplacementType_e type	= nodeDisplacementList ->GetNodeDisplacement( node_disp_id[i] ) ->GetType( );
		float					dx		= nodeDisplacementList ->GetNodeDisplacement( node_disp_id[i] ) ->GetDx( );
		float					dy		= nodeDisplacementList ->GetNodeDisplacement( node_disp_id[i] ) ->GetDy( );

		switch ( type ) {
			case ND_X : {
				fprintf( file, "D,\t%d,\tUX,\t%g\n", node_disp_node_id[i] + 1, dx );
				break;
			}
			case ND_Y : {
				fprintf( file, "D,\t%d,\tUY,\t%g\n", node_disp_node_id[i] + 1, dy );
				break;
			}
			case ND_ALL : {
				fprintf( file, "D,\t%d,\tUX,\t%g\n", node_disp_node_id[i] + 1, dx );
				fprintf( file, "D,\t%d,\tUY,\t%g\n", node_disp_node_id[i] + 1, dy );
				break;
			}
		};
	}
	fprintf( file, "\n" );

	
	// 4) Write nodal loads
	fprintf( file, "! Nodal load: \'F\', node_id, \'FX / FY\', value\n" );
	for ( int i = 0; i < node_load_id.Num( ); i++ ) {
		float fx = nodeLoadList ->GetNodeLoad( node_load_id[i] ) ->GetFx( );
		float fy = nodeLoadList ->GetNodeLoad( node_load_id[i] ) ->GetFy( );

		if ( abs(fx) > FLT_MIN ) {
			fprintf( file, "F,\t%d,\tFX,\t%g\n", node_load_node_id[i] + 1, fx );
		}
		if ( abs(fy) > FLT_MIN ) {
			fprintf( file, "F,\t%d,\tFY,\t%g\n", node_load_node_id[i] + 1, fy );
		}
	}
	fprintf( file, "\n" );

	// 5) Write surface loads
	fprintf( file, "! Surface load: \'SFE\', element_id, face_id, \'PRES\', 0, value\n" );
	for ( int f = 0; f < f_elem_id.Num( ); f++ ) {
		float val = surfLoadList ->GetSurfLoad( f_load_id[f] ) ->GetVal( );
		fprintf( file, "SFE,\t%d,\t%d,\tPRESS,\t0,\t%g\n", f_elem_id[f] + 1, f_side_id[f] + 1, val );
	}
	fprintf( file, "\n" );


	// 6) Write acceleration
	fprintf( file, "! Acceleration: \'ACEL\', x-value, y-value, z-value\n" );
	fprintf( file, "ACEL,\t0.0,\t0.0,\t0.0\n" );
	fprintf( file, "\nFINISH\n" );

	fclose( file );



}

