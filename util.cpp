#include "util.h"


double sliderRealValue(const QSlider& slider)
{
    return slider.value() / double(slider.maximum());
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
