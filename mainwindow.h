#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "puzzlesolverlayout.h"
#include "interactivepiece.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT
    QString lastDir;
    QImage *processedImage;
    PuzzleSolverLayout *puzzleLayout;
    QPoint *orgImageCenter;

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    bool isShadeOfWhite(const QRgb &color);
    bool isSurroundedRed(int pixelX, int pixelY);
    bool isSurroundedBlack(int pixelX, int pixelY);

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
