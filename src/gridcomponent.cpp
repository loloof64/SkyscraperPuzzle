#include "gridcomponent.h"
#include "gridcellid.h"
#include <QMessageBox>

GridComponent::GridComponent(QWidget *parent) : QWidget(parent)
{
    this->layout = new QGridLayout();
    this->setLayout(this->layout);
    this->initializeCells();
    this->setSizePolicy(QSizePolicy(QSizePolicy::Policy::Preferred, QSizePolicy::Policy::Preferred));
    this->solver = new GridSolver();
    this->grabKeyboard();
}

GridComponent::~GridComponent()
{
    delete this->solver;
    this->cleanUpCells();
    delete this->layout;
}

QSize GridComponent::sizeHint() const
{
    const int sideSize = (this->sideCellsCount + 2) * SIZE;
    return QSize(sideSize, sideSize);
}

void GridComponent::initializeCells()
{
    this->gameCells = new QVector<QVector<GridCell *> *>();
    for (int j = 0; j < this->sideCellsCount; j++)
    {
        QVector<GridCell *> *currentLine = new QVector<GridCell *>();
        for (int i = 0; i < this->sideCellsCount; i++)
        {
            auto currentCell = new GridCell(
                        GridCellId(GridCellIdType::Game, j*this->sideCellsCount + i),
            this);
            QObject::connect(currentCell, SIGNAL(notifyCellSelected(GridCellId)), this, SLOT(updateSelectedCell(GridCellId)));
            currentLine->push_back(currentCell);
            this->layout->addWidget(currentCell, j+1, i+1);
        }
        this->gameCells->push_back(currentLine);
    }

    this->topCluesCells = new QVector<GridCell *>();
    for (int i = 0; i < this->sideCellsCount; i++)
    {
        auto currentCell = new GridCell(GridCellId(GridCellIdType::ClueTop, i), this);
        QObject::connect(currentCell, SIGNAL(notifyCellSelected(GridCellId)), this, SLOT(updateSelectedCell(GridCellId)));
        this->topCluesCells->push_back(currentCell);
        this->layout->addWidget(currentCell, 0, i+1);
    }

    this->bottomCluesCells = new QVector<GridCell *>();
    for (int i = 0; i < this->sideCellsCount; i++)
    {
        auto currentCell = new GridCell(GridCellId(GridCellIdType::ClueBottom, i), this);
        QObject::connect(currentCell, SIGNAL(notifyCellSelected(GridCellId)), this, SLOT(updateSelectedCell(GridCellId)));
        this->bottomCluesCells->push_back(currentCell);
        this->layout->addWidget(currentCell, this->sideCellsCount + 1, i+1);
    }

    this->leftCluesCells = new QVector<GridCell *>();
    for (int i = 0; i < this->sideCellsCount; i++)
    {
        auto currentCell = new GridCell(GridCellId(GridCellIdType::ClueLeft, i), this);
        QObject::connect(currentCell, SIGNAL(notifyCellSelected(GridCellId)), this, SLOT(updateSelectedCell(GridCellId)));
        this->leftCluesCells->push_back(currentCell);
        this->layout->addWidget(currentCell, i+1, 0);
    }

    this->rightCluesCells = new QVector<GridCell *>();
    for (int i = 0; i < this->sideCellsCount; i++)
    {
        auto currentCell = new GridCell(GridCellId(GridCellIdType::ClueRight, i), this);
        QObject::connect(currentCell, SIGNAL(notifyCellSelected(GridCellId)), this, SLOT(updateSelectedCell(GridCellId)));
        this->rightCluesCells->push_back(currentCell);
        this->layout->addWidget(currentCell, i+1, this->sideCellsCount + 1);
    }
}

void GridComponent::cleanUpCells()
{
    for (auto outer_it = this->gameCells->begin(); outer_it != this->gameCells->end(); outer_it++)
    {
        QVector<GridCell *> *current_line = *outer_it;
        for (auto inner_it = current_line->begin(); inner_it != current_line->end(); inner_it++)
        {
            delete *inner_it;
        }
        delete *outer_it;
    }
    delete this->gameCells;

    for (auto it = this->topCluesCells->begin(); it != this->topCluesCells->end(); it++)
    {
        delete *it;
    }
    delete this->topCluesCells;

    for (auto it = this->bottomCluesCells->begin(); it != this->bottomCluesCells->end(); it++)
    {
        delete *it;
    }
    delete this->bottomCluesCells;

    for (auto it = this->leftCluesCells->begin(); it != this->leftCluesCells->end(); it++)
    {
        delete *it;
    }
    delete this->leftCluesCells;

    for (auto it = this->rightCluesCells->begin(); it != this->rightCluesCells->end(); it++)
    {
        delete *it;
    }
    delete this->rightCluesCells;
}

void GridComponent::updateSelectedCell(GridCellId id)
{
    bool isValidCellType = tellIfCellCandidateForSelection(id);
    if (! isValidCellType) return;

    GridCell *referencedCell = findReferenceCell(id);

    if (this->selectedCell != nullptr)
    {
        this->selectedCell->updateSelectStatus(false);
    }

    if (referencedCell != nullptr)
    {
        referencedCell->updateSelectStatus(true);
        this->selectedCell = referencedCell;
    }
}

bool GridComponent::tellIfCellCandidateForSelection(GridCellId id)
{
    switch (this->gameMode) {
        case GameMode::Playing:
            return id.getType() == GridCellIdType::Game;
        case GameMode::SolvingPreparation:
            return id.getType() != GridCellIdType::Game;
    default:
        return false;
    }
}

GridCell* GridComponent::findReferenceCell(GridCellId id) const
{
    GridCell* reference = nullptr;
    int maxId = -1;
    const int idNumber = id.getId();

    switch (id.getType())
    {
        case GridCellIdType::Game:
            maxId = (this->sideCellsCount * this->sideCellsCount) - 1;
            if (idNumber >= 0 && idNumber <= maxId)
            {
                const int line = idNumber / this->sideCellsCount;
                const int column = idNumber % this->sideCellsCount;

                reference = (*(*this->gameCells)[line])[column];
            }
        break;
        case GridCellIdType::ClueTop:
            maxId = this->sideCellsCount - 1;
            if (idNumber >= 0 && idNumber <= maxId)
            {
                reference = (*this->topCluesCells)[idNumber];
            }
        break;
        case GridCellIdType::ClueBottom:
            maxId = this->sideCellsCount - 1;
            if (idNumber >= 0 && idNumber <= maxId)
            {
                reference = (*this->bottomCluesCells)[idNumber];
            }
        break;
        case GridCellIdType::ClueLeft:
           maxId = this->sideCellsCount - 1;
           if (idNumber >= 0 && idNumber <= maxId)
           {
               reference = (*this->leftCluesCells)[idNumber];
           }
        break;
        case GridCellIdType::ClueRight:
            maxId = this->sideCellsCount - 1;
            if (idNumber >= 0 && idNumber <= maxId)
            {
                reference = (*this->rightCluesCells)[idNumber];
            }
        }
    return reference;
}

void GridComponent::keyReleaseEvent(QKeyEvent *event)
{
    if (this->selectedCell == nullptr) return;
    int key = event->key();

    bool isValidKey = key >= Qt::Key::Key_0 && key <= Qt::Key::Key_9;
    if (isValidKey)
    {
        GridCell *referencedCell = findReferenceCell(this->selectedCell->getId());
        int value = key - Qt::Key::Key_0;

        referencedCell->updateValue(value);
    }
}

void GridComponent::configureAsSolver(int size)
{
    bool isValidSize = size >= 4 && size <= 9;
    if (! isValidSize) return;
    this->gameMode = GameMode::SolvingPreparation;
    this->sideCellsCount = size;
    this->update();
}

void GridComponent::solve()
{
    if (this->gameMode != GameMode::SolvingPreparation) return;

    if (this->selectedCell != nullptr)
    {
        this->selectedCell->updateSelectStatus(false);
        this->selectedCell = nullptr;
    }
    this->gameMode = GameMode::SolvingProcess;

    this->solver->setParameters(
        this->sideCellsCount,
        this->getDigitsFromCells(this->topCluesCells),
        this->getDigitsFromCells(this->bottomCluesCells),
        this->getDigitsFromCells(this->leftCluesCells),
        this->getDigitsFromCells(this->rightCluesCells)
    );

    bool success = this->solver->solve();
    if ( ! success )
    {
        QMessageBox::critical(this->parentWidget(), tr("Failure"), tr("Failed to solve the grid."));
    }
    else
    {
        auto values = this->solver->getValues();
        for (auto rowIt = values.begin(); rowIt != values.end(); rowIt++)
        {
            auto rowIndex = static_cast<int>(rowIt - values.begin());
            for (auto colIt = rowIt->begin(); colIt != rowIt->end(); colIt++)
            {
                auto colIndex = static_cast<int>(colIt - rowIt->begin());
                auto currentCell = (*(*this->gameCells)[rowIndex])[colIndex];
                currentCell->updateValue(values[rowIndex][colIndex]);
            }
        }

        QMessageBox::information(this->parentWidget(), tr("Success"), tr("Solved the grid."));
    }
}

QVector<int> GridComponent::getDigitsFromCells(QVector<GridCell *> *cellsLine) const
{
    QVector<int> result;

    for (auto it = cellsLine->begin(); it != cellsLine->end(); it++)
    {
        GridCell *currentCell(*it);
        QSet<int> currentCellValues(currentCell->getValues());

        assert(currentCellValues.size() <= 1);

        int clueValue = 0;
        if (currentCellValues.size() > 0) clueValue = currentCellValues.toList()[0];

        result.push_back(clueValue);
    }

    return result;
}
