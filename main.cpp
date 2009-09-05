#include <QMetaType>
#include <QApplication>
#include "game.h"
#include "assaultmainwindow.h"


int main(int argc, char *argv[])
{            
    QApplication a(argc, argv);     

    qRegisterMetaType<Move>("Move");
    
    AssaultMainWindow w;
    w.show();
    
    a.setQuitOnLastWindowClosed(true);
    return a.exec();
}
