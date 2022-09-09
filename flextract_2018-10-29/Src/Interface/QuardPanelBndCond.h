#ifndef __QUARD_PNL_BNDCOND_H__
#define __QUARD_PNL_BNDCOND_H__


#include <QWidget>
#include <QtGui/QtGui>
#include "Mesh/QuardMesh.h"


class QAbstractItemModel;
class fQuardMaterialList;
class fQuardNodeDispList;
class fQuardNodeLoadList;
//class fQuardSurfLoadList;
class fClickableLabel;



class fEditQuardNodeDispDialog : public QDialog {
	Q_OBJECT

public:
										fEditQuardNodeDispDialog( QString &name, const QColor &color, fNodeDisplacementType_e type, float dx, float dy, QWidget *parent = 0 ); 
										~fEditQuardNodeDispDialog(  );

	QString								ReadName( );
	QColor								ReadColor( );
	fNodeDisplacementType_e				ReadType( );
	float								ReadDx( );
	float								ReadDy( );

public slots:
	void								OnColorChangeClick( );
	void								OnRBDispXClick( bool );
	void								OnRBDispYClick( bool );
	void								OnRBDispAClick( bool );

private:
	// Quard mesh panel and its contents
	fClickableLabel *					lColor;
	QColor								mColor;
	QLineEdit *							leName;
	QLineEdit *							leDx;
	QLineEdit *							leDy;

	QLabel *							lDx;
	QLabel *							lDy;

	QRadioButton *						rbDispX;
	QRadioButton *						rbDispY;
	QRadioButton *						rbDispA;
};



class fEditQuardNodeLoadDialog : public QDialog {
	Q_OBJECT

public:
										fEditQuardNodeLoadDialog( QString &name, const QColor &color, float fx, float fy, QWidget *parent = 0 );
										~fEditQuardNodeLoadDialog(  );

	QString								ReadName( );
	QColor								ReadColor( );
	float								ReadFx( );
	float								ReadFy( );

public slots:
	void								OnColorChangeClick( );

private:
	// Quard mesh panel and its contents
	fClickableLabel *					lColor;
	QColor								mColor;
	QLineEdit *							leName;
	QLineEdit *							leFx;
	QLineEdit *							leFy;
};



class fEditQuardSurfLoadDialog : public QDialog {
	Q_OBJECT

public:
										fEditQuardSurfLoadDialog( QString &name, const QColor &color, float val, QWidget *parent = 0 );
										~fEditQuardSurfLoadDialog(  );

	QString								ReadName( );
	QColor								ReadColor( );
	float								ReadVal( );

public slots:
	void								OnColorChangeClick( );

private:
	// Quard mesh panel and its contents
	fClickableLabel *					lColor;
	QColor								mColor;
	QLineEdit *							leName;
	QLineEdit *							leVal;
};





class fQuardNodeDispsPanel : public QWidget {
	Q_OBJECT

public:
										fQuardNodeDispsPanel( fNodeDisplacementList *disps = 0, QWidget *parent = 0 );
										~fQuardNodeDispsPanel(  );

	void								SetNodeDisplacementList( fNodeDisplacementList *disps );
	void								UpdateSelection( int id );
	int									GetSelectedDisplacementId( );

signals:
	void								DisplacementSelected( int );
	void								DisplacementUpdated( );

private slots:
	void								OnQuardNodeDispSelected( const QItemSelection &, const QItemSelection & );
	void								OnRowsAboutToBeInserted( const QModelIndex &, int, int );
	void								OnRowsAboutToBeRemoved( const QModelIndex &, int, int );

	void								OnDxEdited( const QString & );
	void								OnDyEdited( const QString & );
	void								OnRBDispXClick( bool );
	void								OnRBDispYClick( bool );
	void								OnRBDispAClick( bool );

	void								OnRemoveDisplacement( );
	void								OnAddDisplacement( );
	void								OnEditDisplacement( const QModelIndex & index );

private:
	fNodeDisplacementList *				displacements;

	QListView *							lvDisplacements;
	QPushButton *						btnAddDisplacement;
	QPushButton *						btnRemoveDisplacement;

	QLineEdit *							leDx;
	QLineEdit *							leDy;

	QLabel *							lDx;
	QLabel *							lDy;

	QRadioButton *						rbDispX;
	QRadioButton *						rbDispY;
	QRadioButton *						rbDispA;
};



class fQuardNodeLoadsPanel : public QWidget {
	Q_OBJECT

public:
										fQuardNodeLoadsPanel( fNodeLoadList *loads = 0, QWidget *parent = 0 );
										~fQuardNodeLoadsPanel(  );

	void								SetNodeLoadList( fNodeLoadList *loads );
	void								UpdateSelection( int id );
	int									GetSelectedLoadId( );

signals:
	void								LoadSelected( int );
	void								LoadUpdated( );

private slots:
	void								OnQuardNodeLoadSelected( const QItemSelection &, const QItemSelection & );
	void								OnRowsAboutToBeInserted( const QModelIndex &, int, int );
	void								OnRowsAboutToBeRemoved( const QModelIndex &, int, int );

	void								OnFxEdited( const QString & );
	void								OnFyEdited( const QString & );

	void								OnRemoveLoad( );
	void								OnAddLoad( );
	void								OnEditLoad( const QModelIndex & index );

private:
	fNodeLoadList *						loads;

	QListView *							lvLoads;
	QPushButton *						btnAddLoad;
	QPushButton *						btnRemoveLoad;

	QLineEdit *							leFx;
	QLineEdit *							leFy;
};




class fQuardSurfLoadsPanel : public QWidget {
	Q_OBJECT

public:
										fQuardSurfLoadsPanel( fSurfLoadList *loads = 0, QWidget *parent = 0 );
										~fQuardSurfLoadsPanel(  );

	void								SetSurfLoadList( fSurfLoadList *loads );
	void								UpdateSelection( int id );
	int									GetSelectedLoadId( );

signals:
	void								LoadSelected( int );
	void								LoadUpdated( );

private slots:
	void								OnQuardSurfLoadSelected( const QItemSelection &, const QItemSelection & );
	void								OnRowsAboutToBeInserted( const QModelIndex &, int, int );
	void								OnRowsAboutToBeRemoved( const QModelIndex &, int, int );

	void								OnValEdited( const QString & );

	void								OnRemoveLoad( );
	void								OnAddLoad( );
	void								OnEditLoad( const QModelIndex & index );

private:
	fSurfLoadList *						loads;

	QListView *							lvLoads;
	QPushButton *						btnAddLoad;
	QPushButton *						btnRemoveLoad;

	QLineEdit *							leVal;
};





#endif//__QUARD_PNL_BNDCOND_H__
