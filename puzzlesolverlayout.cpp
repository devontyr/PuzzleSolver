#include "puzzlesolverlayout.h"
#include "interactivepiece.h"

#include <QtWidgets>
#include <imageviewer.h>

PuzzleSolverLayout::PuzzleSolverLayout(const QImage &_image):image(_image) {
    QImage imageToProcess = image.copy();
    pieceSeperator(imageToProcess);

    // LAYOUTS
    // set the vertical box
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    setLayout(mainLayout);

    // add the ImageViewer to the top
    ImageViewer *imageViewer = new ImageViewer(imageToProcess);
    mainLayout->addWidget(imageViewer);

    // add QGraphicsView to the bottom
    solverInterface = new QGraphicsView;
    mainLayout->addWidget(imageViewer);

    scene = new QGraphicsScene(this);
    solverInterface->setScene(scene);
    interactivePiece* interactivePieceLayout = new interactivePiece(pieces);
    mainLayout->addWidget(interactivePieceLayout);
}

bool PuzzleSolverLayout::isShadeOfWhite(const QRgb &color) {
    int threshold = 10; // larger num if we want to accept more gray colors as "white"
    int minBrightness = 169; // smaller num allows more gray colors as "white"

    //values are between 0-255
    int r = qRed(color);
    int g = qGreen(color);
    int b = qBlue(color);

    // Check if r/g/b are right hue (pure white/black would be 0)
    if (abs(r - g) > threshold || abs(r - b) > threshold || abs(g - b) > threshold) {
        return false;
    }
    // Check if r/g/b is right shade of white-gray-black
    return r >= minBrightness && g >= minBrightness && b >= minBrightness;
}

void PuzzleSolverLayout::pieceSeperator(QImage& image) {
    QSet<QPoint> PuzzlePixels; QVector<QSet<QPoint>> PuzzlePieces;
    QSet<QPoint> toDo;
    int minPieceSize = 30000;
    int C = image.width(), R = image.height();
    QRgb white = 0xffffffff;


    // loop through possible start positions to find the beginning of a piece
    for (int iStartRow=0; iStartRow < R; ++iStartRow) {
        for (int iStartCol=0; iStartCol < C; ++iStartCol) {

            //create a new puzzle piece
            QImage piece(C, R, QImage::Format_ARGB32);
            piece.fill(0x00ffffff);
            int curPiecePixels = 0;
            int minX = C; int minY = R; int maxX = 0; int maxY = 0;

            // add first valid pixel to the toDo
            QPoint curPoint = QPoint(iStartCol, iStartRow);
            if (!isShadeOfWhite(image.pixel(curPoint))) {
                toDo.insert(curPoint);
            } else {
                continue;
            }

            while (!toDo.isEmpty()) {

                QPoint topPoint = *toDo.begin();
                toDo.erase(toDo.begin()); // pop top pixel off the stack

                QRgb orgColor = image.pixel(topPoint);

                image.setPixel(topPoint, white); // make it white to avoid duplicate processes

                // add it to the current puzzle piece
                piece.setPixel(topPoint, orgColor);
                PuzzlePixels.insert(topPoint);
                minX = fmin(minX, topPoint.x()); minY = fmin(minY, topPoint.y());
                maxX = fmax(maxX, topPoint.x()); maxY = fmax(maxY, topPoint.y());

                ++curPiecePixels; // add to the number of pixels

                // add any valid colored neighbors to toDo
                QPoint lNeighbor = QPoint(topPoint.x() - 1, topPoint.y());
                QPoint rNeighbor = QPoint(topPoint.x() + 1, topPoint.y());
                QPoint tNeighbor = QPoint(topPoint.x(), topPoint.y() + 1);
                QPoint bNeighbor = QPoint(topPoint.x(), topPoint.y() - 1);
                if (image.valid(lNeighbor) && !isShadeOfWhite(image.pixel(lNeighbor))) toDo.insert(lNeighbor);
                if (image.valid(rNeighbor) && !isShadeOfWhite(image.pixel(rNeighbor))) toDo.insert(rNeighbor);
                if (image.valid(tNeighbor) && !isShadeOfWhite(image.pixel(tNeighbor))) toDo.insert(tNeighbor);
                if (image.valid(bNeighbor) && !isShadeOfWhite(image.pixel(bNeighbor))) toDo.insert(bNeighbor);
            }
            // once toDo empty, we have found a full piece
            // check that it has enough pixels and then add that pieces to the collection of pieces
            if (curPiecePixels >= minPieceSize) {
                PuzzlePieces.append(PuzzlePixels);

                //crop piece and add it to pieces
                QRect cropRect(minX, minY, maxX-minX, maxY-minY);
                QImage croppedPiece = piece.copy(cropRect);
                pieces.append(croppedPiece);
            }
        }
    }
}
