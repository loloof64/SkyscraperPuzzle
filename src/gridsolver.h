#ifndef GRIDSOLVER_H
#define GRIDSOLVER_H

#include <QVector>
#include <QSet>
#include <QStack>

class GridSolver
{
public:
    GridSolver();
    void setParameters(
            int size,
            QVector<int> topClues,
            QVector<int> bottomClues,
            QVector<int> leftClues,
            QVector<int> rightClues
    );

    QVector<QVector<QVector<int>>> solve();

private:
    int size = 0;
    QVector<int> topClues;
    QVector<int> bottomClues;
    QVector<int> leftClues;
    QVector<int> rightClues;
    QVector<QVector<QVector<int>>> currentSolutions;

    QVector<QVector<int>> getEmptyGridState();
    void solveIteration(QVector<QVector<int>> gridState, int globalCellIndex = 0);
    bool checkIfDigitAlreadyInLineOrColumn(QVector<QVector<int>> const &gridState,
                                    int digitToTest, int rowIndex, int colIndex) const;
    bool checkIfIsASolution(QVector<QVector<int>> gridState) const;
    bool checkIfGoodConfiguration(int expectedVisiblesBuildings, QVector<int> configuration) const;
};

#endif // GRIDSOLVER_H
