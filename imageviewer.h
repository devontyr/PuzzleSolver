#ifndef IMAGEVIEWER_H
#define IMAGEVIEWER_H

#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGraphicsPixmapItem>

class ImageViewer : public QGraphicsView
{
    Q_OBJECT

    QGraphicsScene scene;
    double scaleFactor;

public:
    explicit ImageViewer(QWidget *parent = nullptr);
    void addImage(const QImage &image);
    void scaleImages(double factor);
    QGraphicsScene *getScene();

protected:
    void keyPressEvent(QKeyEvent *evt) override;
};

#endif // IMAGEVIEWER_H
