#include "puzzlesolverlayout.h"

#include <QtWidgets>
#include <imageviewer.h>

PuzzleSolverLayout::PuzzleSolverLayout(const QImage &_image):image(_image) {

    // LAYOUTS
    // set the vertical box
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    setLayout(mainLayout);

    // add the ImageViewer to the top
    ImageViewer *imageViewer = new ImageViewer(image);
    mainLayout->addWidget(imageViewer);

    // add QGraphicsView to the bottom
    QGraphicsView *solverInterface = new QGraphicsView();
    mainLayout->addWidget(solverInterface);
}
