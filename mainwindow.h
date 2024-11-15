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
    // interactivePiece* interactivePieceLayout;

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

public slots:
    void openImageSlot();

};

extern MainWindow *mainWindow;

class Waiter {
    static int count;
public:
    Waiter();
    ~Waiter();
};
#endif // MAINWINDOW_H
