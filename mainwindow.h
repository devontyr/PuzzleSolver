#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "puzzlesolverlayout.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT
    QString lastDir;
    PuzzleSolverLayout *puzzleLayout;
    QImage *processedImage;
    QPoint *orgImageCenter;

    bool isShadeOfWhite(const QRgb &color);
    QImage* floodFill(QImage* image);

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

public slots:
    void openImageSlot();

};
#endif // MAINWINDOW_H
