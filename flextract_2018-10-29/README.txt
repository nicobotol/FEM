2018-10-29
CLFE


Minimal instructions for building FlExtract for Linux and Windows.
Generation of the *.pro file needs to be done only once (and
whenever new source files are added to the project). Following
the steps below one could create a *.pro file for Linux and another
one for Windows. Extra customization could be added to the *.pro
file(s), such as specifying name of executable or optimization level.
Use google "qmake <something...>".


Compile on Linux:

1)
Navigate to the FlExtract top level directory

2)
Create a qmake project file:
    qmake -project

3)
Open the project file <my_project>.pro in a text editor and add the following lines at the end of the document:
    QT += opengl
    LIBS += -lGL -lGLU
    DESTDIR = release_linux
    OBJECTS_DIR = release_linux
    MOC_DIR = release_linux
    RCC_DIR = release_linux
    UI_DIR = release_linux

4)
Create a Makefile:
    qmake <my_project>.pro

5)
Run make:
    make

6)
Look for executable in "release_linux"



Cross-compile on Linux (Ubuntu) for Windows:

1)
Install mxe cross compiler
    a) Move into the folder there you want mxe installed
    b) git clone https://github.com/mxe/mxe.git 
    c) cd into the dir where you want to install
    d) Fetch files by typing: git clone https://github.com/mxe/mxe.git
    e) Install all requirements mentioned on (read carefully): mxe.cc/#requirements-debian
    f) Install also (if missing, target lua could fail): libreadline6 (and perhaps -dev and/or others related)
    g) cd into mxe
    h) Type: make gcc qt

2)
Navigate to the FlExtract top level directory

3)
Create a qmake project file
    <where MXE is installed>/usr/i686-w64-mingw32.static/qt/bin/qmake -project

4)
Open the project file <my_project>.pro in a text editor and add the following lines at the end of the document:
    QT += opengl
    DESTDIR = release_windows
    OBJECTS_DIR = release_windows
    MOC_DIR = release_windows
    RCC_DIR = release_windows
    UI_DIR = release_windows

Yes,  _do_not_ add "LIBS += -lGL -lGLU" for cross-compiling with mxe :)

5)
Create a Makefile:
    <where MXE is installed>/usr/i686-w64-mingw32.static/qt/bin/qmake <my_project>.pro

6)
Run make:
    PATH=<where MXE is installed>/usr/i686-w64-mingw32.static/qt/bin:$PATH make

7) 
Look for executable in "release_windows"
    
