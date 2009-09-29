#include <QMetaType>
#include <QApplication>
#include <cstdlib>
#include <ctime>
#include "game.h"
#include "assaultmainwindow.h"
#include "minimax.h"
#include <QPoint>
#include <fstream>
using namespace std;

int main(int argc, char *argv[])
{            
    srand(time(0));
    
    QApplication a(argc, argv);     

    qRegisterMetaType<Move>("Move");

    AssaultMainWindow w;
    w.show();
    
    a.setQuitOnLastWindowClosed(true);
    return a.exec();
}
