#ifndef GRIDCOMPONENT_H
#define GRIDCOMPONENT_H

#include <QVector>
#include <QWidget>
#include <QGridLayout>
#include <QKeyEvent>
#include "gridcell.h"
#include "gridsolver.h"

enum class GameMode {
    Playing,
    SolvingPreparation,
    SolvingProcess,
    SolvingDone,
};

class GridComponent : public QWidget
{
    Q_OBJECT
public:
    explicit GridComponent(QWidget *parent = nullptr);
    ~GridComponent() override;
    void configureAsSolver(int size);

public slots:
    void updateSelectedCell(GridCellId id);
    void solve();

protected:
    QSize sizeHint() const override;
    void keyReleaseEvent(QKeyEvent *event) override;

private:
    int sideCellsCount = 0;
    GridCell *selectedCell = nullptr;
    GridSolver *solver = nullptr;
    GameMode gameMode = GameMode::SolvingPreparation;
    bool tellIfCellCandidateForSelection(GridCellId id);
    QVector<QVector<GridCell *> *> *gameCells = nullptr;
    QVector<GridCell *> *topCluesCells = nullptr; // from left to right
    QVector<GridCell *> *bottomCluesCells = nullptr; // from left to right
    QVector<GridCell *> *leftCluesCells = nullptr; // from top to bottom
    QVector<GridCell *> *rightCluesCells = nullptr; // from top to bottom
    QGridLayout *layout = nullptr;
    void initializeCells();
    void cleanUpCells();
    GridCell *findReferenceCell(GridCellId id) const;
    QVector<int> getDigitsFromCells(QVector<GridCell *> *cellsLine) const;
};

#endif // GRIDCOMPONENT_H
