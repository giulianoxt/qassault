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

#include <cstdlib>
#include <iostream>
#include <algorithm>
#define for_(i,a,b) for (int i = a; i < b; ++i)
using namespace std;


int randInt(int, int);

double sliderRealValue(const QSlider&);

void changeColors(QPixmap&, int, int);

QPoint boardToScene(int, int);

ostream& operator<<(ostream&, const QPoint&);

template<typename T>
ostream& operator<<(ostream& out, const QList<T>& l)
{
    out << '[';
    
    bool f = true;
    foreach(const T& x, l) {
        if (f) f = false;
        else out << ", ";
        out << x;
    }
    
    return out << ']';
}

template<typename V, typename T>
const QList<QVariant> toVariantList(const V& v)
{
    QList<QVariant> l;
    
    foreach (const T& x, v) {
        QVariant v;
        v.setValue(x);
        l.push_back(v);
    }
    
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
