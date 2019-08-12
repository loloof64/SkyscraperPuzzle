#include "gridcomponent.h"
#include "gridcellid.h"

GridComponent::GridComponent(QWidget *parent) : QWidget(parent)
{
    this->layout = new QGridLayout();
    this->setLayout(this->layout);
    this->initializeCells();
    this->setSizePolicy(QSizePolicy(QSizePolicy::Policy::Preferred, QSizePolicy::Policy::Preferred));
    this->grabKeyboard();
}

GridComponent::~GridComponent()
{
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

GridCell* GridComponent::findReferenceCell(GridCellId id) const
{
    GridCell* reference = nullptr;
    int maxId = -1;
    const int idNumber = id.getId();

    switch (id.getType())
    {
        case Game:
            maxId = (this->sideCellsCount * this->sideCellsCount) - 1;
            if (idNumber >= 0 && idNumber <= maxId)
            {
                const int line = idNumber / this->sideCellsCount;
                const int column = idNumber % this->sideCellsCount;

                reference = (*(*this->gameCells)[line])[column];
            }
        break;
        case ClueTop:
            maxId = this->sideCellsCount - 1;
            if (idNumber >= 0 && idNumber <= maxId)
            {
                reference = (*this->topCluesCells)[idNumber];
            }
        break;
        case ClueBottom:
            maxId = this->sideCellsCount - 1;
            if (idNumber >= 0 && idNumber <= maxId)
            {
                reference = (*this->bottomCluesCells)[idNumber];
            }
        break;
        case ClueLeft:
           maxId = this->sideCellsCount - 1;
           if (idNumber >= 0 && idNumber <= maxId)
           {
               reference = (*this->leftCluesCells)[idNumber];
           }
        break;
        case ClueRight:
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
