#ifndef PUZZLESOLVERLAYOUT_H
#define PUZZLESOLVERLAYOUT_H

#include <QtWidgets>

class PuzzleSolverLayout: public QWidget
{
    Q_OBJECT
    QImage image;
    QImage redImage;
    QGraphicsScene* scene;
    QGraphicsView *solverInterface;
    QList<QImage> pieces;
    QList<QVector<QVector<int>>> pieceMatricies;

    bool isShadeOfWhite(const QRgb &color);
    bool isShadeOfBlack(const QRgb &color);
    bool isSurroundedRed(int pixelX, int pixelY);
    void pieceSeperator(QImage& image, QImage& redImage);
    void processImage(QImage& image);
    void pieceOutput();
    QVector<QVector<int>> imageToMatrix(QImage &pieceImg);

public:
    explicit PuzzleSolverLayout(const QImage &_image);
};

#endif // PUZZLESOLVERLAYOUT_H
