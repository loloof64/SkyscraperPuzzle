#include "gridsolver.h"
#include <QMap>

/////////////////////////
#include "algorithm"

void debugClues(QVector<QVector<QSet<int>>> clues)
{
    QMessageLogger logger;
    logger.debug("These are the clues ##################################");

    for (auto rowIt = clues.begin(); rowIt != clues.end(); rowIt++)
    {
        logger.debug("-------------------------------------");
        for (auto colIt = rowIt->begin(); colIt != rowIt->end(); colIt++)
        {
            QString message;
            QList<int> values(colIt->toList());
            std::sort(values.begin(), values.end());

            for (auto valuesIt = values.begin(); valuesIt != values.end(); valuesIt++)
            {
                message.append(QString::number(*valuesIt));
            }

            logger.debug(message.toUtf8());
        }
    }
}
////

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
    auto result = QVector<QVector<int>>();

    for (auto rowIt = this->internalClues.begin(); rowIt != this->internalClues.end(); rowIt++)
    {
        auto lineValues = QVector<int>();

        for (auto colIt = rowIt->begin(); colIt != rowIt->end(); colIt++)
        {
            if (colIt->size() == 1) lineValues.push_back(colIt->toList()[0]);
            else lineValues.push_back(0);
        }

        result.push_back(lineValues);
    }

    return result;
}

bool GridSolver::solve()
{
    this->resetInternalClues();

    bool reducingCluesChangedGrid = false;
    do {
        reducingCluesChangedGrid = this->reduceInternalCluesAndReturnIfChanged();
    } while(reducingCluesChangedGrid);

    return this->gridCompletelyReduced();
}

void GridSolver::resetInternalClues()
{
    this->internalClues.clear();

    for (auto j = 0; j < this->size; j++)
    {
        auto currentLine = QVector<QSet<int>>();

        for (auto i = 0; i < this->size; i++)
        {
            auto currentSet = QSet<int>();
            for (auto digit = 1; digit <= this->size; digit++)
            {
                currentSet.insert(digit);
            }
            currentLine.push_back(currentSet);
        }

        this->internalClues.push_back(currentLine);
    }
}

bool GridSolver::reduceInternalCluesAndReturnIfChanged()
{
    //////////////////
    //debugClues(this->internalClues);
    ///////////////////
    if (this->updateInternalCluesAndReturnIfChanged())
    {
        return true;
    }
    if (this->reduceCluesFromRowsAndColumnsAndReturnIfChanged())
    {
        return true;
    }
    if (this->findMaximumsAndReturnIfChanged())
    {
        return true;
    }
    if (this->reduceByVisibilityAndReturnIfChanged())
    {
        return true;
    }

    return false;
}

bool GridSolver::gridCompletelyReduced()
{
    auto completelyReducedCells = 0;

    for (auto rowIt = this->internalClues.begin(); rowIt != this->internalClues.end(); rowIt++)
    {
        for (auto colIt = rowIt->begin(); colIt != rowIt->end(); colIt++)
        {
            if (colIt->size() == 1) completelyReducedCells++;
        }
    }

    return completelyReducedCells == this->size * this->size;
}

bool GridSolver::updateInternalCluesAndReturnIfChanged()
{
    for (auto rowIt = this->internalClues.begin(); rowIt != this->internalClues.end(); rowIt++)
    {
        for (auto colIt = rowIt->begin(); colIt != rowIt->end(); colIt++)
        {
            // Is a found value
            if (colIt->size() == 1)
            {
                auto lonelyDigit = colIt->toList()[0];
                auto rowIndex = static_cast<int>(rowIt - this->internalClues.begin());
                auto colIndex = static_cast<int>(colIt - rowIt->begin());

                auto somethingChanged = false;

                ////////////////////////////////////////////////////////////////////////////////////
                QMessageLogger logger;
                QString message;
                //message.sprintf("Updating clues from (row = %d, col= %d)\n", rowIndex, colIndex);
                //logger.debug(message.toUtf8());
                ////////////////////////////////////////////////////////////////////////////////////

                for (auto col = 0; col < this->size; col++)
                {
                    if (col == colIndex) continue;
                    if (this->internalClues[rowIndex][col].remove(lonelyDigit)) somethingChanged = true;
                }

                for (auto row = 0; row < this->size; row++)
                {
                    if (row == rowIndex) continue;
                    if (this->internalClues[row][colIndex].remove(lonelyDigit)) somethingChanged = true;
                }

                ///////////////////////////////////////////////////////
                //debugClues(this->internalClues);
                ///////////////////////////////////////////////////////


                if (somethingChanged) return true;
            }
        }
    }
    return false;
}

bool GridSolver::reduceCluesFromRowsAndColumnsAndReturnIfChanged()
{
    auto cluesPresences = QMap<int, QString>();

    // Scanning rows
    for (int row = 0; row < this->size; row++)
    {
        cluesPresences.clear();
        for (int col = 0; col < this->size; col++)
        {
            auto values = this->internalClues[row][col].toList();
            bool isAlreadySolvedDigit = values.size() == 1;

            if (isAlreadySolvedDigit) continue;

            // Register all clues of the current column into presences of this line
            for (auto it = values.begin(); it != values.end(); it++)
            {
                cluesPresences[*it].append(QString::number(col));
            }
       }

        // Scanning this line presences for lonely digit
        for (auto it = cluesPresences.begin(); it != cluesPresences.end(); it++)
        {
            auto currentDigit = it.key();
            auto currentPresencesCols = it.value();

            bool hasLonelyCol = currentPresencesCols.length() == 1;
            if (hasLonelyCol)
            {
                ////////////////////////////////////////////////////////////////////////////////////
                QMessageLogger logger;
                QString message;
                //message.sprintf("Updating ROW ##LONELY %d## clue from (row = %d)\n", currentDigit, row);
                //logger.debug(message.toUtf8());
                ////////////////////////////////////////////////////////////////////////////////////


                auto lonelyDigitCol = currentPresencesCols.toInt();

                for (auto tempCol = 0; tempCol < this->size; tempCol++)
                {
                    if (tempCol == lonelyDigitCol)
                    {
                      // We only need to keep the lonely digit here
                      this->internalClues[row][tempCol].clear();
                      this->internalClues[row][tempCol].insert(currentDigit);
                    }
                    else {
                     this->internalClues[row][tempCol].remove(currentDigit);
                    }
                }

                /////////////////////
                //debugClues(this->internalClues);
                /////////////////////

                return true;
            }
        }
    }

    // Scanning columns
    for (int col = 0; col < this->size; col++)
    {
        cluesPresences.clear();
        for (int row = 0; row < this->size; row++)
        {
            auto values = this->internalClues[row][col].toList();
            bool isAlreadySolvedDigit = values.size() == 1;

            if (isAlreadySolvedDigit) continue;

            // Register all clues of the current line into presences of this column
            for (auto it = values.begin(); it != values.end(); it++)
            {
                cluesPresences[*it].append(QString::number(row));
            }
        }

        // Scanning this column presences for lonely digit
        for (auto it = cluesPresences.begin(); it != cluesPresences.end(); it++)
        {
            auto currentDigit = it.key();
            auto currentPresencesLines = it.value();

            bool hasLonelyLine = currentPresencesLines.length() == 1;
            if (hasLonelyLine)
            {
                ////////////////////////////////////////////////////////////////////////////////////
                QMessageLogger logger;
                QString message;
                //message.sprintf("Updating COLUMN ##LONELY %d## clue from (col= %d)\n", currentDigit, col);
                //logger.debug(message.toUtf8());
                ////////////////////////////////////////////////////////////////////////////////////

                auto lonelyDigitLine = currentPresencesLines.toInt();

                for (auto tempRow = 0; tempRow < this->size; tempRow++)
                {
                    if (tempRow == lonelyDigitLine)
                    {
                        // We only need to keep the lonely digit here
                        this->internalClues[tempRow][col].clear();
                        this->internalClues[tempRow][col].insert(currentDigit);
                    }
                    else {
                        this->internalClues[tempRow][col].remove(currentDigit);
                    }
                }

                /////////////////////
                //debugClues(this->internalClues);
                /////////////////////

                return true;
            }
        }
    }

    return false;
}

bool GridSolver::findMaximumsAndReturnIfChanged()
{
    bool somethingChanged = false;

    // Scanning top clues
    for (auto it = this->topClues.begin(); it != this->topClues.end(); it++)
    {
        auto currentClue = *it;
        auto clueIndex = static_cast<int>(it - this->topClues.begin());

        auto isBlankClue = currentClue == 0;
        if (isBlankClue) continue;

        // Trying to update clues of related column
        for (int i = 0; i < this->size; i++)
        {
            auto currentCellValues = this->internalClues[i][clueIndex].toList();
            auto maximumClue = this->size - currentClue + i + 1;
            for (int testedClue = maximumClue + 1; testedClue <= this->size; testedClue++)
            {
                if (currentCellValues.contains(testedClue)){
                    somethingChanged = true;
                    this->internalClues[i][clueIndex].remove(testedClue);
                }
            }
        }
    }

    // Scanning bottom clues
    for (auto it = this->bottomClues.begin(); it != this->bottomClues.end(); it++)
    {
        auto currentClue = *it;
        auto clueIndex = static_cast<int>(it - this->bottomClues.begin());

        auto isBlankClue = currentClue == 0;
        if (isBlankClue) continue;

        // Trying to update clues of related column
        for (int i = 0; i < this->size; i++)
        {
            auto currentCellValues = this->internalClues[this->size - 1 - i][clueIndex].toList();
            auto maximumClue = this->size - currentClue + i + 1;
            for (int testedClue = maximumClue + 1; testedClue <= this->size; testedClue++)
            {
                if (currentCellValues.contains(testedClue)){
                    somethingChanged = true;
                    this->internalClues[this->size - 1 - i][clueIndex].remove(testedClue);
                }
            }
        }
    }

    // Scanning left clues
    for (auto it = this->leftClues.begin(); it != this->leftClues.end(); it++)
    {
        auto currentClue = *it;
        auto clueIndex = static_cast<int>(it - this->leftClues.begin());

        auto isBlankClue = currentClue == 0;
        if (isBlankClue) continue;

        // Trying to update clues of related line
        for (int i = 0; i < this->size; i++)
        {
            auto currentCellValues = this->internalClues[clueIndex][i].toList();
            auto maximumClue = this->size - currentClue + i + 1;
            for (int testedClue = maximumClue + 1; testedClue <= this->size; testedClue++)
            {
                if (currentCellValues.contains(testedClue)){
                    somethingChanged = true;
                    this->internalClues[clueIndex][i].remove(testedClue);
                }
            }
        }
    }

    // Scanning right clues
    for (auto it = this->rightClues.begin(); it != this->rightClues.end(); it++)
    {
        auto currentClue = *it;
        auto clueIndex = static_cast<int>(it - this->rightClues.begin());

        auto isBlankClue = currentClue == 0;
        if (isBlankClue) continue;

        // Trying to update clues of related line
        for (int i = 0; i < this->size; i++)
        {
            auto currentCellValues = this->internalClues[clueIndex][this->size - 1 - i].toList();
            auto maximumClue = this->size - currentClue + i + 1;
            for (int testedClue = maximumClue + 1; testedClue <= this->size; testedClue++)
            {
                if (currentCellValues.contains(testedClue)){
                    somethingChanged = true;
                    this->internalClues[clueIndex][this->size - 1 - i].remove(testedClue);
                }
            }
        }
    }

    /////////////////////////////////////
    QMessageLogger logger;
    logger.debug("Tried to reduce with maximums");
    //debugClues(this->internalClues);
    /////////////////////////////////////
    return somethingChanged;
}

bool GridSolver::reduceByVisibilityAndReturnIfChanged()
{
    bool somethingChanged = false;

    //////////////////////////////////
    QMessageLogger logger;
    //////////////////////////////////

    /////////////////////////////
    debugClues(this->internalClues);
    //////////////////////////////

    // Scanning top clues
    for (auto columnIt = this->topClues.begin(); columnIt != this->topClues.end(); columnIt++)
    {
        auto currentClue = *columnIt;
        auto clueIndex = static_cast<int>(columnIt - this->topClues.begin());

        auto isBlankClue = currentClue == 0;
        if (isBlankClue) continue;

        auto lineConfigurationToReduce = QVector<QSet<int>>();
        for (auto row = 0; row < this->size; row++)
        {
            lineConfigurationToReduce.push_back(this->internalClues[row][clueIndex]);
        }

        if (reduceLineConfigurationAndReturnIfChanged(currentClue, lineConfigurationToReduce))
        {
            // Updating the column clues
            for (auto row = 0; row < this->size; row++)
            {
                this->internalClues[row][clueIndex] = lineConfigurationToReduce[row];
            }

            somethingChanged = true;

            ///////////////////////////////////////
            logger.debug("Reduced from top clue");
            debugClues(this->internalClues);
            ///////////////////////////////////////
        }

    }

    ///////////////////////////////////////
    logger.debug("After top clues reduction");
    debugClues(this->internalClues);
    ///////////////////////////////////////

    // Scanning bottom clues
    for (auto columnIt = this->bottomClues.begin(); columnIt != this->bottomClues.end(); columnIt++)
    {
        auto currentClue = *columnIt;
        auto clueIndex = static_cast<int>(columnIt - this->bottomClues.begin());

        auto isBlankClue = currentClue == 0;
        if (isBlankClue) continue;

        auto lineConfigurationToReduce = QVector<QSet<int>>();
        for (auto row = 0; row < this->size; row++)
        {
            lineConfigurationToReduce.push_back(this->internalClues[this->size - 1 - row][clueIndex]);
        }

        if (reduceLineConfigurationAndReturnIfChanged(currentClue, lineConfigurationToReduce))
        {
            // Updating the column clues
            for (auto row = 0; row < this->size; row++)
            {
                this->internalClues[this->size - 1 - row][clueIndex] = lineConfigurationToReduce[row];
            }
            somethingChanged = true;

            ///////////////////////////////////////
            logger.debug("Reduced from bottom clue");
            debugClues(this->internalClues);
            ///////////////////////////////////////
        }

    }

    ///////////////////////////////////////
    logger.debug("After bottom clues reduction");
    debugClues(this->internalClues);
    ///////////////////////////////////////


    // Scanning left clues
    for (auto rowIt = this->leftClues.begin(); rowIt != this->leftClues.end(); rowIt++)
    {
        auto currentClue = *rowIt;
        auto clueIndex = static_cast<int>(rowIt - this->leftClues.begin());

        auto isBlankClue = currentClue == 0;
        if (isBlankClue) continue;

        auto lineConfigurationToReduce = QVector<QSet<int>>();
        for (auto col = 0; col < this->size; col++)
        {
            lineConfigurationToReduce.push_back(this->internalClues[clueIndex][col]);
        }

        if (reduceLineConfigurationAndReturnIfChanged(currentClue, lineConfigurationToReduce))
        {
            // Updating the line clues
            for (auto col = 0; col < this->size; col++)
            {
                this->internalClues[clueIndex][col] = lineConfigurationToReduce[col];
            }
            somethingChanged = true;

            ///////////////////////////////////////
            logger.debug("Reduced from left clue");
            debugClues(this->internalClues);
            ///////////////////////////////////////
        }

    }

    ///////////////////////////////////////
    logger.debug("After left clues reduction");
    debugClues(this->internalClues);
    ///////////////////////////////////////


    // Scanning right clues
    for (auto rowIt = this->rightClues.begin(); rowIt != this->rightClues.end(); rowIt++)
    {
        auto currentClue = *rowIt;
        auto clueIndex = static_cast<int>(rowIt - this->rightClues.begin());

        auto isBlankClue = currentClue == 0;
        if (isBlankClue) continue;

        auto lineConfigurationToReduce = QVector<QSet<int>>();
        for (auto col = 0; col < this->size; col++)
        {
            lineConfigurationToReduce.push_back(this->internalClues[clueIndex][this->size - 1 - col]);
        }

        if (reduceLineConfigurationAndReturnIfChanged(currentClue, lineConfigurationToReduce))
        {
            // Updating the line clues
            for (auto col = 0; col < this->size; col++)
            {
                this->internalClues[clueIndex][this->size - 1 - col] = lineConfigurationToReduce[col];
            }
            somethingChanged = true;

            ///////////////////////////////////////
            logger.debug("Reduced from right clue");
            debugClues(this->internalClues);
            ///////////////////////////////////////
        }

    }

    //////////////////////////////////////
    logger.debug("After right clues reduction");
    debugClues(this->internalClues);
    ///////////////////////////////////////


    ////////////////////////////////////
    //logger.debug("Tried to reduce with visibility rule");
    //debugClues(this->internalClues);
    /////////////////////////////////////
    return somethingChanged;
}

bool GridSolver::testReductionApplicationInLineConfigurationByVisibilityAndReturnIfGood(int visibilityTarget,
                                                                                        int testIndexInConfiguration,
                                                                                        int testDigit,
                                                                                        QVector<QSet<int>> configuration)
{
    ////////////////////////////////
    QMessageLogger logger;

    QString localMessage;
    localMessage.sprintf("Reducing with visibilityTarget = %d, testIndex = %d, testDigit = %d", visibilityTarget, testIndexInConfiguration, testDigit);
    logger.debug(localMessage.toUtf8());

    logger.info("---Before local reduction --------");
    for (auto tempIt = configuration.begin(); tempIt != configuration.end(); tempIt++)
    {
        auto message = QString();
        auto values = tempIt->toList();
        std::sort(values.begin(), values.end());
        for (auto temp2It = values.begin(); temp2It != values.end(); temp2It++)
        {
            message.append(QString::number(*temp2It));
        }
        logger.info(message.toUtf8());
    }
    ////////////////////////////////

    // Removing requested digit from all cells
    for (auto it = configuration.begin(); it != configuration.end(); it++)
    {
        it->remove(testDigit);
    }

    // We only have to keep the tested digit here
    configuration[testIndexInConfiguration].clear();
    configuration[testIndexInConfiguration].insert(testDigit);

    // Checking that the reduction is good
    // Otherwise that means that the digit can't be accepted here
    for (auto it = configuration.begin(); it != configuration.end(); it++)
    {
        if (it->size() == 0) return false;
    }

    auto configurationIsFullyDeduced = true;

    ////////////////////////////
    logger.info("----After local reduction ---");
    for (auto tempIt = configuration.begin(); tempIt != configuration.end(); tempIt++)
    {
        auto message = QString();
        auto values = tempIt->toList();
        std::sort(values.begin(), values.end());
        for (auto temp2It = values.begin(); temp2It != values.end(); temp2It++)
        {
            message.append(QString::number(*temp2It));
        }
        logger.info(message.toUtf8());
    }
    ////////////////////////////

    for (auto it = configuration.begin(); it != configuration.end(); it++)
    {
        if (it->size() > 1) configurationIsFullyDeduced = false;
    }

    if (configurationIsFullyDeduced)
    {
        auto visibleBuildingsCount = 0;
        auto lastVisibleBuildingSize = 0;

        for (auto it = configuration.begin(); it != configuration.end(); it++)
        {
            if (1 == it->size())
            {
                auto currentBuildingSize = it->toList()[0];
                if (currentBuildingSize > lastVisibleBuildingSize)
                {
                    lastVisibleBuildingSize = currentBuildingSize;
                    visibleBuildingsCount++;
                }
            }
        }

        bool isGoodTestedDigit = visibilityTarget == visibleBuildingsCount;

        ////////////////////////////////
        QString message;
        message.sprintf("So, is digit %d good ? %d (targetVisibility = %d)", testDigit, isGoodTestedDigit, visibilityTarget);
        logger.debug(message.toUtf8());
        ////////////////////////////////

        return isGoodTestedDigit;
    }
    else // Configuration is still partially deduced
    {
        // Searching for first not deduced cell
        auto cellToDeducePointer = configuration.begin();
        for (; cellToDeducePointer != configuration.end(); cellToDeducePointer++)
        {
            if (cellToDeducePointer->size() == 1) break;
        }

        auto cellToDeduceIndex = static_cast<int>(cellToDeducePointer - configuration.begin());
        assert(cellToDeduceIndex < configuration.size());

        for (auto digitIt = cellToDeducePointer->begin(); digitIt != cellToDeducePointer->end(); digitIt++)
        {
            auto digitIsGood = testReductionApplicationInLineConfigurationByVisibilityAndReturnIfGood(
               visibilityTarget, cellToDeduceIndex, *digitIt, configuration
            );
            if (digitIsGood) return true;
        }

        // Here no tested digit could be approved
        return false;
    }
}

bool GridSolver::reduceLineConfigurationAndReturnIfChanged(int visibilityTarget, QVector<QSet<int>> &lineConfigurationToReduce)
{
    bool somethingChanged = false;

    for (auto cellIndex = 0; cellIndex < lineConfigurationToReduce.size(); cellIndex++)
    {
        auto currentCell = lineConfigurationToReduce[cellIndex];
        auto cellAlreadyDeduced = currentCell.size() == 1;
        if (cellAlreadyDeduced) continue;

        for (auto valueIt = currentCell.begin(); valueIt != currentCell.end(); valueIt++)
        {

            auto valueIsGood = testReductionApplicationInLineConfigurationByVisibilityAndReturnIfGood(
                visibilityTarget, cellIndex, *valueIt, lineConfigurationToReduce
            );

            if ( ! valueIsGood )
            {
                lineConfigurationToReduce[cellIndex].remove(*valueIt);
                somethingChanged = true;
            }
        }
    }

    return somethingChanged;
}
