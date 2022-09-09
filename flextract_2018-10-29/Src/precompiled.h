#ifndef __PRECOMPILED_H__
#define __PRECOMPILED_H__


// Common headers
#include <float.h>
#include <malloc.h>
#include <stdlib.h>
#include <memory.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <assert.h>
#include <time.h>
#include <ctype.h>
#include <typeinfo>
#include <errno.h>
#include <math.h>

#include <GL/glu.h> // CLFE change

// Qt library headers
#include <QObject>
#include <QtGui/QtGui>
#include <QtGlobal>
#include <QTreeWidget>
#include <QtOpenGL/QGLWidget> // CLFE change
#include <QtXml/QtXml>
#include <QTime>


// Base classes and functions
#include "Base/BinSearch.h"
#include "Base/List.h"
#include "Base/Vector.h"
#include "Base/Bounds.h"


#include "Math/Math.h"


// Mesh types
#include "Mesh/Mesh.h"
#include "Mesh/TrussMesh.h"
#include "Mesh/QuardMesh.h"


// Interface elements]
#include "Interface/Brush.h"
#include "Interface/ClickableLabel.h"
#include "Interface/TrussPanelMaterial.h"
#include "Interface/TrussPanelBndCond.h"
#include "Interface/QuardPanelMaterial.h"
#include "Interface/QuardPanelBndCond.h"
#include "Interface/DlgNewMesh.h"
#include "Interface/Camera.h"
#include "Interface/Canvas.h"
#include "Interface/MainWindow.h"


// Application class
#include "App.h"





#endif//__PRECOMPILED_H__
