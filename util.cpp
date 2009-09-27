#include "util.h"
#include "globals.h"

int randInt(int n) {
    double scale = rand() / double(RAND_MAX);
    int r = scale * n;
    return r;
}

int bounded(int x, int a, int b) {
    return max(a, min(x, b));
}

double sliderRealValue(const QSlider& slider)
{
    return slider.value() / 10000.;
}

void changeColors(QPixmap& px, int h_delta, int s_delta)
{    
    QPixmap alpha = px.alphaChannel();
    QImage img = px.toImage();
    
    for (int x = 0; x < img.width(); ++x)
        for (int y = 0; y < img.height(); ++y) {
        QColor c = img.pixel(x, y);
        
        int h, s, v;
        c.getHsv(&h, &s, &v);
        c.setHsv((h + h_delta + 2*360) % 360, bounded(s + s_delta, 0, 255), v);
        
        img.setPixel(x, y, c.rgb());
    }
    
    px = QPixmap::fromImage(img);
    px.setAlphaChannel(alpha);
}

QPoint boardToScene(int i, int j)
{
    int squareStep = boardSquareSize + boardSquareDistance;
    int offset = -3 * squareStep;
    
    return QPoint(j * squareStep + offset, i * squareStep + offset);
}

ostream& operator<<(ostream& out, const QPoint& p)
{
    return out << "(" << p.x() << "," << p.y() << ")";
}
