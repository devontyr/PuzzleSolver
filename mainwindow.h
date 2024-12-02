#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "puzzlesolverlayout.h"
#include "interactivepiece.h"

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

extern MainWindow *mainWindow;

class Waiter {
    static int count;
public:
    Waiter();
    ~Waiter();
};
#endif // MAINWINDOW_H
