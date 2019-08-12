#include <QPainter>
#include <QBrush>
#include <QMouseEvent>
#include <algorithm>
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

    this->drawValues();
}

void GridCell::updateValue(int value)
{
    if (value < 1 || value > 9) return;
    if (this->id.getType() != GridCellIdType::Game)
    {
        this->values.clear();
    }
    if (this->values.contains(value))
    {
        this->values.remove(value);
    }
    else {
        this->values.insert(value);
    }
    this->update();
}

void GridCell::updateSelectStatus(bool newStatus)
{
    this->selected = newStatus;
    this->update();
}

void GridCell::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
    {
        emit this->notifyCellSelected(this->id);
    }
}


GridCellId GridCell::getId() const
{
    return this->id;
}

void GridCell::drawValues()
{
    QPainter painter(this);

    if (this->values.size() == 0) return;
    if (this->values.size() == 1) {
        int value = this->values.toList()[0];
        painter.setFont(QFont("Helvetica", 20, 50));
        painter.setPen(this->selected ? Qt::yellow : Qt::black);
        painter.drawText(QPoint(18, 34), QString::number(value));
        return;
    }
    QList<int> sortedValues = this->values.toList();
    std::sort(sortedValues.begin(), sortedValues.end());

    QString group1Str = QString();
    QString group2Str = QString();

    int group1Limit = sortedValues.length() < 5 ? sortedValues.length() : 5;

    for (int i = 0; i < group1Limit; i++)
    {
        group1Str.append(QString::number(sortedValues[i]));
    }

    for (int i = 5; i < sortedValues.size(); i++)
    {
        group2Str.append(QString::number(sortedValues[i]));
    }

    painter.setFont(QFont("Helvetica", 8, 10));
    painter.setPen(this->selected ? Qt::yellow : Qt::black);
    painter.drawText(QPoint(8, 16), group1Str);
    painter.drawText(QPoint(8, 36), group2Str);
}
