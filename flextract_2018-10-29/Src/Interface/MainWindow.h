#ifndef __WNDMAIN_H__
#define __WNDMAIN_H__


#include <QtGui/QtGui>


class fCanvas;
class fBrush;

class fTrussMesh;
class fTrussMaterialsPanel;
class fTrussNodeDispsPanel;
class fTrussNodeLoadsPanel;

class fQuardMesh;
class fQuardMaterialsPanel;
class fQuardNodeDispsPanel;
class fQuardNodeLoadsPanel;
class fQuardSurfLoadsPanel;




struct fTrussEditActions {
	QActionGroup *					group;

	QAction *						selection;
	QAction *						paintElemSetMaterial;
	QAction *						paintNodeSetDisplacements;
	QAction *						paintNodeSetForces;
	QAction *						paintElemRemove;
	QAction *						paintNodeRemove;
	QAction *						paintNodeMove;

	QAction *						removeElementsWithMaterial;
};

struct fQuardEditActions {
	QActionGroup *					group;

	QAction *						selection;
	QAction *						paintElemSetMaterial;
	QAction *						paintNodeSetDisplacements;
	QAction *						paintNodeSetForces;
	QAction *						paintElemRemove;
	QAction *						paintNodeMove;
	QAction *						paintFaceSetForces;

	QAction *						removeElementsWithMaterial;
};






class fMainWindow : public QMainWindow {
    Q_OBJECT
public:
									fMainWindow( QWidget *parent = 0 );
									~fMainWindow( );

	fCanvas *						GetCanvas( )							{ return canvas; };


private slots:
	void 							OnNewMesh( );
	void 							OnSaveMesh( );
	void 							OnExit( );

	void							OnCenterMesh( );

	void							OnAbout( );

	void							OnTrussEditSelection( );
	void							OnTrussEditSetElementMaterials( );
	void							OnTrussEditSetNodalDiplacements( );
	void							OnTrussEditSetNodalForces( );
	void							OnTrussEditRemoveElements( );
	void							OnTrussEditRemoveElementsWithSpecificMaterial( );
	void							OnTrussEditRemoveNodes( );
	void							OnTrussEditMoveNodes( );
	void							OnTrussEditChange( QAction * );

	void							OnQuardEditSelection( );
	void							OnQuardEditSetElementMaterials( );
	void							OnQuardEditSetNodalDiplacements( );
	void							OnQuardEditSetNodalForces( );
	void							OnQuardEditSetFaceForces( );
	void							OnQuardEditRemoveElements( );
	void							OnQuardEditRemoveElementsWithSpecificMaterial( );
	void							OnQuardEditMoveNodes( );
	void							OnQuardEditChange( QAction * );

	void							OnTrussMaterialSelected( int );
	void							OnTrussNodeDispSelected( int );
	void							OnTrussNodeLoadSelected( int );

	void							OnQuardMaterialSelected( int );
	void							OnQuardNodeDispSelected( int );
	void							OnQuardNodeLoadSelected( int );
	void							OnQuardFaceLoadSelected( int );


	// Brush related
	void							OnBrushSizeChange( int );
	void							OnBrushHardnessChange( int );
	void							OnInclusiveStateChange( bool );

protected:
	virtual void					closeEvent( QCloseEvent * event );

private:
	void							CreateActions( );
	void							CreateMenus( );
	void							CreateToolBars( );

	void							AdjustMenus( );
	void							AdjustFileMenu( );
	void							AdjustEditMenu( );
	void							AdjustViewMenu( );
	void							AdjustWindowMenu( );
	void							AdjustHelpMenu( );

	void							AdjustToolBars( );

	void							RemoveMeshPanels( );
	void							CreateTrussPanels( fTrussMesh *mesh );
	void							CreateQuardPanels( fQuardMesh *mesh );

	void							ReadSettings( );
	void							WriteSettings( );

	// Main components of the interface
	fCanvas *						canvas;

	// Panels (windows)
	// Navigator
	QWidget	*						navigator;
	QDockWidget *					dwNavigator;

	// Truss control panels
	fTrussMaterialsPanel *			tmpTrussMaterials;
	fTrussNodeDispsPanel *			tndpNodeDisps;
	fTrussNodeLoadsPanel *			tnlpNodeLoads;
	QDockWidget *					dwTrussMaterials;
	QDockWidget *					dwTrussNodeDisps;
	QDockWidget *					dwTrussNodeLoads;

	// Quard control panels
	fQuardMaterialsPanel *			qmpQuardMaterials;
	fQuardNodeDispsPanel *			qndpNodeDisps;
	fQuardNodeLoadsPanel *			qnlpNodeLoads;
	fQuardSurfLoadsPanel *			qflpFaceLoads;
	QDockWidget *					dwQuardMaterials;
	QDockWidget *					dwQuardNodeDisps;
	QDockWidget *					dwQuardNodeLoads;
	QDockWidget *					dwQuardFaceLoads;
	

	// Menus
	QMenu *							menuFile;
	QMenu *							menuEdit;
	QMenu *							menuView;
	QMenu *							menuWindow;
	QMenu *							menuHelp;

	// Menu actions
	QAction *						actNewMesh;
	QAction *						actSaveMesh;
	QAction *						actExit;

	fTrussEditActions				actTrussEdit;
	fQuardEditActions				actQuardEdit;

	QAction *						actCenterMesh;

	QAction *						actAbout;

	// Status bar widgets
	QLabel *						lElemNum;
	QLabel *						lNodeNum;
	QLabel *						lTotalVol;

	
	// Brush ToolBar
	QToolBar *						tbBrush;
	QSpinBox *						sbBrushSize;
	QSlider *						sBrushHardness;
	QLabel *						lBrushHardness;
	QCheckBox *						cbInclusive;


	QByteArray						trussMeshDockState;
	QByteArray						quardMeshDockState;
	QString							saveFilePath;
};

#endif//__WNDMAIN_H__
