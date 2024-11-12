#include "mainwindow.h"
#include <imageviewer.h>
#include <QtWidgets>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent) {

    setWindowTitle("Puzzle Solver");

    //MENU BAR
    QAction *openImageAct = new QAction("Open Image");
    connect(openImageAct, &QAction::triggered, this, &MainWindow::openImageSlot);
    openImageAct->setShortcut(Qt::CTRL | Qt::Key_O); //slot triggered with ctrl O key

    QAction *solvePuzzleAct = new QAction("Solve Puzzle");
    connect(solvePuzzleAct, &QAction::triggered, this, &MainWindow::openImageSlot);
    solvePuzzleAct->setShortcut(Qt::CTRL | Qt::Key_S); //slot triggered with ctrl S key

    QMenu *fileMenu = new QMenu("&File");
    fileMenu->addAction(openImageAct);
    fileMenu->addAction(solvePuzzleAct);
    menuBar()->addMenu(fileMenu);

    //save the last directory
    QSettings settings("Group", "puzzle solver");
    lastDir = settings.value("lastDir", "").toString();


}

MainWindow::~MainWindow() {
    QSettings settings("Group", "puzzle solver");
    lastDir = settings.value("lastDir", lastDir).toString();
}

bool MainWindow::isShadeOfWhite(const QRgb &color) {
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

QImage* MainWindow::floodFill(QImage* image) {
    QSet<QPoint> PuzzlePixels; QVector<QSet<QPoint>> PuzzlePieces;
    QSet<QPoint> toDo;
    int curPiecePixels = 0;
    int C = image->width(), R = image->height();
    QRgb orgColor = 0xffff0000; QRgb processedColor = 0xff0000bb;


    // loop through possible start positions to find the beginning of a piece
    for (int iStartRow=0; iStartRow < R; ++iStartRow) {
        for (int iStartCol=0; iStartCol < C; ++iStartCol) {

            // add first red pixel to the toDo
            QPoint curPoint = QPoint(iStartRow, iStartCol);
            if (image->pixel(curPoint) == orgColor) {
                toDo.insert(curPoint);
            } else {
                continue;
            }

            while (!toDo.isEmpty()) {
                QPoint topPoint = *toDo.begin(); // pop top pixel off the stack
                image->setPixel(topPoint, processedColor); // turn it to a blue color
                PuzzlePixels.insert(topPoint); // add it to the current puzzle piece
                ++curPiecePixels; // add to the number of pixels

                // add any red colored neighbors to toDo
                QPoint lNeighbor = QPoint(topPoint.x() - 1, topPoint.y());
                QPoint rNeighbor = QPoint(topPoint.x() + 1, topPoint.y());
                QPoint tNeighbor = QPoint(topPoint.x(), topPoint.y() + 1);
                QPoint bNeighbor = QPoint(topPoint.x(), topPoint.y() - 1);
                if (image->valid(lNeighbor) && image->pixel(lNeighbor) == orgColor) toDo.insert(lNeighbor);
                if (image->valid(rNeighbor) && image->pixel(rNeighbor) == orgColor) toDo.insert(rNeighbor);
                if (image->valid(tNeighbor) && image->pixel(tNeighbor) == orgColor) toDo.insert(tNeighbor);
                if (image->valid(bNeighbor) && image->pixel(bNeighbor) == orgColor) toDo.insert(bNeighbor);
            }
            // once toDo empty, we have found a full piece
            // check that it has enough pixels and then add that pieces to the collection of pieces
            if (curPiecePixels > 250) PuzzlePieces.append(PuzzlePixels);
        }
    }
    return image;
}

void MainWindow::openImageSlot() {
    //open file as QImage and put on screen
    QString fName = QFileDialog::getOpenFileName(this, "select image file", lastDir, "image files (*.png *.jpg *.bmp *.jpeg)");
    if (fName.isEmpty()) return;
    QImage image(fName);
    if (image.isNull()) return;

    lastDir = QFileInfo(fName).absolutePath(); //update last directory

    //save the center of the original image
    orgImageCenter = new QPoint(image.height()/2, image.width()/2);


    //process the image into B&W
    processedImage = new QImage(image.width(), image.height(), QImage::Format_RGB32);

    for (int y = 0; y < image.height(); ++y) {
        for (int x = 0; x < image.width(); ++x) {
            QRgb pixel = image.pixel(x, y);
            if (!isShadeOfWhite(pixel)) {
                processedImage->setPixelColor(QPoint(x,y), Qt::red); //mark the pixel as a puzzle piece
            }
        }
    }

    processedImage = floodFill(processedImage);

    puzzleLayout = new PuzzleSolverLayout(*processedImage);
    setCentralWidget(puzzleLayout);
}
