#include "interactivepiece.h"
#include <cmath>

using namespace std;


/*
Main method show all the processed puzzle pieces in a user friendly, interactive format
    input: a list of pieces as QImages
    output: displays each individual piece with the ability to drag, resize, rotate them
*/
interactivePiece::interactivePiece(const QList<QImage> &pieces, QWidget *parent)
    : QGraphicsView(parent), scene(new QGraphicsScene(this)), currentZ(1) {

    setScene(scene);
    setBackgroundBrush(Qt::white);

    grabKeyboard();
    scaleF = 1.0;

    snapDistance = 200;
    pieceScan = 260;


    int pos1 = 0;
    for (const QImage &piece : pieces) {
        QGraphicsPixmapItem *item = new QGraphicsPixmapItem(QPixmap::fromImage(piece));
        item -> setFlags(QGraphicsItem::ItemIsMovable);
        scene -> addItem(item);
        item->setTransformOriginPoint(item->boundingRect().center());
        item->setPos(pos1, 0);
        pos1 += 600;
        item -> setScale(scaleF);

        originalPositions[item] = item->pos();

    }
    setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);

}

/*
Mouse Pressed function. Moves selected piece to front of focus
    input: mouse event
    output: selected piece given new Z value
*/
void interactivePiece::mousePressEvent(QMouseEvent *evt) {
    QGraphicsItem *item = itemAt(evt->pos());
    if (item) {
        item->setZValue(++currentZ);
        lastSelectedItem = item;
    }

    QGraphicsView::mousePressEvent(evt);
}

/*
Kep Pressed function. Rotates a single piece or rescales all pieces.
    input: key event
    output: rotated or rescaled piece
*/
void interactivePiece::keyPressEvent(QKeyEvent *evt) {
    if (evt->key() == Qt::Key_Equal || evt->key() == Qt::Key_Minus) {
        double scaleFactor = (evt->key() == Qt::Key_Equal) ? 2.0 : 0.5;
        scaleF *= scaleFactor;

        this->scale(scaleFactor, scaleFactor);

        QTransform transform = this->viewportTransform();

        for (QGraphicsItem *item : scene->items()) {
            QPointF itemScenePos = item->pos();
            QPointF itemViewPos = transform.map(itemScenePos);
            QPointF newScenePos = transform.inverted().map(itemViewPos);
            item->setPos(newScenePos);
        }
    } else if (lastSelectedItem) {
        if (evt->key() == Qt::Key_D) {
            lastSelectedItem->setRotation(lastSelectedItem->rotation() + 90);
        } else if (evt->key() == Qt::Key_A) {
            lastSelectedItem->setRotation(lastSelectedItem->rotation() - 90);
        }
    }

    QGraphicsView::keyPressEvent(evt);
}

/*
Mouse Released function. Moves selected piece to new location, snaps onto another piece if it is close enough
    input: mouse event
    output: selected piece moved to new location
*/
void interactivePiece::mouseReleaseEvent(QMouseEvent *evt) {
    QGraphicsItem *item = itemAt(evt->pos());
    if (!item) {
        QGraphicsView::mouseReleaseEvent(evt);
        return;
    }

    int snapRadius = snapDistance; // like a threshhold for how far out a piece will snap
    int pieceSize = pieceScan; // replace with the piece's actual size (minus a rough estimate for half a nub)
    QPointF itemPos = item->pos();
    QRectF searchArea(itemPos.x() - snapRadius, itemPos.y() - snapRadius, snapRadius * 2, snapRadius * 2);
    QGraphicsItem *closestItem = nullptr;
    int closestDistance = snapRadius;
    QPointF snapPosition;

    for (QGraphicsItem *otherItem : scene->items(searchArea)) {
        if (otherItem == item) continue;

        QPointF otherPos = otherItem->pos();
        int distance = QLineF(itemPos, otherPos).length();

        if (distance < closestDistance) {
            int deltaX = itemPos.x() - otherPos.x();
            int deltaY = itemPos.y() - otherPos.y();
            QPointF potentialSnapPos;

            if (abs(deltaX) > abs(deltaY)) {
                potentialSnapPos = QPointF(otherPos.x() + (deltaX > 0 ? pieceSize : -pieceSize), otherPos.y());
            } else {
                potentialSnapPos = QPointF(otherPos.x(), otherPos.y() + (deltaY > 0 ? pieceSize : -pieceSize));
            }

            bool positionOccupied = false;
            for (QGraphicsItem *checkItem : scene->items(QRectF(potentialSnapPos, QSizeF(pieceSize, pieceSize)))) {
                if (checkItem != item) {
                    positionOccupied = true;
                    break;
                }
            }

            if (!positionOccupied) {
                closestItem = otherItem;
                closestDistance = distance;
                snapPosition = potentialSnapPos;
            }
        }
    }

    if (closestItem) {
        item->setPos(snapPosition);
    }

    QGraphicsView::mouseReleaseEvent(evt);
}

void interactivePiece::saveDataSlot() {
    QByteArray byteArray;
    QDataStream out(&byteArray, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_5_15); // Ensure compatibility

    QList<QGraphicsItem *> items = scene->items();
    out << items.size();

    for (QGraphicsItem *item : items) {
        QGraphicsPixmapItem *pixmapItem = qgraphicsitem_cast<QGraphicsPixmapItem *>(item);
        if (pixmapItem) {
            out << pixmapItem->pos();
            out << pixmapItem->rotation();
            out << pixmapItem->scale();

            QImage image = pixmapItem->pixmap().toImage();
            out << image;
        }
    }
    emit emitSave(byteArray);
}

void interactivePiece::loadData(const QByteArray &data) {
    QDataStream in(data);
    in.setVersion(QDataStream::Qt_5_15);

    int itemCount;
    in >> itemCount;

    for (int i = 0; i < itemCount; ++i) {
        QPointF position;
        double rotation, scale;
        QImage image;

        in >> position >> rotation >> scale >> image;

        addPiece(image, position, rotation, scale);
    }
}

void interactivePiece::addPiece(const QImage &image, const QPointF &position, double rotation, double scale) {
    QGraphicsPixmapItem *pixmapItem = new QGraphicsPixmapItem(QPixmap::fromImage(image));
    pixmapItem->setFlags(QGraphicsItem::ItemIsMovable);
    pixmapItem->setPos(position);
    pixmapItem->setRotation(rotation);
    pixmapItem->setScale(scale);

    scene->addItem(pixmapItem);
}


