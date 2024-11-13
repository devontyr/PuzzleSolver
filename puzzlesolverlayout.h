#ifndef PUZZLESOLVERLAYOUT_H
#define PUZZLESOLVERLAYOUT_H

#include <QtWidgets>

class PuzzleSolverLayout: public QWidget
{
    Q_OBJECT
    QImage image;
    QGraphicsScene* scene;
    QGraphicsView *solverInterface;

    bool isShadeOfWhite(const QRgb &color);
    QImage processImage(QImage& image);
    void pieceSeperator(QImage& image);

public:
    explicit PuzzleSolverLayout(const QImage &_image);
};

#endif // PUZZLESOLVERLAYOUT_H
