#include "../precompiled.h"


fClickableLabel::fClickableLabel( const QString & text, QWidget * parent ) : QLabel( parent ) {
	
}
 
void fClickableLabel::mousePressEvent( QMouseEvent * event ) {
	emit clicked( );
}