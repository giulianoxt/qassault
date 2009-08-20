#include <QtGui/QApplication>
#include "assaultmainwindow.h"


int main(int argc, char *argv[])
{            
    QApplication a(argc, argv);     
    
    AssaultMainWindow w;
    w.show();
    
    a.setQuitOnLastWindowClosed(true);
    return a.exec();
}
