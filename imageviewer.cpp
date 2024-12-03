#include "imageviewer.h"
#include <QtWidgets>

ImageViewer::ImageViewer(QWidget *parent)
    : QGraphicsView(parent), scaleFactor(1.0) {
    setScene(&scene);
    setBackgroundBrush(Qt::lightGray);

    setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);

    setMinimumSize(300, 200);
}

void ImageViewer::addImage(const QImage &image) {
    QGraphicsPixmapItem *giPixmap = scene.addPixmap(QPixmap::fromImage(image));
    giPixmap->setTransformationMode(Qt::SmoothTransformation);

    QRectF boundingRect = scene.itemsBoundingRect();
    giPixmap->setPos(0, boundingRect.height());
}

void ImageViewer::scaleImages(double factor) {
    scaleFactor *= factor;
    scale(factor, factor);
}

void ImageViewer::keyPressEvent(QKeyEvent *evt) {
    if (evt->key() == Qt::Key_Equal || evt->key() == Qt::Key_Minus) {
        double factor = (evt->key() == Qt::Key_Equal) ? 1.2 : 0.8;
        scaleImages(factor);
    } else {
        QGraphicsView::keyPressEvent(evt);
    }
}

QGraphicsScene *ImageViewer::getScene() {
    return &scene;
}
