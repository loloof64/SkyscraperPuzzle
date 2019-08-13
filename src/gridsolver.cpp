#include "gridsolver.h"

GridSolver::GridSolver()
{

}

void GridSolver::setParameters(
        int size,
        QVector<int> topClues,
        QVector<int> bottomClues,
        QVector<int> leftClues,
        QVector<int> rightClues
)
{
    assert(topClues.size() == size);
    assert(bottomClues.size() == size);
    assert(leftClues.size() == size);
    assert(rightClues.size() == size);

    this->size = size;
    this->topClues = topClues;
    this->bottomClues = bottomClues;
    this->leftClues = leftClues;
    this->rightClues = rightClues;
}

QVector<QVector<int>> GridSolver::getValues() const
{
    return this->values;
}

bool GridSolver::solve()
{
    bool success = false;


    return success;
}
