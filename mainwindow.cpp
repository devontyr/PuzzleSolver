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

    /*
    QColor pixel1 = processedImage->pixel(pixelX - 1, pixelY - 1);
    QColor pixel2 = processedImage->pixel(pixelX, pixelY - 1);
    QColor pixel3 = processedImage->pixel(pixelX + 1, pixelY - 1);

    QColor pixel4 = processedImage->pixel(pixelX -1, pixelY);
    QColor pixel5 = processedImage->pixel(pixelX + 1, pixelY );

    QColor pixel6 = processedImage->pixel(pixelX - 1, pixelY + 1);
    QColor pixel7 = processedImage->pixel(pixelX, pixelY + 1);
    QColor pixel8 = processedImage->pixel(pixelX + 1, pixelY + 1);
    */

    QColor pixel1 = processedImage->pixel(pixelX - 2, pixelY - 2);
    QColor pixel2 = processedImage->pixel(pixelX - 1, pixelY - 2);
    QColor pixel3 = processedImage->pixel(pixelX, pixelY - 2);
    QColor pixel4 = processedImage->pixel(pixelX + 1, pixelY - 2);
    QColor pixel5 = processedImage->pixel(pixelX + 2, pixelY - 2);

    QColor pixel6 = processedImage->pixel(pixelX - 2, pixelY - 1);
    QColor pixel7 = processedImage->pixel(pixelX - 1, pixelY - 1);
    QColor pixel8 = processedImage->pixel(pixelX, pixelY - 1);
    QColor pixel9 = processedImage->pixel(pixelX + 1, pixelY - 1);
    QColor pixel10 = processedImage->pixel(pixelX + 2, pixelY - 1);

    QColor pixel11 = processedImage->pixel(pixelX - 2, pixelY);
    QColor pixel12 = processedImage->pixel(pixelX - 1, pixelY);
    QColor pixel13 = processedImage->pixel(pixelX, pixelY);
    QColor pixel14 = processedImage->pixel(pixelX + 1, pixelY);
    QColor pixel15 = processedImage->pixel(pixelX + 2, pixelY);

    QColor pixel16 = processedImage->pixel(pixelX - 2, pixelY + 1);
    QColor pixel17 = processedImage->pixel(pixelX - 1, pixelY + 1);
    QColor pixel18 = processedImage->pixel(pixelX, pixelY + 1);
    QColor pixel19 = processedImage->pixel(pixelX + 1, pixelY + 1);
    QColor pixel20 = processedImage->pixel(pixelX + 2, pixelY + 1);

    QColor pixel21 = processedImage->pixel(pixelX - 2, pixelY + 2);
    QColor pixel22 = processedImage->pixel(pixelX - 1, pixelY + 2);
    QColor pixel23 = processedImage->pixel(pixelX, pixelY + 2);
    QColor pixel24 = processedImage->pixel(pixelX + 1, pixelY + 2);
    QColor pixel25 = processedImage->pixel(pixelX + 2, pixelY + 2);


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
    if (pixel9.red() == 255){
        ++redCount;
    }
    if (pixel10.red() == 255){
        ++redCount;
    }
    if (pixel11.red() == 255){
        ++redCount;
    }
    if (pixel12.red() == 255){
        ++redCount;
    }
    if (pixel13.red() == 255){
        ++redCount;
    }
    if (pixel14.red() == 255){
        ++redCount;
    }
    if (pixel15.red() == 255){
        ++redCount;
    }
    if (pixel16.red() == 255){
        ++redCount;
    }
    if (pixel17.red() == 255){
        ++redCount;
    }
    if (pixel18.red() == 255){
        ++redCount;
    }
    if (pixel19.red() == 255){
        ++redCount;
    }
    if (pixel20.red() == 255){
        ++redCount;
    }
    if (pixel21.red() == 255){
        ++redCount;
    }
    if (pixel22.red() == 255){
        ++redCount;
    }
    if (pixel23.red() == 255){
        ++redCount;
    }
    if (pixel24.red() == 255){
        ++redCount;
    }
    if (pixel25.red() == 255){
        ++redCount;
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

    /*
    for (int y = 0; y < image.height(); ++y) {
        for (int x = 0; x < image.width(); ++x) {
            if(isSurroundedRed(x, y)){
                processedImage->setPixelColor(QPoint(x,y), Qt::red); //mark the pixel as red
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

    for (int y = 0; y < image.height(); ++y) {
        for (int x = 0; x < image.width(); ++x) {
            if(isSurroundedRed(x, y)){
                processedImage->setPixelColor(QPoint(x,y), Qt::red); //mark the pixel as red
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
    */

    puzzleLayout = new PuzzleSolverLayout(*processedImage);
    setCentralWidget(puzzleLayout);
}
