#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "imageprocess.h"
#include "interactivepiece.h"
#include "imageviewer.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT
    QString lastDir;
    ImageProcess *puzzleLayout;
    QImage processedImage;
    QPoint orgImageCenter;
    QHBoxLayout* layout;
    QHBoxLayout *mainLayout;
    QScrollArea *scrollArea;
    QWidget *placeholder;
    ImageViewer *imageViewer;

    QAction *saveAct;
    QAction *loadSaveAct;

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
    void saveSlot();
    void loadSaveSlot();

};

#endif // MAINWINDOW_H
