#include "../precompiled.h"



// Element materials
fTrussMaterial::fTrussMaterial( ) : QObject( ) {
	this ->A			= 1.0f;
	this ->E			= 1.0f;
	this ->color		= Qt::black;
}


fTrussMaterial::fTrussMaterial( const QString &name, const QColor &clr, float A, float E ) : QObject( ) {
	this ->setObjectName( name );
	this ->color		= clr;
	this ->A			= A;
	this ->E			= E;
}


fTrussMaterial::~fTrussMaterial( ) {
	
}




// Truss material list
fTrussMaterialList::fTrussMaterialList( QObject *parent ) : QAbstractListModel( parent ) {
	
}


fTrussMaterialList::~fTrussMaterialList(  ) {
	qDeleteAll( materials );
}


QVariant fTrussMaterialList::data( const QModelIndex &index, int role ) const {
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


Qt::ItemFlags fTrussMaterialList::flags( const QModelIndex & index ) const {
	Qt::ItemFlags flags = Qt::NoItemFlags;

	if ( index != QModelIndex( ) ) {
		flags |= Qt::ItemIsEnabled;
		flags |= Qt::ItemIsSelectable;
	}

	return flags;
}


int fTrussMaterialList::rowCount( const QModelIndex & parent ) const {
	return materials.size( );
}


void fTrussMaterialList::AddMaterial( fTrussMaterial *mat ) {
	// Let views know what we are doing
	beginInsertRows( QModelIndex( ), materials.size( ), materials.size( ) );
	materials.append( mat );
	endInsertRows( );
}


void fTrussMaterialList::RemoveMaterial( int id ) {
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

















fTrussMesh::fTrussMesh( float width, float height, int nx, int ny, fTrussMeshConnectivity_e connectivity, int con_size ) {
	editOperation			= TME_SELECTION;

	materialList			= NULL;
	brushMaterialId			= 0;
	nodeDisplacementList	= NULL;
	brushNodeDisplacementId = 0;
	nodeLoadList			= NULL;
	brushNodeLoadId			= 0;

	ReadSettings( );

	// Resize node arrays
	int node_num	= nx*ny;
	nodePosition.SetNum( node_num );
	nodeSelectStatus.SetNum( node_num );
	nodeElemIds.SetNum( node_num );
	nodeElemNum.SetNum( node_num );
	nodeEnabled.SetNum( node_num );

	nodeEnabled.SetVal( true );

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

	// Material list and co
	materialList			= new fTrussMaterialList( );
	nodeDisplacementList	= new fNodeDisplacementList( );
	nodeLoadList			= new fNodeLoadList( );

	// Initialize elements
	int elem_num = 0;
	if ( connectivity == TMC_NEIGHBOR_NODES ) {
		int granularity = 64 * 1024;
		elemNodeId1.SetGranularity( node_num * 4 );
		elemNodeId2.SetGranularity( node_num * 4 );
		// For all nodes connect with the nodes, having higer id
		for ( int i = 0; i < nx; i++ ) {
			for ( int j = 0; j < ny; j++ ) {
				// Current node
				int id0 = i*ny + j;
				// Neighboring nodes
				int id1 = (i + 0)*ny + (j + 1);
				int id2 = (i + 1)*ny + (j + 1);
				int id3 = (i + 1)*ny + (j + 0);
				int id4 = (i + 1)*ny + (j - 1);

				// Change granularity for the nodeElemIds
				nodeElemIds[id0].SetGranularity( 8 );

				// Check for validity of the surrounding nodes
				bool id1_valid = true;
				bool id2_valid = true;
				bool id3_valid = true;
				bool id4_valid = true;
				// Top
				if ( (j + 1) >= ny ) {
					id1_valid = false;
					id2_valid = false;
				}
				// Right
				if ( (i + 1) >= nx ) {
					id2_valid = false;
					id3_valid = false;
					id4_valid = false;
				}
				// Bottom
				if ( (j - 1) < 0 ) {
					id4_valid = false;
				}

				// Append nodes
				if ( id1_valid ) {
					elemNodeId1.Append( id1 );
					elemNodeId2.Append( id0 );
					nodeElemIds[id1].Append( elem_num );
					nodeElemIds[id0].Append( elem_num );
					elem_num++;
				}
				if ( id2_valid ) {
					elemNodeId1.Append( id2 );
					elemNodeId2.Append( id0 );
					nodeElemIds[id2].Append( elem_num );
					nodeElemIds[id0].Append( elem_num );
					elem_num++;
				}
				if ( id3_valid ) {
					elemNodeId1.Append( id3 );
					elemNodeId2.Append( id0 );
					nodeElemIds[id3].Append( elem_num );
					nodeElemIds[id0].Append( elem_num );
					elem_num++;
				}
				if ( id4_valid ) {
					elemNodeId1.Append( id4 );
					elemNodeId2.Append( id0 );
					nodeElemIds[id4].Append( elem_num );
					nodeElemIds[id0].Append( elem_num );
					elem_num++;
				}
			}
		}
	// Connect all to all
	} else {
		int granularity = 64 * 1024;
		elemNodeId1.SetGranularity( node_num * 4 );
		elemNodeId2.SetGranularity( node_num * 4 );

		// Find radius
		float max_dist   = ( nodePosition[0].x - nodePosition[node_num - 1].x ) * ( nodePosition[0].x - nodePosition[node_num - 1].x );
		max_dist		+= ( nodePosition[0].y - nodePosition[node_num - 1].y ) * ( nodePosition[0].y - nodePosition[node_num - 1].y );
		max_dist		 = sqrt( max_dist);
		float r			 = 0.01f * float( con_size ) * max_dist;

		float min_dist	 = sqrt( ( nodePosition[0].x - nodePosition[1].x )*( nodePosition[0].x - nodePosition[1].x ) + ( nodePosition[0].y - nodePosition[1].y )*( nodePosition[0].y - nodePosition[1].y ) );
		if ( r <= min_dist * 1.1f ) {
			r = min_dist * 1.1f;
		}

		float r_sq		 = r * r;
		
		// Find node within specific radius and connect them
		for ( int i = 0; i < nx; i++ ) {
			for ( int j = 0; j < ny; j++ ) {
				// Current node
				int id0 = i*ny + j;
				fVec2 curr_node_pos = nodePosition[id0];

				// Change granularity for the nodeElemIds
				nodeElemIds[id0].SetGranularity( 8 );

				for ( int n = id0 + 1; n < node_num; n++ ) {
					fVec2 local_node_pos = nodePosition[n];
					if ( ( local_node_pos - curr_node_pos ).LengthSqr( ) <= r_sq ) {
						elemNodeId1.Append( n );
						elemNodeId2.Append( id0 );
						nodeElemIds[n].Append( elem_num );
						nodeElemIds[id0].Append( elem_num );
						elem_num++;
					}
				}
				
			}
		}
	}

	for ( int i = 0; i < node_num; i++ ) {
		nodeElemNum[i] = nodeElemIds[i].Num( );
	}
	
	elemMaterialId.SetNum( elem_num );
	elemMaterialId.SetVal( 0 );
	elemEnabled.SetNum( elem_num );
	elemEnabled.SetVal( true );
}


fTrussMesh::~fTrussMesh( ) {
	WriteSettings( );

	if ( materialList ) delete materialList;
	if ( nodeDisplacementList ) delete nodeDisplacementList;
	if ( nodeLoadList ) delete nodeLoadList;
}


void fTrussMesh::Draw( fCamera2 *camera ) {
	// Draw elements
	glLineWidth( elemLineWidth );
	glBegin( GL_LINES );
	for ( int i = 0; i < elemMaterialId.Num( ); i++ ) {
		if ( !elemEnabled[i] ) {
			continue;
		}
		QColor c = materialList ->GetMaterial( elemMaterialId[i] ) ->GetColor( );
		glColor4ub( c.red( ), c.green( ), c.blue( ), 0 );
		glVertex2fv( nodePosition[ elemNodeId1[i] ].ToFloatPtr( ) );
		glVertex2fv( nodePosition[ elemNodeId2[i] ].ToFloatPtr( ) );
	}
	glEnd( );

	// Draw nodes
	if ( drawNodes ) {
		QColor c = nodeColor;
		glPointSize( nodeSize );
		glBegin( GL_POINTS );
		if ( app ->IsPointColorFloatFormat( ) ) {
			glColor4f( float(c.red( ))/ 255.0f, float(c.green( ))/ 255.0f,  float(c.blue( ))/ 255.0f, 0.0f );
		} else {
			glColor4ub( c.red( ), c.green( ), c.blue( ), 0 );
		}
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


void fTrussMesh::OnMaterialRemoved( int id ) {
	for ( int i = 0; i < elemMaterialId.Num( ); i++ ) {
		if ( elemMaterialId[i] > id ) {
			elemMaterialId[i] -= 1;
		}
	}
}



void fTrussMesh::OnNodeDisplacementRemoved( int id ) {
	for ( int i = 0; i < nodeDispId.Num( ); i++ ) {
		if ( nodeDispId[i] == id ) {
			nodeDispId[i] = -1;
		} else if ( nodeDispId[i] > id ) {
			nodeDispId[i] -= 1;
		}
	}
}


void fTrussMesh::OnNodeLoadRemoved( int id ) {
	for ( int i = 0; i < nodeLoadId.Num( ); i++ ) {
		if ( nodeLoadId[i] == id ) {
			nodeLoadId[i] = -1;
		} else if ( nodeLoadId[i] > id ) {
			nodeLoadId[i] -= 1;
		}
	}
}


void fTrussMesh::SetBrushMaterialId( int id ) {
	brushMaterialId = id;
}



void fTrussMesh::SetBrushNodeDisplacementId( int id ) {
	brushNodeDisplacementId = id;
}


void fTrussMesh::SetBrushNodeLoadId( int id ) {
	brushNodeLoadId = id;
}


void fTrussMesh::ApplyMaterialForElementsInsideBrush( fCamera2 *camera ) {
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

			fVec2 v1 = nodePosition[ elemNodeId1[e] ] - c;
			fVec2 v2 = nodePosition[ elemNodeId2[e] ] - c;
			if ( v1.LengthSqr( ) <= r_sq && v2.LengthSqr( ) <= r_sq ) {
				elemMaterialId[e] = brushMaterialId;
			}
		}
	} else {
		for ( int e = 0; e < elemMaterialId.Num( ); e++ ) {
			// Make sure that the element is enabled
			if ( !elemEnabled[e] ) continue;

			fVec2 p1 = nodePosition[ elemNodeId1[e] ];
			fVec2 p2 = nodePosition[ elemNodeId2[e] ];
			if ( fMath::IntersectCircleSegment( pos, rad, p1, p2 ) ) {
				elemMaterialId[e] = brushMaterialId;
			}
		}
	}
}



void fTrussMesh::StartMoveNodesInsideBrush( fCamera2 *camera ) {
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


void fTrussMesh::MoveNodesInsideBrush( fCamera2 *camera ) {
	QPoint	screen_pos	= app ->GetBrush( ) ->GetPosition( );
	fVec2	pos			= camera ->GetModelPositionAt( screen_pos.x( ), screen_pos.y( ) );
	fVec2	shift		= pos - prevBrushPos;

	for ( int i = 0; i < moveNodeIds.Num( ); i++ ) {
		int id = moveNodeIds[i];
		nodePosition[id] += shift * moveNodeFrac[i];

	}	
	prevBrushPos		= pos;
}


void fTrussMesh::RemoveNodesInsideBrush( fCamera2 *camera ) {
	QPoint	screen_pos	= app ->GetBrush( ) ->GetPosition( );
	fVec2	pos			= camera ->GetModelPositionAt( screen_pos.x( ), screen_pos.y( ) );
	float	rad			= camera ->GetModelSize( app ->GetBrush( ) ->GetDiameter( ) ) * 0.5f;


	float r_sq	= rad * rad;
	fVec2 c		= pos;
	// Find all nodes inside the brush
	for ( int i = 0; i < nodePosition.Num( ); i++ ) {
		// Make sure that the element is enabled
		if ( !nodeEnabled[i] ) {
			continue;
		}

		float d_sq	= ( pos - nodePosition[i] ).LengthSqr( );

		if ( d_sq > r_sq ) {
			continue;
		} else {
			for ( int e = 0; e < nodeElemIds[i].Num( ); e++ ) {
				int e_id = nodeElemIds[i][e];

				elemEnabled[e_id] = false;

				// Remove adjacent nodes if needed
				int id1 = elemNodeId1[e_id];
				int id2 = elemNodeId2[e_id];

				nodeElemNum[id1] = nodeElemNum[id1] - 1;
				nodeElemNum[id2] = nodeElemNum[id2] - 1;
				
				if ( nodeElemNum[id1] == 0 ) {
					//nodeElemIds[id1].Clear( );
					nodeEnabled[id1] = false;
				}
				if ( nodeElemNum[id2] == 0 ) {
					//nodeElemIds[id2].Clear( );
					nodeEnabled[id2] = false;
				}
			}
			nodeElemIds[i].Clear( );
			nodeEnabled[i] = false;
		}
	}

}


void fTrussMesh::RemoveElementsInsideBrush( fCamera2 *camera ) {
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

			fVec2 v1 = nodePosition[ elemNodeId1[e] ] - c;
			fVec2 v2 = nodePosition[ elemNodeId2[e] ] - c;
			if ( v1.LengthSqr( ) <= r_sq && v2.LengthSqr( ) <= r_sq ) {
				DisableElement( e );
			}
		}
	} else {
		fBounds2 brush_bounds( pos - fVec2( rad, rad ), pos +  fVec2( rad, rad ) );
		for ( int e = 0; e < elemMaterialId.Num( ); e++ ) {
			// Make sure that the element is enabled
			if ( !elemEnabled[e] ) continue;

			fVec2 p1 = nodePosition[ elemNodeId1[e] ];
			fVec2 p2 = nodePosition[ elemNodeId2[e] ];
			
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
				DisableElement( e );
			}
		}
	}
}


void fTrussMesh::SetNodeDisplacementForNodesInsideBrush( fCamera2 *camera ) {
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


void fTrussMesh::RemoveNodeDisplacementForNodesInsideBrush( fCamera2 *camera ) {
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


void fTrussMesh::SetNodeLoadForNodesInsideBrush( fCamera2 *camera ) {
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


void fTrussMesh::RemoveNodeLoadForNodesInsideBrush( fCamera2 *camera ) {
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



void fTrussMesh::DisableElement( int e ) {
	elemEnabled[e] = false;
	// Remove adjacent nodes if needed
	int id1 = elemNodeId1[e];
	int id2 = elemNodeId2[e];
	nodeElemIds[id1].Remove( e );
	nodeElemIds[id2].Remove( e );

	nodeElemNum[id1] = nodeElemNum[id1] - 1;
	nodeElemNum[id2] = nodeElemNum[id2] - 1;

	if ( nodeElemNum[id1] == 0 ) {
		nodeElemIds[id1].Clear( );
		nodeEnabled[id1] = false;
	}
	if ( nodeElemNum[id2] == 0 ) {
		nodeElemIds[id2].Clear( );
		nodeEnabled[id2] = false;
	}
}



void fTrussMesh::RemoveElementsWithMaterialId( int id ) {
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


void fTrussMesh::StopMoveNodesInsideBrush( fCamera2 *camera ) {
	moveNodeIds.Clear( );
	moveNodeFrac.Clear( );
}


fBounds2 fTrussMesh::GetBounds( ) {
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


void fTrussMesh::OnMousePress( QMouseEvent *event, fCamera2 *camera ) {
	switch ( editOperation ) {
		case TME_PAINT_ELEM_SET_MATERIAL: {
			if ( event ->buttons( ) & Qt::LeftButton ) {
				ApplyMaterialForElementsInsideBrush( camera );
			}
			break;
		}
		case TME_PAINT_NODE_MOVE: {
			if ( event ->buttons( ) & Qt::LeftButton ) {
				StartMoveNodesInsideBrush( camera );
			}
			break;
		}
		case TME_PAINT_NODE_REMOVE: {
			if ( event ->buttons( ) & Qt::LeftButton ) {
				RemoveNodesInsideBrush( camera );
			}
			break;
		}
		case TME_PAINT_ELEM_REMOVE: {
			if ( event ->buttons( ) & Qt::LeftButton ) { 
			RemoveElementsInsideBrush( camera );
			}
			break;
		}
		case TME_PAINT_NODE_ADD_REMOVE_BNDCOND: {
			if ( event ->buttons( ) & Qt::LeftButton ) {
				SetNodeDisplacementForNodesInsideBrush( camera );
			} else if ( event ->buttons( ) & Qt::RightButton ) {
				RemoveNodeDisplacementForNodesInsideBrush( camera );
			}
			break;
		}
		case TME_PAINT_NODE_ADD_REMOVE_LOAD: {
			if ( event ->buttons( ) & Qt::LeftButton ) {
				SetNodeLoadForNodesInsideBrush( camera );
			} else if ( event ->buttons( ) & Qt::RightButton ) {
				RemoveNodeLoadForNodesInsideBrush( camera );
			}
			break;
		}
	}
}


void fTrussMesh::OnMouseMove( QMouseEvent *event, fCamera2 *camera ) {
	switch ( editOperation ) {
		case TME_PAINT_ELEM_SET_MATERIAL: {
			if ( event ->buttons( ) & Qt::LeftButton ) {
				ApplyMaterialForElementsInsideBrush( camera );
			}
			break;
		}
		case TME_PAINT_NODE_MOVE: {
			if ( event ->buttons( ) & Qt::LeftButton ) {
				MoveNodesInsideBrush( camera );
			}
			break;
		}
		case TME_PAINT_NODE_REMOVE: {
			if ( event ->buttons( ) & Qt::LeftButton ) {
				RemoveNodesInsideBrush( camera );
			}
			break;
		}
		case TME_PAINT_ELEM_REMOVE: {
			if ( event ->buttons( ) & Qt::LeftButton ) {
				RemoveElementsInsideBrush( camera );
			}
			break;
		}
		case TME_PAINT_NODE_ADD_REMOVE_BNDCOND: {
			if ( event ->buttons( ) & Qt::LeftButton ) {
				SetNodeDisplacementForNodesInsideBrush( camera );
			} else if ( event ->buttons( ) & Qt::RightButton ) {
				RemoveNodeDisplacementForNodesInsideBrush( camera );
			}
			break;
		}
		case TME_PAINT_NODE_ADD_REMOVE_LOAD: {
			if ( event ->buttons( ) & Qt::LeftButton ) {
				SetNodeLoadForNodesInsideBrush( camera );
			} else if ( event ->buttons( ) & Qt::RightButton ) {
				RemoveNodeLoadForNodesInsideBrush( camera );
			}
			break;
		}						  
	}
}


void fTrussMesh::OnMouseRelease( QMouseEvent *event, fCamera2 *camera ) {
	switch ( editOperation ) {
		case TME_PAINT_ELEM_SET_MATERIAL: {
			break;
		}
		case TME_PAINT_NODE_MOVE: {
			break;
		}
		case TME_PAINT_NODE_REMOVE: {
			break;
		}
		case TME_PAINT_ELEM_REMOVE: {
			break;
		}
		case TME_PAINT_NODE_ADD_REMOVE_BNDCOND: {
			break;
		}
		case TME_PAINT_NODE_ADD_REMOVE_LOAD: {
			break;
		}
		
	}
}


void fTrussMesh::ReadSettings( ) {
	// Get application settings
	QSettings * settings = app ->GetSettings( );

	settings ->beginGroup( "truss_mesh" );
		elemLineWidth		= settings ->value( "element_line_width", 2 ).toInt( );
		nodeDispIconSize	= settings ->value( "displacement_icon_size", 11 ).toInt( );
		nodeDispLineWidth	= settings ->value( "displacement_line_width", 1 ).toInt( );
		nodeLoadIconSize	= settings ->value( "load_icon_size", 10 ).toInt( );

		nodeSize				= settings ->value( "node_size", 2 ).toInt( );
		nodeColor				= settings ->value( "node_color", QColor( 255, 255, 255 ) ).value<QColor>( );
		drawNodes				= settings ->value( "draw_nodes", true ).toBool( );

		defaultMat1Color	= settings ->value( "default_material1_color", Qt::blue ).value<QColor>( );
		defaultMat2Color	= settings ->value( "default_material2_color", QColor(0, 80, 0) ).value<QColor>( );
		defaultLoadColor	= settings ->value( "default_load_color", Qt::black ).value<QColor>( );
		defaultDispColor	= settings ->value( "default_displacement_color", QColor(140, 0, 0) ).value<QColor>( );
	settings ->endGroup( );
}


void fTrussMesh::WriteSettings( ) {
	QSettings * settings = app ->GetSettings( );

	settings ->beginGroup( "truss_mesh" );
		settings ->setValue( "element_line_width", elemLineWidth );
		settings ->setValue( "displacement_icon_size", nodeDispIconSize );
		settings ->setValue( "displacement_line_width", nodeDispLineWidth );
		settings ->setValue( "load_icon_size", nodeLoadIconSize );

		settings ->setValue( "node_size", nodeSize );
		settings ->setValue( "node_color", nodeColor );
		settings ->setValue( "draw_nodes", drawNodes );

		settings ->setValue( "default_material1_color", defaultMat1Color );
		settings ->setValue( "default_material2_color", defaultMat2Color );
		settings ->setValue( "default_load_color", defaultLoadColor );
		settings ->setValue( "default_displacement_color", defaultDispColor );

	settings ->endGroup( );
}


QString fTrussMesh::GetFileFilters( ) {
	return QString( "Matlab file (*.m);;Ansys mesh file (*.dat)" );
}


void fTrussMesh::Save( QString &file_name, QString &filter_name ) {
	int save_format = 0;
	int node_num	= nodeEnabled.Num( );
	int elem_num	= elemEnabled.Num( );
	QFileInfo fi( file_name );

	if ( filter_name == "Matlab file (*.m)" ) {
		save_format = 0;
		if ( fi.suffix( ) != "m" ) {
			file_name.append( ".m" );
		}
	} else if ( filter_name == "Ansys mesh file (*.dat)" ) {
		save_format = 1;
		if ( fi.suffix( ) != "dat" ) {
			file_name.append( ".dat" );
		}
	}


	/* =================================================
					Compiling the mesh
	================================================= */

	// Run through all enabled nodes
	fList<int>		node_new_id;
	fList<fVec2>	node_position;

	fList<int>		elem_node1_id;
	fList<int>		elem_node2_id;
	fList<int>		elem_mater_id;

	fList<int>		node_load_id;
	fList<int>		node_disp_id;
	fList<int>		node_load_node_id;
	fList<int>		node_disp_node_id;

	// Setup higher granularity
	int granularity = 1024 * 64;
	node_position.SetGranularity( granularity );
	node_load_id.SetGranularity( granularity );
	node_disp_id.SetGranularity( granularity );
	node_disp_node_id.SetGranularity( granularity );
	node_load_node_id.SetGranularity( granularity );
	// New id of the node in the list of old nodes
	node_new_id.SetNum( node_num );
	node_new_id.SetVal( -1 );

	elem_node1_id.SetGranularity( granularity );
	elem_node2_id.SetGranularity( granularity );
	elem_mater_id.SetGranularity( granularity );


	// Run through the nodes first
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

	// Run through the elements next
	int mat_id_max = 0;
	for ( int e = 0; e < elem_num; e++ ) {
		// 1) Check if the element is enabled
		if ( !elemEnabled[e] ) {
			continue;
		}
		// Append node with its' position and save its id
		int node_id1	= node_new_id[ elemNodeId1[e] ];
		int node_id2	= node_new_id[ elemNodeId2[e] ];
		int mat_id		= elemMaterialId[e];

		elem_node1_id.Append( node_id1 );
		elem_node2_id.Append( node_id2 );
		elem_mater_id.Append( mat_id );

		if ( mat_id > mat_id_max ) {
			mat_id_max = mat_id;
		}
	}
	int mat_num = mat_id_max + 1;

	// Sort elements per material
	fList<fList<int> > elem_per_mat_id;
	if ( save_format == 1 ) {
		elem_per_mat_id.SetNum( mat_num );
		for ( int i = 0; i < mat_num; i++ ) {
			elem_per_mat_id[i].SetGranularity( elem_num );
		}

		for ( int e = 0; e < elem_mater_id.Num( ); e++ ) {
			int mat_id = elem_mater_id[e];
			elem_per_mat_id[mat_id].Append( e );
		}
	}

	/* =================================================
						Reordering
	================================================= */
	
	


	/* =================================================
							Saving
	================================================= */
	// If Matlab save format
	if ( save_format == 0 ) {
		// Open the file
		FILE *file = fopen( file_name.toAscii( ).data( ), "w" );

		// Intro
		QString first_line = QString( "%% Created with:       " ) + app ->GetName( ) + QString( " " ) + app ->GetVersion( ) + QString( "\n" );
		fprintf( file, first_line.toAscii( ).data( ) );
		fprintf( file, "%% Element type:       truss\n" );
		fprintf( file, "%% Number of nodes:    %d\n", node_position.Num( ) );
		fprintf( file, "%% Number of elements: %d\n", elem_mater_id.Num( ) );
		fprintf( file, "\n" );
		fprintf( file, "clear all;\n\n", elem_mater_id.Num( ) );

		// Write nodes
		fprintf( file, "%% Node coordinates: x, y\n" );
		fprintf( file, "X = [\n" );
		for ( int i = 0; i < node_position.Num( ); i++ ) {
			fprintf( file, "%g\t%g\n", node_position[i].x, node_position[i].y );
		}
		fprintf( file, "];\n" );

		// Write elements
		fprintf( file, "%% Element connectivity: node1_id, node2_id, material_id\n" );
		fprintf( file, "IX = [\n" );
		for ( int e = 0; e < elem_mater_id.Num( ); e++ ) {
			fprintf( file, "%d\t%d\t%d\n", elem_node1_id[e] + 1, elem_node2_id[e] + 1, elem_mater_id[e] + 1 );
		}
		fprintf( file, "];\n" );


		// Write materials
		fprintf( file, "%% Element properties: Young's modulus, area\n" );
		fprintf( file, "mprop = [\n" );
		for ( int i = 0; i < materialList ->GetNum( ); i++ ) {
			float y = materialList ->GetMaterial( i ) ->GetYoungsModulus( );
			float a = materialList ->GetMaterial( i ) ->GetArea( );
			fprintf( file, "%g\t%g\n", y, a );
		}
		fprintf( file, "];\n" );

		// Write displacements
		fprintf( file, "%% Nodal diplacements: node_id, degree of freedom (1 - x, 2 - y), displacement\n" );
		fprintf( file, "bound = [\n" );
		for ( int i = 0; i < node_disp_id.Num( ); i++ ) {
			fNodeDisplacementType_e type	= nodeDisplacementList ->GetNodeDisplacement( node_disp_id[i] ) ->GetType( );
			float					dx		= nodeDisplacementList ->GetNodeDisplacement( node_disp_id[i] ) ->GetDx( );
			float					dy		= nodeDisplacementList ->GetNodeDisplacement( node_disp_id[i] ) ->GetDy( );

			switch ( type ) {
				case ND_X : {
					fprintf( file, "%d\t1\t%g\n", node_disp_node_id[i] + 1, dx );
					break;
				}
				case ND_Y : {
					fprintf( file, "%d\t2\t%g\n", node_disp_node_id[i] + 1, dy );
					break;
				}
				case ND_ALL : {
					fprintf( file, "%d\t1\t%g\n", node_disp_node_id[i] + 1, dx );
					fprintf( file, "%d\t2\t%g\n", node_disp_node_id[i] + 1, dy );
					break;
				}
			};
		}
		fprintf( file, "];\n" );


		// Write loads
		fprintf( file, "%% Nodal loads: node_id, degree of freedom (1 - x, 2 - y), load\n" );
		fprintf( file, "loads = [\n" );
		int last_dof = -1;
		for ( int i = 0; i < node_load_id.Num( ); i++ ) {
			float fx = nodeLoadList ->GetNodeLoad( node_load_id[i] ) ->GetFx( );
			float fy = nodeLoadList ->GetNodeLoad( node_load_id[i] ) ->GetFy( );

			if ( abs(fx) > FLT_MIN ) {
				fprintf( file, "%d\t1\t%g\n", node_load_node_id[i] + 1, fx );
				last_dof = -1;
			}
			if ( abs(fy) > FLT_MIN ) {
				fprintf( file, "%d\t2\t%g\n", node_load_node_id[i] + 1, fy );
				last_dof = 0;
			}
		}
		fprintf( file, "];\n" );


		// Write Control parameters
		fprintf( file, "%% Control parameters\n" );
		int id_last = node_load_id.Num( );
		fprintf( file, "plotdof = %d;\n", 2*id_last - last_dof );
		
		fclose( file );

	// Ansys save file format
	} else {
		// Open the file
		FILE *file = fopen( file_name.toAscii( ).data( ), "w" );

		// Intro
		fprintf( file, "/TITLE, TRUSS GROUND STRUCTURE\n" );
		QString first_line = QString( "! Created with:       " ) + app ->GetName( ) + QString( " " ) + app ->GetVersion( ) + QString( "\n" );
		fprintf( file, first_line.toAscii( ).data( ) );
		fprintf( file, "! Element type:       LINK1\n" );
		fprintf( file, "! Number of nodes:    %d\n", node_position.Num( ) );
		fprintf( file, "! Number of elements: %d\n\n", elem_mater_id.Num( ) );

		fprintf( file, "/PREP7\n" );
		fprintf( file, "! Element type: ET, type #, name\n" );
		fprintf( file, "ET, 1, LINK1\n" );

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
				fprintf( file, "MP,\tEX,  \t%d,\t%g\n", m + 1, materialList ->GetMaterial( m ) ->GetYoungsModulus( ) );
			}
			fprintf( file, "R, \t%d,\t%g\n", m + 1, materialList ->GetMaterial( m ) ->GetArea( ) ); // CLFE change

			fprintf( file, "! Connectivity\n" );
			for ( int e = 0; e < elem_per_mat_id[m].Num( ); e++ ) {
				int id = elem_per_mat_id[m][e];
				//fprintf( file, "EN,\t%d\t%d\t%d\n", id + 1, 
                                fprintf( file, "EN,\t%d,\t%d,\t%d\n", id + 1, 
					elem_node1_id[id] + 1, 
					elem_node2_id[id] + 1 );
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

		// 5) End of file
		fprintf( file, "\nFINISH\n" );

		fclose( file );
	}

}

