#ifndef __CLICKABLE_LABEL_H__
#define __CLICKABLE_LABEL_H__


#include <QtGui/QtGui>


class fClickableLabel : public QLabel {
	Q_OBJECT
public:
							fClickableLabel( const QString & text = "", QWidget * parent = 0 );
							~fClickableLabel( ) { };
 
signals:
	void					clicked( );
 
protected:
	void					mousePressEvent ( QMouseEvent * event ) ;
 
};


#endif//__CLICKABLE_LABEL_H__