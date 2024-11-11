 #include "interactivepiece.h"
#include <cmath>

using namespace std;

interactivePiece::interactivePiece(const QList<QImage> &pieces, QWidget *parent)
    : QGraphicsView(parent), scene(new QGraphicsScene(this)), currentZ(1) {

    setScene(scene);

    int pos1 = 10;
    for (const QImage &piece : pieces) {
        QGraphicsPixmapItem *item = new QGraphicsPixmapItem(QPixmap::fromImage(piece));
        item -> setFlags(QGraphicsItem::ItemIsMovable);
        scene -> addItem(item);

        item->setPos(pos1, pos1);
        pos1 += 30;

    }

    setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);

}

void interactivePiece::mousePressEvent(QMouseEvent *evt) {
    QGraphicsItem *item = itemAt(evt->pos());
    if (item) {
        item->setZValue(++currentZ);
    }

    QGraphicsView::mousePressEvent(evt);
}

void interactivePiece::mouseReleaseEvent(QMouseEvent *evt) {
    QGraphicsItem *item = itemAt(evt->pos());
    if (!item) {
        QGraphicsView::mouseReleaseEvent(evt);
        return;
    }

    const int snapRadius = 60;  // how far out a piece will snap
    const int pieceSize = 52;   // replace with the piece's actual size (maybe plus a tiny bit of room)

    QPointF itemPos = item->pos();

    QRectF searchArea(itemPos.x() - snapRadius, itemPos.y() - snapRadius, snapRadius * 2, snapRadius * 2);

    QGraphicsItem *closestItem = nullptr;
    int closestDistance = snapRadius;

    for (QGraphicsItem *otherItem : scene->items(searchArea)) {
        if (otherItem == item) continue;

        QPointF otherPos = otherItem->pos();
        int distance = QLineF(itemPos, otherPos).length();

        if (distance < closestDistance) {
            closestItem = otherItem;
            closestDistance = distance;
        }
    }

    if (closestItem) {
        QPointF otherPos = closestItem->pos();

        int deltaX = itemPos.x() - otherPos.x();
        int deltaY = itemPos.y() - otherPos.y();

        if (std::abs(deltaX) > std::abs(deltaY)) {
            item->setPos(otherPos.x() + (deltaX > 0 ? pieceSize : -pieceSize), otherPos.y());
        } else {
            item->setPos(otherPos.x(), otherPos.y() + (deltaY > 0 ? pieceSize : -pieceSize));
        }
    }


    QGraphicsView::mouseReleaseEvent(evt);
}
