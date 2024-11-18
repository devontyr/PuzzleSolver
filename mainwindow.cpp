#include "mainwindow.h"
#include <imageviewer.h>
#include <QtWidgets>
#include <QtMath>

using namespace std;

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

// 45 40 48

MainWindow::~MainWindow() {
    QSettings settings("Group", "puzzle solver");
    lastDir = settings.value("lastDir", lastDir).toString();
}

bool MainWindow::isShadeOfWhite(const QRgb &color) {
    //values are between 0-255
    int r = qRed(color);
    int g = qGreen(color);
    int b = qBlue(color);

    int combo = r + g + b;

    if (combo < 100){
        return true;
    }

    if ((qAbs(r - 45) <= 15) && (qAbs(g - 40) <= 15) && (qAbs(b - 48) <= 15)){
        return true;
    }
    else {
        return false;
    }
}

bool MainWindow::isSurroundedRed(int pixelX, int pixelY){
    int redCount = 0;
    if ((pixelX <= 1) || (pixelY <=1) || (pixelX >= processedImage->width() - 2) || (pixelY >= processedImage->height() - 2)){
        return false;
    }

    QList<QRgb> surroundingPix(25);

    int index = 0;
    for (int row = -2; row <= 2; ++row){
        for (int col = -2; col <= 2; ++col){
            surroundingPix[index] = processedImage->pixel(pixelX + col, pixelY + row);
            ++index;
        }
    }

    for (int i = 0; i < 25; ++i){
        if (qRed(surroundingPix[i]) == 255){
            ++redCount;
        }
    }

    return (redCount >= 13);

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
    //processedImage->fill(Qt::white);

    for (int y = 0; y < image.height(); ++y) {
        for (int x = 0; x < image.width(); ++x) {
            QRgb pixel = image.pixel(x, y);
            if (!isShadeOfWhite(pixel)) {
                processedImage->setPixelColor(QPoint(x,y), Qt::red); //mark the pixel as a puzzle piece
            }
            else{
                processedImage->setPixelColor(QPoint(x,y), Qt::black); //mark the pixel as a puzzle piece

            }
        }
    }


    for (int y = 0; y < image.height(); ++y) {
        for (int x = 0; x < image.width(); ++x) {
            if(isSurroundedRed(x, y)){
                processedImage->setPixelColor(QPoint(x,y), Qt::red); //mark the pixel as red
            }
        }
    }


    puzzleLayout = new PuzzleSolverLayout(*processedImage);
    setCentralWidget(puzzleLayout);
}
