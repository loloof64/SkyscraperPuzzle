#ifndef GRIDCOMPONENT_H
#define GRIDCOMPONENT_H

#include <QVector>
#include <QWidget>
#include <QGridLayout>
#include <QKeyEvent>
#include "gridcell.h"

class GridComponent : public QWidget
{
    Q_OBJECT
public:
    explicit GridComponent(QWidget *parent = nullptr);
    ~GridComponent() override;

public slots:
    void updateSelectedCell(GridCellId id);

protected:
    QSize sizeHint() const override;
    void keyReleaseEvent(QKeyEvent *event) override;

private:
    int sideCellsCount = 4;
    GridCell* selectedCell = nullptr;
    QVector<QVector<GridCell *> *> *gameCells = nullptr;
    QVector<GridCell *> *topCluesCells = nullptr; // from left to right
    QVector<GridCell *> *bottomCluesCells = nullptr; // from left to right
    QVector<GridCell *> *leftCluesCells = nullptr; // from top to bottom
    QVector<GridCell *> *rightCluesCells = nullptr; // from top to bottom
    QGridLayout *layout = nullptr;
    void initializeCells();
    void cleanUpCells();
    GridCell *findReferenceCell(GridCellId id) const;
};

#endif // GRIDCOMPONENT_H
