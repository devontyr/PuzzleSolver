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

    //Save last directory
    QSettings settings("Group", "puzzle solver");
    lastDir = settings.value("lastDir", "").toString();

}

MainWindow::~MainWindow() {
    QSettings settings("Group", "puzzle solver");
    lastDir = settings.value("lastDir", "").toString();
}

void MainWindow::openImageSlot() {
    //open file as pixmap and put on screen
    QString fName = QFileDialog::getOpenFileName(this, "select image file", lastDir, "image files (*.png *.jpg *.bmp *.jpeg)");
    if (fName.isEmpty()) return;
    QImage image(fName);
    if (image.isNull()) return;

    lastDir = QFileInfo(fName).absolutePath(); //update last directory


    puzzleLayout = new PuzzleSolverLayout(image);
    setCentralWidget(puzzleLayout);
}
