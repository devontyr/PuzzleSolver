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
    if (evt->key()==Qt::Key_Plus || evt->key()==Qt::Key_Minus) {
        scale *= (evt->key()==Qt::Key_Plus) ? 2.0 : 0.5;
        giPixmap->setScale(scale);
        setSceneRect(QRectF(QPointF(0,0), image.size()*scale));
    } else
        QGraphicsView::keyPressEvent(evt);
}

void ImageViewer::mouseMoveEvent(QMouseEvent *evt) {
    // qDebug() << evt->pos();
    // qDebug() << mapToScene(evt->pos());
    // qDebug() << giPixmap->mapFromScene(mapToScene(evt->pos()));
    // qDebug() << (giPixmap->mapFromScene(mapToScene(evt->pos()))-QPointF(0.5, 0.5)).toPoint();
    // qDebug() << "";

    QPoint pos = (giPixmap->mapFromScene(mapToScene(evt->pos()))-QPointF(0.5, 0.5)).toPoint();

    if (image.valid(pos))
        emit mouseMoved(pos, image.pixelColor(pos));
}
