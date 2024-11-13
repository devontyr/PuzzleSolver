#include "puzzlesolverlayout.h"

#include <QtWidgets>
#include <imageviewer.h>

PuzzleSolverLayout::PuzzleSolverLayout(const QImage &_image):image(_image) {
    QImage redImage = processImage(image);
    pieceSeperator(redImage);

    // LAYOUTS
    // set the vertical box
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    setLayout(mainLayout);

    // add the ImageViewer to the top
    ImageViewer *imageViewer = new ImageViewer(redImage);
    mainLayout->addWidget(imageViewer);

    // add QGraphicsView to the bottom
    solverInterface = new QGraphicsView();
    mainLayout->addWidget(solverInterface);

    scene = new QGraphicsScene(this);
    solverInterface->setScene(scene);

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

QImage PuzzleSolverLayout::processImage(QImage& image) {
    //process the image into red
    QImage processedImage = QImage(image.width(), image.height(), QImage::Format_RGB32);

    for (int y = 0; y < image.height(); ++y) {
        for (int x = 0; x < image.width(); ++x) {
            QRgb pixel = image.pixel(x, y);
            if (!isShadeOfWhite(pixel)) {
                processedImage.setPixelColor(QPoint(x,y), Qt::red); //mark the pixel as a puzzle piece
            }
        }
    }
    return processedImage;
}

void PuzzleSolverLayout::pieceSeperator(QImage& image) {
    QSet<QPoint> PuzzlePixels; QVector<QSet<QPoint>> PuzzlePieces;
    QSet<QPoint> toDo;
    int curPiecePixels = 0; int minPieceSize = 15000;
    int C = image.width(), R = image.height();
    QRgb orgColor = 0xffff0000; QRgb processedColor = 0xff0000bb;
    bool color = true;

    // loop through possible start positions to find the beginning of a piece
    for (int iStartRow=0; iStartRow < R; ++iStartRow) {
        for (int iStartCol=0; iStartCol < C; ++iStartCol) {

            //create a new puzzle piece
            QImage piece;

            // add first red pixel to the toDo
            QPoint curPoint = QPoint(iStartCol, iStartRow);
            if (image.pixel(curPoint) == orgColor) {
                toDo.insert(curPoint);
            } else {
                continue;
            }

            while (!toDo.isEmpty()) {

                QPoint topPoint = *toDo.begin();
                toDo.erase(toDo.begin()); // pop top pixel off the stack

                if(color) {processedColor = 0xff0000ff;}
                else {processedColor = 0xff00ff00;}

                image.setPixel(topPoint, processedColor); // turn it to a blue color

                // add it to the current puzzle piece
                piece.setPixel(topPoint, processedColor);
                PuzzlePixels.insert(topPoint);

                ++curPiecePixels; // add to the number of pixels

                // add any red colored neighbors to toDo
                QPoint lNeighbor = QPoint(topPoint.x() - 1, topPoint.y());
                QPoint rNeighbor = QPoint(topPoint.x() + 1, topPoint.y());
                QPoint tNeighbor = QPoint(topPoint.x(), topPoint.y() + 1);
                QPoint bNeighbor = QPoint(topPoint.x(), topPoint.y() - 1);
                if (image.valid(lNeighbor) && image.pixel(lNeighbor) == orgColor) toDo.insert(lNeighbor);
                if (image.valid(rNeighbor) && image.pixel(rNeighbor) == orgColor) toDo.insert(rNeighbor);
                if (image.valid(tNeighbor) && image.pixel(tNeighbor) == orgColor) toDo.insert(tNeighbor);
                if (image.valid(bNeighbor) && image.pixel(bNeighbor) == orgColor) toDo.insert(bNeighbor);
            }
            // once toDo empty, we have found a full piece
            // check that it has enough pixels and then add that pieces to the collection of pieces
            color = !color;

            qDebug() << "num Pixels: " << curPiecePixels;
            if (curPiecePixels >= minPieceSize) {
                PuzzlePieces.append(PuzzlePixels);

                //make into Pixmap and add to the scene
                scene->addPixmap(QPixmap::fromImage(piece));
            }

            qDebug() << "pieces:" << PuzzlePieces.size();
        }
    }
}
