#ifndef UTIL_H
#define UTIL_H

#include <QSlider>
#include <QPixmap>
#include <QString>
#include <QColor>
#include <QPoint>
#include <QEvent>
#include <QList>
#include <QVariant>
#include <QtSignalTransition>

#include <iostream>
#include <algorithm>
using namespace std;


double sliderRealValue(const QSlider&);

void changeColors(QPixmap&, int, int);

QPoint boardToScene(int, int);

template<typename V>
bool findPos(int i, int j, const V& v)
{
    foreach(const QPoint& p, v)
        if (p.x() == i && p.y() == j)
            return true;
    
    return false;
}

template<typename V, typename T>
const QList<QVariant> toVariantList(const V& v)
{
    QList<QVariant> l;
    
    foreach (const T& x, v)
        l.push_back(QVariant(x));
    
    return l;
}

template<typename T>
const QList<T> fromVariantList(const QVariant& v)
{
    QList<T> l;
    
    foreach (const QVariant& x, v.toList())
        l.push_back(x.value<T>());
        
    return l;
}


template<typename T>
void debug(const QString& msg, const T& s) {
    cout << "[" << msg.toStdString() << "] " << s << endl;
}       

#endif // UTIL_H
