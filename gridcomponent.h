#ifndef GRIDCOMPONENT_H
#define GRIDCOMPONENT_H

#include <QVector>
#include <QWidget>
#include <QGridLayout>
#include "gridcell.h"

using GridCellVector = QVector<Gridcell *> *;

class GridComponent : public QWidget
{
    Q_OBJECT
public:
    explicit GridComponent(QWidget *parent = nullptr);
    ~GridComponent();

signals:

public slots:
private:
    int sideCellsCount = 4;
    QVector<GridCellVector> *gameCells;
    QVector<Gridcell *> *topCluesCells; // from left to right
    QVector<Gridcell *> *bottomCluesCells; // from left to right
    QVector<Gridcell *> *leftCluesCells; // from top to bottom
    QVector<Gridcell *> *rightCluesCells; // from top to bottom
    QGridLayout *layout;
    QSize sizeHint() const;
    void paintEvent(QPaintEvent* event);
    void initializeCells();
    void cleanUpCells();
};

#endif // GRIDCOMPONENT_H
