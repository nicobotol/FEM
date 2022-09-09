#ifndef __DLG_NEW_MESH_H__
#define __DLG_NEW_MESH_H__


#include <QDialog>
#include "../Mesh/Mesh.h"
#include "../Mesh/TrussMesh.h"


class QGroupBox;
class QStackedWidget;
class QButtonGroup;
class QLineEdit;
class QSpinBox;
class QComboBox;
class QDialogButtonBox;



class fNewMeshDialog : public QDialog {
	Q_OBJECT

public:
										fNewMeshDialog( QWidget *parent = 0 );
										~fNewMeshDialog(  );

	fMeshType_e							ReadMeshType( );

	double								ReadTrussMeshSizeX( );
	double								ReadTrussMeshSizeY( );
	int									ReadTrussMeshNodeNumX( );
	int									ReadTrussMeshNodeNumY( );
	int									ReadTrussMeshConnectivitySize( );
	fTrussMeshConnectivity_e			ReadTrussMeshConnectivity( );

	double								ReadQuardMeshSizeX( );
	double								ReadQuardMeshSizeY( );
	int									ReadQuardMeshElemNumX( );
	int									ReadQuardMeshElemNumY( );
	

public slots:
	void								ChangeMeshTypePanel( int id );
	void								OnConnectivityChanged( int );

private:
	QGroupBox *							CreateMeshTypeGroupBox( );
	QWidget *							CreateTrussMeshPanel( );
	QWidget *							CreateContinuumMeshPanel( );

	void								ReadSettings( );
	void								WriteSettings( );

	QButtonGroup *						bgMeshType;
	QGroupBox *							gbMeshType;
	QStackedWidget *					swMeshDetails;

	QDialogButtonBox *					dbbConfirmation;

	// Truss mesh panel and its contents
	QWidget *							wTrussMeshPanel;
	QLineEdit *							leTrussMeshSizeX;
	QLineEdit *							leTrussMeshSizeY;
	QSpinBox *							sbTrussMeshNodeNumX;
	QSpinBox *							sbTrussMeshNodeNumY;

	QComboBox *							cbTrussMeshConnecType;
	QLabel *							lConSize;
	QSpinBox *							sbAllConnectSize;


	// Continuum mesh and its components
	QWidget *							wQuardMeshPanel;
	QLineEdit *							leQuardMeshSizeX;
	QLineEdit *							leQuardMeshSizeY;
	QSpinBox *							sbQuardMeshElemNumX;
	QSpinBox *							sbQuardMeshElemNumY;
};


#endif//__DLG_NEW_MESH_H__