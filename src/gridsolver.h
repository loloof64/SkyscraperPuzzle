#ifndef GRIDSOLVER_H
#define GRIDSOLVER_H

#include <QVector>

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
    bool solve();
    QVector<QVector<int>> getValues() const;

private:
    int size = 0;
    QVector<QVector<int>> values;
    QVector<int> topClues;
    QVector<int> bottomClues;
    QVector<int> leftClues;
    QVector<int> rightClues;
};

#endif // GRIDSOLVER_H
