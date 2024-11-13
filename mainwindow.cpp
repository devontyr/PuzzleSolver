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

    if (combo <= 600 && combo >=200){
        if ((qAbs(b-r) + qAbs(b-g) +qAbs(r-g) > 28) && (b > r || b > g)){
            return false;
        }
        else {
            return true;
        }
    }
    else{
        return true;
    }
}

bool MainWindow::isSurroundedRed(int pixelX, int pixelY){
    int redCount = 0;
    if ((pixelX == 0) || (pixelY == 0) || (pixelX == processedImage->width() - 1) || (pixelY == processedImage->height() - 1)){
        return false;
    }

    QColor pixel1 = processedImage->pixel(pixelX - 1, pixelY - 1);
    QColor pixel2 = processedImage->pixel(pixelX, pixelY - 1);
    QColor pixel3 = processedImage->pixel(pixelX + 1, pixelY - 1);

    QColor pixel4 = processedImage->pixel(pixelX -1, pixelY);
    QColor pixel5 = processedImage->pixel(pixelX + 1, pixelY );

    QColor pixel6 = processedImage->pixel(pixelX - 1, pixelY + 1);
    QColor pixel7 = processedImage->pixel(pixelX, pixelY + 1);
    QColor pixel8 = processedImage->pixel(pixelX + 1, pixelY + 1);


    if (pixel1.red() == 255){
        ++redCount;
    }

    if (pixel2.red() == 255){
        ++redCount;
    }

    if (pixel3.red() == 255){
        ++redCount;
    }

    if (pixel4.red() == 255){
        ++redCount;
    }

    if (pixel5.red() == 255){
        ++redCount;
    }

    if (pixel6.red() == 255){
        ++redCount;
    }

    if (pixel7.red() == 255){
        ++redCount;
    }

    if (pixel8.red() == 255){
        ++redCount;
    }

    return (redCount >= 5);
}


bool MainWindow::isSurroundedBlack(int pixelX, int pixelY){
    int blackCount = 0;
    if ((pixelX == 0) || (pixelY == 0) || (pixelX == processedImage->width() - 1) || (pixelY == processedImage->height() - 1)){
        return false;
    }

    QColor pixel1 = processedImage->pixel(pixelX - 1, pixelY - 1);
    QColor pixel2 = processedImage->pixel(pixelX, pixelY - 1);
    QColor pixel3 = processedImage->pixel(pixelX + 1, pixelY - 1);

    QColor pixel4 = processedImage->pixel(pixelX -1, pixelY);
    QColor pixel5 = processedImage->pixel(pixelX + 1, pixelY );

    QColor pixel6 = processedImage->pixel(pixelX - 1, pixelY + 1);
    QColor pixel7 = processedImage->pixel(pixelX, pixelY + 1);
    QColor pixel8 = processedImage->pixel(pixelX + 1, pixelY + 1);


    if (pixel1.red() == 0){
        ++blackCount;
    }

    if (pixel2.red() == 0){
        ++blackCount;
    }

    if (pixel3.red() == 0){
        ++blackCount;
    }

    if (pixel4.red() == 0){
        ++blackCount;
    }

    if (pixel5.red() == 0){
        ++blackCount;
    }

    if (pixel6.red() == 0){
        ++blackCount;
    }

    if (pixel7.red() == 0){
        ++blackCount;
    }

    if (pixel8.red() == 0){
        ++blackCount;
    }

    return (blackCount >= 6);
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

            if(isSurroundedBlack(x,y)){
                processedImage->setPixelColor(QPoint(x,y), Qt::black); //mark the pixel as red
            }
            if(isSurroundedRed(x, y)){
                processedImage->setPixelColor(QPoint(x,y), Qt::red); //mark the pixel as red
            }
        }
    }

    for (int y = 0; y < image.height(); ++y) {
        for (int x = 0; x < image.width(); ++x) {

            if(isSurroundedBlack(x,y)){
                processedImage->setPixelColor(QPoint(x,y), Qt::black); //mark the pixel as red
            }
            if(isSurroundedRed(x, y)){
                processedImage->setPixelColor(QPoint(x,y), Qt::red); //mark the pixel as red
            }
        }
    }


    puzzleLayout = new PuzzleSolverLayout(*processedImage);
    setCentralWidget(puzzleLayout);
}
