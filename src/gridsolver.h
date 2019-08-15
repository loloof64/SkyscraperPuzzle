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
    bool solve();
    QVector<QVector<int>> getValues() const;

private:
    int size = 0;
    QVector<int> topClues;
    QVector<int> bottomClues;
    QVector<int> leftClues;
    QVector<int> rightClues;
    QVector<QVector<QSet<int>>> internalClues;

    void resetInternalClues();
    bool reduceInternalCluesAndReturnIfChanged();
    bool gridCompletelyReduced();

    bool updateInternalCluesAndReturnIfChanged();
    bool reduceCluesFromRowsAndColumnsAndReturnIfChanged();
    bool findMaximumsAndReturnIfChanged();
    bool reduceByVisibilityAndReturnIfChanged();

    bool testReductionApplicationInLineConfigurationByVisibilityAndReturnIfGood(int visibilityTarget, int testIndexInConfiguration,
                                                             int testDigit, QVector<QSet<int>> configuration);
    bool reduceLineConfigurationAndReturnIfChanged(int visibilityTarget, QVector<QSet<int>> &lineConfigurationToReduce);
};

#endif // GRIDSOLVER_H
