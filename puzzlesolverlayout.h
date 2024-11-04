#ifndef PUZZLESOLVERLAYOUT_H
#define PUZZLESOLVERLAYOUT_H

#include <QtWidgets>

class PuzzleSolverLayout: public QWidget
{
    Q_OBJECT
    QImage image;

public:
    explicit PuzzleSolverLayout(const QImage &_image);
};

#endif // PUZZLESOLVERLAYOUT_H
