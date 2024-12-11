#ifndef IMAGEPROCESS_H
#define IMAGEPROCESS_H

#include <QtWidgets>
#include "puzzlepiece.h"

class ImageProcess: public QWidget
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
    pair <int, int> findCorner(QVector<QVector<int>> piece, QVector<QVector<int>> idealCorner);
    void findEdge(pair<int, int> corner1, pair<int, int> corner2, pair<int, int> direction);
    void mapEdges(QVector<QVector<int>> piece);

public:
    explicit ImageProcess(const QImage &_image);
};

#endif // IMAGEPROCESS_H
