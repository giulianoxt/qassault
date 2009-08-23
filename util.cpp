#include "util.h"
#include "globals.h"


double sliderRealValue(const QSlider& slider)
{
    return slider.value() / 10000.;
}

void changeHue(QPixmap& px, int h_delta)
{    
    QPixmap alpha = px.alphaChannel();
    QImage img = px.toImage();
    
    for (int x = 0; x < img.width(); ++x)
        for (int y = 0; y < img.height(); ++y) {
        QColor c = img.pixel(x, y);
        
        int h, s, v;
        c.getHsv(&h, &s, &v);
        c.setHsv((h + h_delta + 2*360) % 360, s, v);
        
        img.setPixel(x, y, c.rgb());
    }
    
    px = QPixmap::fromImage(img);
    px.setAlphaChannel(alpha);
}

QPoint boardToScene(int i, int j)
{
    int squareStep = boardSquareSize + boardSquareDistance;
    int offset = -3 * squareStep;
    
    return QPoint(i * squareStep + offset, j * squareStep + offset);
}
