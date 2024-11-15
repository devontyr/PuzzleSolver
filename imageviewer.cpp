#include "imageviewer.h"
#include <QtWidgets>

ImageViewer::ImageViewer(const QImage &_image):image(_image), scale(1.0) {
    setScene(&scene);
    giPixmap = scene.addPixmap(QPixmap::fromImage(image));

    setMinimumSize(300, 200);

    setBackgroundBrush(Qt::lightGray);
    setSceneRect(QRectF(QPointF(0, 0), image.size()));
    setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);
    grabKeyboard();
}

void ImageViewer::keyPressEvent(QKeyEvent *evt) {
    if (evt->key()==Qt::Key_Equal || evt->key()==Qt::Key_Minus) {
        scale *= (evt->key()==Qt::Key_Equal) ? 2.0 : 0.5;
        giPixmap->setScale(scale);
        setSceneRect(QRectF(QPointF(0,0), image.size()*scale));
    } else
        QGraphicsView::keyPressEvent(evt);
}
