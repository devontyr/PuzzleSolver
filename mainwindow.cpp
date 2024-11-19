#include "mainwindow.h"
#include <imageviewer.h>
#include <QtWidgets>

MainWindow *mainWindow;

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

int Waiter::count = 0;

Waiter::Waiter() {
    if (++count == 1) {
        mainWindow->setEnabled(false);
        mainWindow->statusBar()->showMessage("patience is a virtue...");
        mainWindow->statusBar()->show();
        QApplication::processEvents();
    }
}

Waiter::~Waiter(){
    if (--count == 0) {
        mainWindow->setEnabled(true);
        mainWindow->statusBar()->clearMessage();
        mainWindow->statusBar()->hide();
        QApplication::processEvents();
    }
}


/*
Handles and processes a given file into puzzle pieces when user opens any image
*/
void MainWindow::openImageSlot() {
    //open file as QImage and put on screen
    QString fName = QFileDialog::getOpenFileName(this, "select image file", lastDir, "image files (*.png *.jpg *.bmp *.jpeg)");
    if (fName.isEmpty()) return;
    QImage image(fName);
    if (image.isNull()) return;

    lastDir = QFileInfo(fName).absolutePath(); //update last directory

    //save the center of the original image
    orgImageCenter = QPoint(image.height()/2, image.width()/2);

    puzzleLayout = new PuzzleSolverLayout(image);
    setCentralWidget(puzzleLayout);
}
