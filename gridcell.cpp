#include <QPainter>
#include <QBrush>
#include <QMouseEvent>
#include "gridcell.h"

GridCell::GridCell(GridCellId id, QWidget *parent) : QWidget(parent), id(id)
{
    this->setSizePolicy(QSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed));
}

QSize GridCell::sizeHint() const
{
    return QSize(SIZE, SIZE);
};

void GridCell::paintEvent(QPaintEvent *event)
{
    qreal STROKE_WIDTH = 3.0;
    qreal INNER_SIZE = static_cast<qreal>(SIZE) - 2*STROKE_WIDTH;
    QPainter painter(this);

    QRectF backgroundRectangle(0.0, 0.0, SIZE, SIZE);
    QBrush backgroundBrush(this->selected ? Qt::blue : Qt::black);
    painter.fillRect(backgroundRectangle, backgroundBrush);

    QRectF innerRectangle(STROKE_WIDTH, STROKE_WIDTH, INNER_SIZE, INNER_SIZE);
    QBrush innerBrush(this->selected ? Qt::red : Qt::white);
    painter.fillRect(innerRectangle, innerBrush);
}

void GridCell::addValue(int value)
{
    if (this->id.getType() != GridCellIdType::Game){
        this->values.clear();
    }
    this->values.insert(value);
}

void GridCell::updateSelectStatus(bool newStatus)
{
    this->selected = newStatus;
    this->repaint();
}

void GridCell::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        emit this->notifyCellSelected(this->id);
    }
}
