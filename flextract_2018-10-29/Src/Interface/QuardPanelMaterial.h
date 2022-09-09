#ifndef __QUARD_PNL_MATERIAL_H__
#define __QUARD_PNL_MATERIAL_H__


#include <QWidget>
#include <QtGui/QtGui>


class QAbstractItemModel;
class fQuardMaterialList;
class fClickableLabel;




class fRemoveQuardElementsWithMaterialDialog : public QDialog {
	Q_OBJECT

public:
										fRemoveQuardElementsWithMaterialDialog( fQuardMaterialList *materials, QWidget *parent = 0 );
										~fRemoveQuardElementsWithMaterialDialog(  );

	int									ReadMaterialId( );

public slots:
	void								OnMaterialDoubleClick( const QModelIndex & );

private:
	QListView *							lvMaterials;
};




class fEditQuardMaterialDialog : public QDialog {
	Q_OBJECT

public:
										fEditQuardMaterialDialog( QString &name, const QColor &color, float thickness, float ex, float poisson,
											float dens, bool isotropic = true, float ey = 0.0f, float gxy = 0.0f, QWidget *parent = 0 ); // CLFE change
										~fEditQuardMaterialDialog(  );

	QString								ReadName( );
	QColor								ReadColor( );
	float								ReadThickness( );
	float								ReadEx( );
	float								ReadEy( );
	float								ReadGxy( );
	float								ReadPoisson( );
	float								ReadDens( );
	bool								ReadIsotropicFlag( );

public slots:
	void								OnColorChangeClick( );

private slots:
	void								OnRBIsotropicClick( bool );
	void								OnRBAnisotropicClick( bool );

private:
	// Quard mesh panel and its contents
	fClickableLabel *					lColor;
	QColor								mColor;
	QLineEdit *							leName;

	QLabel *							lEx;
	QLabel *							lEy;
	QLabel *							lGxy;
	QLabel *							lPoisson;
	QLabel *							lDens;
	QLabel *							lThickness;

	QLineEdit *							leEx;
	QLineEdit *							leEy;
	QLineEdit *							leGxy;
	QLineEdit *							lePoisson;
	QLineEdit *							leDens;
	QLineEdit *							leThickness;

	QRadioButton *						rbIsotropic;
	QRadioButton *						rbAnisotropic;
};





class fQuardMaterialsPanel : public QWidget {
	Q_OBJECT

public:
										fQuardMaterialsPanel( fQuardMaterialList *materials = 0, QWidget *parent = 0 );
										~fQuardMaterialsPanel(  );

	void								SetMaterialList( fQuardMaterialList *mat );
	void								UpdateSelection( int id );
	int									GetSelectedMaterialId( );

signals:
	void								MaterialSelected( int );
	void								MaterialUpdated( );

private slots:
	void								OnQuardMaterialSelected( const QItemSelection &, const QItemSelection & );
	void								OnRowsAboutToBeInserted( const QModelIndex &, int, int );
	void								OnRowsAboutToBeRemoved( const QModelIndex &, int, int );

	void								OnColorChanged( QColor & );
	void								OnNameChanged( QString & );

	void								OnThicknessEdited( const QString & );
	void								OnExEdited( const QString & );
	void								OnEyEdited( const QString & );
	void								OnGxyEdited( const QString & );
	void								OnPoissonEdited( const QString & );
	void								OnDensEdited( const QString & );

	void								OnRBIsotropicClick( bool );
	void								OnRBAnisotropicClick( bool );

	void								OnRemoveMaterial( );
	void								OnAddMaterial( );
	void								OnEditMaterial( const QModelIndex & index );

private:
	fQuardMaterialList *				materials;

	QListView *							lvMaterials;
	QPushButton *						btnAddMaterial;
	QPushButton *						btnRemoveMaterial;

	QLabel *							lEx;
	QLabel *							lEy;
	QLabel *							lGxy;
	QLabel *							lPoisson;
	QLabel *							lDens;
	QLabel *							lThickness;

	QLineEdit *							leEx;
	QLineEdit *							leEy;
	QLineEdit *							leGxy;
	QLineEdit *							lePoisson;
	QLineEdit *							leDens;
	QLineEdit *							leThickness;

	QRadioButton *						rbIsotropic;
	QRadioButton *						rbAnisotropic;
};


#endif//__QUARD_PNL_MATERIAL_H__
