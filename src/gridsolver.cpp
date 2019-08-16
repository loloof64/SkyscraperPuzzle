#include "gridsolver.h"
#include <QMap>

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

QVector<QVector<QVector<int>>> GridSolver::solve()
{
    currentSolutions.clear();
    solveIteration(getEmptyGridState());
    return currentSolutions;
}

void GridSolver::solveIteration(QVector<QVector<int>> gridState, int globalCellIndex)
{
    int rowIndex = globalCellIndex / size;
    int colIndex = globalCellIndex % size;

    auto digitAlreadyDefined = gridState[rowIndex][colIndex] > 0;
    if (digitAlreadyDefined) return;

    for (auto digit = 1; digit <= size; digit++)
    {
        if (checkIfDigitAlreadyInLineOrColumn(gridState, digit, rowIndex, colIndex)) continue;
        gridState[rowIndex][colIndex] = digit;

        auto gridFilled = globalCellIndex == (size*size - 1);

        if (gridFilled)
        {
            auto isASolution = checkIfIsASolution(gridState);
            if (isASolution)
            {
                currentSolutions.push_back(gridState);
            }
            return;
        }
        else
        {
            solveIteration(gridState, globalCellIndex+1);
        }
    }
}

QVector<QVector<int>> GridSolver::getEmptyGridState()
{
    auto gridState = QVector<QVector<int>>();
    for (auto rowIndex = 0; rowIndex < size; rowIndex++)
    {
        auto currentLine = QVector<int>();
        for (auto colIndex = 0; colIndex < size; colIndex++)
        {
            currentLine.push_back(0);
        }
        gridState.push_back(currentLine);
    }
    return gridState;
}

bool GridSolver::checkIfDigitAlreadyInLineOrColumn(const QVector<QVector<int>> &gridState,
                                            int digitToTest, int rowIndex, int colIndex) const
{
    // Scanning line
    for (auto scannedColIndex = 0; scannedColIndex < size; scannedColIndex++)
    {
        if (gridState[rowIndex][scannedColIndex] == digitToTest) return true;
    }

    // Scanning column
    for (auto scannedRowIndex = 0; scannedRowIndex < size; scannedRowIndex++)
    {
        if (gridState[scannedRowIndex][colIndex] == digitToTest) return true;
    }

    return false;
}

bool GridSolver::checkIfIsASolution(QVector<QVector<int>> gridState) const
{

    // Scannning top clues
    for (auto topClueIndex = 0; topClueIndex < size; topClueIndex++)
    {
        auto currentClue = topClues[topClueIndex];

        bool weMustSkipClue = currentClue == 0;
        if (weMustSkipClue) continue;

        auto matchingConfiguration = QVector<int>();
        for (auto rowIndex = 0; rowIndex < size; rowIndex++)
        {
            matchingConfiguration.push_back(gridState[rowIndex][topClueIndex]);
        }

        if ( ! checkIfGoodConfiguration(currentClue, matchingConfiguration) )
        {
            return false;
        }
    }

    // Scanning bottom clues
    for (auto bottomClueIndex = 0; bottomClueIndex < size; bottomClueIndex++)
    {
        auto currentClue = bottomClues[bottomClueIndex];

        bool weMustSkipClue = currentClue == 0;
        if (weMustSkipClue) continue;

        auto matchingConfiguration = QVector<int>();
        for (auto rowIndex = 0; rowIndex < size; rowIndex++)
        {
            matchingConfiguration.push_back(gridState[size - 1 - rowIndex][bottomClueIndex]);
        }

        if ( ! checkIfGoodConfiguration(currentClue, matchingConfiguration) )
        {
            return false;
        }
    }

    // Scanning left clues
    for (auto leftClueIndex = 0; leftClueIndex < size; leftClueIndex++)
    {
        auto currentClue = leftClues[leftClueIndex];

        bool weMustSkipClue = currentClue == 0;
        if (weMustSkipClue) continue;

        auto matchingConfiguration = QVector<int>();
        for (auto colIndex = 0; colIndex < size; colIndex++)
        {
            matchingConfiguration.push_back(gridState[leftClueIndex][colIndex]);
        }

        if ( ! checkIfGoodConfiguration(currentClue, matchingConfiguration) )
        {
            return false;
        }
    }

    // Scanning right clues
    for (auto rightClueIndex = 0 ; rightClueIndex < size; rightClueIndex++)
    {
        auto currentClue = rightClues[rightClueIndex];

        bool weMustSkipClue = currentClue == 0;
        if (weMustSkipClue) continue;

        auto matchingConfiguration = QVector<int>();
        for (auto colIndex = 0; colIndex < size; colIndex++)
        {
            matchingConfiguration.push_back(gridState[rightClueIndex][size - 1 -colIndex]);
        }

        if ( ! checkIfGoodConfiguration(currentClue, matchingConfiguration) )
        {
            return false;
        }
    }

    return true;
}

bool GridSolver::checkIfGoodConfiguration(int expectedVisiblesBuildings, QVector<int> configuration) const
{
    auto visibleBuildings = 0;
    auto lastHighestBuilding = 0;

    for (auto it = configuration.begin(); it != configuration.end(); it++)
    {
        auto currentBuilding = *it;
        if (currentBuilding > lastHighestBuilding)
        {
            lastHighestBuilding = currentBuilding;
            visibleBuildings++;
        }
    }

    auto isGoodConfiguration = visibleBuildings == expectedVisiblesBuildings;
    return isGoodConfiguration;
}
