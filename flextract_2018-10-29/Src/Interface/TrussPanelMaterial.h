#ifndef __TRUSS_PNL_MATERIAL_H__
#define __TRUSS_PNL_MATERIAL_H__


#include <QWidget>
#include <QtGui/QtGui>


class QAbstractItemModel;
class fTrussMaterialList;
class fClickableLabel;




class fRemoveTrussElementsWithMaterialDialog : public QDialog {
	Q_OBJECT

public:
										fRemoveTrussElementsWithMaterialDialog( fTrussMaterialList *materials, QWidget *parent = 0 );
										~fRemoveTrussElementsWithMaterialDialog(  );

	int									ReadMaterialId( );

public slots:
	void								OnMaterialDoubleClick( const QModelIndex & );

private:
	QListView *							lvMaterials;
};




class fEditTrussMaterialDialog : public QDialog {
	Q_OBJECT

public:
										fEditTrussMaterialDialog( QString &name, const QColor &color, float area, float youngs, QWidget *parent = 0 ); // CLFE change
										~fEditTrussMaterialDialog(  );

	QString								ReadName( );
	QColor								ReadColor( );
	float								ReadArea( );
	float								ReadYoungsModulus( );

public slots:
	void								OnColorChangeClick( );

private:
	// Truss mesh panel and its contents
	fClickableLabel *					lColor;
	QColor								mColor;
	QLineEdit *							leName;
	QLineEdit *							leArea;
	QLineEdit *							leYoungs;
};





class fTrussMaterialsPanel : public QWidget {
	Q_OBJECT

public:
										fTrussMaterialsPanel( fTrussMaterialList *materials = 0, QWidget *parent = 0 );
										~fTrussMaterialsPanel(  );

	void								SetMaterialList( fTrussMaterialList *mat );
	void								UpdateSelection( int id );
	int									GetSelectedMaterialId( );

signals:
	void								MaterialSelected( int );
	void								MaterialUpdated( );

private slots:
	void								OnTrussMaterialSelected( const QItemSelection &, const QItemSelection & );
	void								OnRowsAboutToBeInserted( const QModelIndex &, int, int );
	void								OnRowsAboutToBeRemoved( const QModelIndex &, int, int );

	void								OnAreaEdited( const QString & );
	void								OnYoungsEdited( const QString & );
	void								OnColorChanged( QColor & );
	void								OnNameChanged( QString & );

	void								OnRemoveMaterial( );
	void								OnAddMaterial( );
	void								OnEditMaterial( const QModelIndex & index );

private:
	fTrussMaterialList *				materials;

	QListView *							lvMaterials;
	QPushButton *						btnAddMaterial;
	QPushButton *						btnRemoveMaterial;

	QLineEdit *							leArea;
	QLineEdit *							leYoungsModulus;
};


#endif//__TRUSS_PNL_MATERIAL_H__
