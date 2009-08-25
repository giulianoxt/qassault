#ifndef UTIL_H
#define UTIL_H

#include <QSlider>
#include <QPixmap>
#include <QString>
#include <QColor>
#include <QPoint>
#include <iostream>
#include <algorithm>
using namespace std;


double sliderRealValue(const QSlider&);

void changeColors(QPixmap&, int, int);

QPoint boardToScene(int, int);


template<typename T>
void debug(const QString& msg, const T& s) {
    cout << "[" << msg.toStdString() << "] " << s << endl;
}

#endif // UTIL_H
