#ifndef PIECE_H
#define PIECE_H

#include <QGraphicsPixmapItem>


class PieceItem : public QObject, public QGraphicsPixmapItem
{
    Q_OBJECT
    
public:
    PieceItem(const QPixmap&, int, int);
    
    void move(int, int);

signals:
    void clicked(int, int);

private:   
    void mousePressEvent(QGraphicsSceneMouseEvent*);    
    void setPixmap(const QPixmap&);
    
    int i, j;
};

#endif // PIECE_H
