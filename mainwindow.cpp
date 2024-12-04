#include "mainwindow.h"
#include <imageviewer.h>
#include <QtWidgets>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), imageViewer(nullptr), processed(false) {

    setWindowTitle("Puzzle Solver");

    resize(500, 300);

    QWidget *center = new QWidget();
    setCentralWidget(center);

    QAction *uploadAct = new QAction("Open Image");
    QPushButton *uploadButton = new QPushButton("Open Image");
    connect(uploadButton, &QPushButton::clicked, uploadAct, &QAction::trigger);
    connect(uploadAct, &QAction::triggered, this, &MainWindow::openImageSlot);
    uploadAct->setShortcut(Qt::CTRL | Qt::Key_U);

    addImageAct = new QAction("Add Image");
    addImageButton = new QPushButton("Add Image");
    connect(addImageButton, &QPushButton::clicked, addImageAct, &QAction::trigger);
    connect(addImageAct, &QAction::triggered, this, &MainWindow::addImageSlot);
    addImageAct->setShortcut(Qt::CTRL | Qt::Key_I);
    addImageAct->setEnabled(false);
    addImageButton->setEnabled(false);

    processAct = new QAction("Process");
    processButton = new QPushButton("Process");
    connect(processButton, &QPushButton::clicked, processAct, &QAction::trigger);
    connect(processAct, &QAction::triggered, this, &MainWindow::processSlot);
    processAct->setShortcut(Qt::CTRL | Qt::Key_P);
    processAct->setEnabled(false);
    processButton->setEnabled(false);

    solvePuzzleAct = new QAction("Solve");
    solveButton = new QPushButton("Solve");
    connect(solveButton, &QPushButton::clicked, solvePuzzleAct, &QAction::trigger);
    connect(solvePuzzleAct, &QAction::triggered, this, &MainWindow::solveSlot);
    solvePuzzleAct->setShortcut(Qt::CTRL | Qt::Key_S);
    solvePuzzleAct->setEnabled(false);
    solveButton->setEnabled(false);

    hintAct = new QAction("Show Hint");
    hintButton = new QPushButton("Show Hint");
    connect(hintButton, &QPushButton::clicked, hintAct, &QAction::trigger);
    connect(hintAct, &QAction::triggered, this, &MainWindow::hintSlot);
    hintAct->setShortcut(Qt::CTRL | Qt::Key_H);
    hintAct->setEnabled(false);
    hintButton->setEnabled(false);

    resetAct = new QAction("Reset");
    resetButton = new QPushButton("Reset");
    connect(resetButton, &QPushButton::clicked, resetAct, &QAction::trigger);
    connect(resetAct, &QAction::triggered, this, &MainWindow::resetSlot);
    resetAct->setShortcut(Qt::CTRL | Qt::Key_R);
    resetAct->setEnabled(false);
    resetButton->setEnabled(false);

    // Menu Bar Setup
    QMenu *fileMenu = new QMenu("&File");
    fileMenu->addAction(uploadAct);
    fileMenu->addAction(addImageAct);
    fileMenu->addAction(processAct);
    fileMenu->addAction(solvePuzzleAct);
    fileMenu->addAction(hintAct);
    fileMenu->addAction(resetAct);
    menuBar()->addMenu(fileMenu);

    // Save the last directory
    QSettings settings("Group", "puzzle solver");
    lastDir = settings.value("lastDir", "").toString();

    mainLayout = new QHBoxLayout();

    QWidget *buttonPanel = new QWidget();
    buttonPanel->setFixedWidth(100);
    QVBoxLayout *buttonLayout = new QVBoxLayout(buttonPanel);

    buttonLayout->addWidget(uploadButton);
    buttonLayout->addWidget(addImageButton);
    buttonLayout->addWidget(processButton);
    buttonLayout->addWidget(solveButton);
    buttonLayout->addWidget(hintButton);
    buttonLayout->addWidget(resetButton);
    buttonLayout->addStretch();

    mainLayout->addWidget(buttonPanel);

    QFrame *separator = new QFrame();
    separator->setFrameShape(QFrame::VLine);
    separator->setFrameShadow(QFrame::Sunken);
    mainLayout->addWidget(separator);


    scrollArea = new QScrollArea(this);
    scrollArea->setWidgetResizable(true);
    placeholder = new QWidget();
    // placeholder->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    scrollArea->setWidget(placeholder);
    mainLayout->addWidget(scrollArea);


    center->setLayout(mainLayout);
}

MainWindow::~MainWindow() {
    QSettings settings("Group", "puzzle solver");
    lastDir = settings.value("lastDir", lastDir).toString();

}


/*
Handles and processes a given file into puzzle pieces when user opens any image
*/
void MainWindow::openImageSlot() {
    QString fName = QFileDialog::getOpenFileName(this, "Select Image File", lastDir, "Image Files (*.png *.jpg *.bmp *.jpeg)");
    if (fName.isEmpty()) return;

    QImage image(fName);
    if (image.isNull()) return;

    lastDir = QFileInfo(fName).absolutePath();

    // if (imageViewer) delete imageViewer;
    imageViewer = new ImageViewer(this);

    placeholder->setLayout(new QVBoxLayout());
    placeholder->layout()->addWidget(imageViewer);

    imageViewer->addImage(image);

    addImageAct->setEnabled(true);
    addImageButton->setEnabled(true);

    processAct->setEnabled(true);
    processButton->setEnabled(true);

    resetAct->setEnabled(true);
    resetButton->setEnabled(true);
}

void MainWindow::addImageSlot() {
    QString fName = QFileDialog::getOpenFileName(this, "Select Additional Image", lastDir, "Image Files (*.png *.jpg *.bmp *.jpeg)");
    if (fName.isEmpty()) return;

    QImage image(fName);
    if (image.isNull()) return;

    lastDir = QFileInfo(fName).absolutePath();

    if (imageViewer) {
        imageViewer->addImage(image);
    } else {
        qDebug() << "No image viewer available to add the image.";
    }
}

void MainWindow::processSlot() {
    if (!imageViewer) {
        qDebug() << "No images to process.";
        return;
    }

    QList<QGraphicsItem *> items = imageViewer->items();
    QList<QImage> images;

    for (QGraphicsItem *item : items) {
        QGraphicsPixmapItem *pixmapItem = qgraphicsitem_cast<QGraphicsPixmapItem *>(item);
        if (pixmapItem) {
            QPixmap pixmap = pixmapItem->pixmap();
            if (!pixmap.isNull()) {
                images.append(pixmap.toImage());
            }
        }
    }

    if (images.isEmpty()) {
        qDebug() << "No valid images to process.";
        return;
    }

    int totalHeight = 0;
    int maxWidth = 0;
    for (const QImage &img : images) {
        totalHeight += img.height();
        maxWidth = std::max(maxWidth, img.width());
    }

    QImage stitchedImage(maxWidth, totalHeight, QImage::Format_ARGB32);
    stitchedImage.fill(Qt::transparent);

    int yOffset = 0;
    for (const QImage &img : images) {
        for (int y = 0; y < img.height(); ++y) {
            for (int x = 0; x < img.width(); ++x) {
                stitchedImage.setPixel(x, yOffset + y, img.pixel(x, y));
            }
        }
        yOffset += img.height();
    }

    if (placeholder && placeholder->layout()) {
        QLayout *layout = placeholder->layout();
        while (QLayoutItem *item = layout->takeAt(0)) {
            delete item->widget();
            delete item;
        }
        delete layout;
    }

    processed = true;
    scrollArea->setVisible(false);
    if (puzzleLayout) delete puzzleLayout;

    puzzleLayout = new PuzzleSolverLayout(stitchedImage);
    mainLayout->addWidget(puzzleLayout);

    addImageAct->setEnabled(false);
    addImageButton->setEnabled(false);

    processAct->setEnabled(false);
    processButton->setEnabled(false);

    solvePuzzleAct->setEnabled(true);
    solveButton->setEnabled(true);
}


/*
Runs solving algorith on puzzle
*/
void MainWindow::solveSlot() {
    qDebug() << "will solve soon...";


    hintAct->setEnabled(true);
    hintButton->setEnabled(true);
}

/*
gives the user a hint on solving the puzzle
*/
void MainWindow::hintSlot() {
    qDebug() << "will give hint soon...";
}

/*
resets the program
*/
void MainWindow::resetSlot() {
    processedImage = QImage();
    orgImageCenter = QPoint();
    scrollArea->setVisible(true);

    // Disable all buttons and actions
    processAct->setEnabled(false);
    processButton->setEnabled(false);

    addImageAct->setEnabled(false);
    addImageButton->setEnabled(false);

    solvePuzzleAct->setEnabled(false);
    solveButton->setEnabled(false);

    hintAct->setEnabled(false);
    hintButton->setEnabled(false);

    resetAct->setEnabled(false);
    resetButton->setEnabled(false);

    if (processed) {
        if (puzzleLayout && puzzleLayout != nullptr) {
            mainLayout->removeWidget(puzzleLayout);
            delete puzzleLayout;
            puzzleLayout = nullptr;
        }
    } else {
        if (imageViewer && imageViewer != nullptr) {
            delete imageViewer;
            imageViewer = nullptr;
        }
    }
    qDebug() << "got here";

    processed = false;

    if (placeholder) {
        if (placeholder->layout()) {
            QLayout *oldLayout = placeholder->layout();
            delete oldLayout;
        }

        QVBoxLayout *imageLayout = new QVBoxLayout(placeholder);
        placeholder->setLayout(imageLayout);
    }

    update();
}
