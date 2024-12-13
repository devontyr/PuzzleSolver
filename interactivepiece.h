#ifndef INTERACTIVEPIECE_H
#define INTERACTIVEPIECE_H

#include <QGraphicsView>
#include <QImage>
#include <QtWidgets>

class interactivePiece : public QGraphicsView {
    Q_OBJECT

public:
    explicit interactivePiece(const QList<QImage> &pieces, QWidget *parent = nullptr);

protected:
    void keyPressEvent(QKeyEvent *evt) override;
    void mousePressEvent(QMouseEvent *evt) override;
    void mouseReleaseEvent(QMouseEvent *evt) override;
    void addPiece(const QImage &image, const QPointF &position, double rotation, double scale);


signals:
    void emitSave(const QString &serializedData);

public slots:
    void saveDataSlot();
    void loadData(const QByteArray &data);

private:
    QGraphicsScene *scene;
    int currentZ;
    QGraphicsItem *lastSelectedItem;
    double scaleF;
    double snapDistance;
    double pieceScan;
    QMap<QGraphicsItem*, QPointF> originalPositions;
};


#endif // INTERACTIVEPIECE_H
