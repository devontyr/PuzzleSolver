#ifndef INTERACTIVEPIECE_H
#define INTERACTIVEPIECE_H

#include <QGraphicsView>
#include <QImage>
#include <QtWidgets>

class interactivePiece : public QGraphicsView {
    Q_OBJECT

public:
    explicit interactivePiece(const QList<QImage> &pieces, QWidget *parent = nullptr);

protected:
    void keyPressEvent(QKeyEvent *evt) override;
    void mousePressEvent(QMouseEvent *evt) override;
    void mouseReleaseEvent(QMouseEvent *evt) override;

private:
    QGraphicsScene *scene;
    int currentZ;
    QGraphicsItem *lastSelectedItem;
    double scaleF;
    double snapDistance;
    double pieceScan;
    QMap<QGraphicsItem*, QPointF> originalPositions;
};


#endif // INTERACTIVEPIECE_H
