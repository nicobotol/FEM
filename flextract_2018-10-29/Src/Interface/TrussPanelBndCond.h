#ifndef __TRUSS_PNL_BNDCOND_H__
#define __TRUSS_PNL_BNDCOND_H__


#include <QWidget>
#include <QtGui/QtGui>
#include "Mesh/TrussMesh.h"


class QAbstractItemModel;
class fTrussMaterialList;
class fClickableLabel;



class fEditTrussNodeDispDialog : public QDialog {
	Q_OBJECT

public:
										fEditTrussNodeDispDialog( QString &name, const QColor &color, fNodeDisplacementType_e type, float dx, float dy, QWidget *parent = 0 ); // CLFE change
										~fEditTrussNodeDispDialog(  );

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
	// Truss mesh panel and its contents
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



class fEditTrussNodeLoadDialog : public QDialog {
	Q_OBJECT

public:
										fEditTrussNodeLoadDialog( QString &name, const QColor &color, float fx, float fy, QWidget *parent = 0 ); // CLFE change
										~fEditTrussNodeLoadDialog(  );

	QString								ReadName( );
	QColor								ReadColor( );
	float								ReadFx( );
	float								ReadFy( );

public slots:
	void								OnColorChangeClick( );

private:
	// Truss mesh panel and its contents
	fClickableLabel *					lColor;
	QColor								mColor;
	QLineEdit *							leName;
	QLineEdit *							leFx;
	QLineEdit *							leFy;
};





class fTrussNodeDispsPanel : public QWidget {
	Q_OBJECT

public:
										fTrussNodeDispsPanel( fNodeDisplacementList *disps = 0, QWidget *parent = 0 );
										~fTrussNodeDispsPanel(  );

	void								SetNodeDisplacementList( fNodeDisplacementList *disps );
	void								UpdateSelection( int id );
	int									GetSelectedDisplacementId( );

signals:
	void								DisplacementSelected( int );
	void								DisplacementUpdated( );

private slots:
	void								OnTrussNodeDispSelected( const QItemSelection &, const QItemSelection & );
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



class fTrussNodeLoadsPanel : public QWidget {
	Q_OBJECT

public:
										fTrussNodeLoadsPanel( fNodeLoadList *disps = 0, QWidget *parent = 0 );
										~fTrussNodeLoadsPanel(  );

	void								SetNodeLoadList( fNodeLoadList *loads );
	void								UpdateSelection( int id );
	int									GetSelectedLoadId( );

signals:
	void								LoadSelected( int );
	void								LoadUpdated( );

private slots:
	void								OnTrussNodeLoadSelected( const QItemSelection &, const QItemSelection & );
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


#endif//__TRUSS_PNL_BNDCOND_H__
