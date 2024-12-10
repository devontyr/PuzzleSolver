#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "puzzlesolverlayout.h"
#include "interactivepiece.h"
#include "imageviewer.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT
    QString lastDir;
    PuzzleSolverLayout *puzzleLayout;
    QImage processedImage;
    QPoint orgImageCenter;
    interactivePiece* interactivePieceLayout;
    QHBoxLayout* layout;
    QHBoxLayout *mainLayout;
    QScrollArea *scrollArea;
    QWidget *placeholder;
    ImageViewer *imageViewer;

    QAction *addImageAct;
    QPushButton *addImageButton;

    QAction *processAct;
    QPushButton *processButton;

    QAction *solvePuzzleAct;
    QPushButton *solveButton;

    QAction *hintAct;
    QPushButton *hintButton;

    QAction *resetAct;
    QPushButton *resetButton;
    bool processed;


public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

public slots:
    void openImageSlot();
    void addImageSlot();
    void processSlot();
    void solveSlot();
    void hintSlot();
    void resetSlot();

};

#endif // MAINWINDOW_H
