#ifndef UTIL_H
#define UTIL_H

#include <QSlider>
#include <QPixmap>
#include <QString>
#include <QColor>
#include <iostream>
#include <algorithm>
using namespace std;


double sliderRealValue(const QSlider&);

void changeHue(QPixmap&, int);

template<typename T>
void debug(const QString& msg, const T& s) {
    cout << "[" << msg.toStdString() << "] " << s << endl;
}

#endif // UTIL_H
