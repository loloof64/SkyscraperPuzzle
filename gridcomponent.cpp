#include "gridcomponent.h"

GridComponent::GridComponent(QWidget *parent) : QWidget(parent)
{
    this->layout = new QGridLayout();
    setLayout(this->layout);
    this->initializeCells();
}

GridComponent::~GridComponent()
{
    this->cleanUpCells();
    delete this->layout;
}

QSize GridComponent::sizeHint() const
{
    const int sideSize = (this->sideCellsCount + 3) * SIZE;
    return QSize(sideSize, sideSize);
}

void GridComponent::paintEvent(QPaintEvent *event)
{

}

void GridComponent::initializeCells()
{
    this->gameCells = new QVector<GridCellVector>();
    for (int j = 0; j < this->sideCellsCount; j++)
    {
        QVector<Gridcell *> *currentLine = new QVector<Gridcell *>();
        for (int i = 0; i < this->sideCellsCount; i++)
        {
            auto currentCell = new Gridcell(this);
            currentLine->push_back(currentCell);
            this->layout->addWidget(currentCell, j+1, i+1);
        }
        this->gameCells->push_back(currentLine);
    }

    this->topCluesCells = new QVector<Gridcell *>();
    for (int i = 0; i < this->sideCellsCount; i++)
    {
        auto currentCell = new Gridcell(this);
        this->topCluesCells->push_back(currentCell);
        this->layout->addWidget(currentCell, 0, i+1);
    }

    this->bottomCluesCells = new QVector<Gridcell *>();
    for (int i = 0; i < this->sideCellsCount; i++)
    {
        auto currentCell = new Gridcell(this);
        this->bottomCluesCells->push_back(currentCell);
        this->layout->addWidget(currentCell, this->sideCellsCount + 1, i+1);
    }

    this->leftCluesCells = new QVector<Gridcell *>();
    for (int i = 0; i < this->sideCellsCount; i++)
    {
        auto currentCell = new Gridcell(this);
        this->leftCluesCells->push_back(currentCell);
        this->layout->addWidget(currentCell, i+1, 0);
    }

    this->rightCluesCells = new QVector<Gridcell *>();
    for (int i = 0; i < this->sideCellsCount; i++)
    {
        auto currentCell = new Gridcell(this);
        this->leftCluesCells->push_back(currentCell);
        this->layout->addWidget(currentCell, i+1, this->sideCellsCount + 1);
    }
}

void GridComponent::cleanUpCells()
{
    for (auto outer_it = this->gameCells->begin(); outer_it != this->gameCells->end(); outer_it++)
    {
        QVector<Gridcell *> *current_line = *outer_it;
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
