#include <QtGui/QApplication>
#include "assaultmainwindow.h"
#include<iostream>
using namespace std;

int main(int argc, char *argv[])
{    
    QApplication a(argc, argv);
    
    AssaultMainWindow w;
    w.show();
    
    return a.exec();
}
