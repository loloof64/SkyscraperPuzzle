#include <QPainter>
#include <QBrush>
#include "gridcell.h"

Gridcell::Gridcell(QWidget *parent) : QWidget(parent)
{
    this->setSizePolicy(QSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed));
}

QSize Gridcell::sizeHint() const {
    return QSize(SIZE, SIZE);
};

void Gridcell::paintEvent(QPaintEvent *event) {
    qreal STROKE_WIDTH = 3.0;
    qreal INNER_SIZE = static_cast<qreal>(SIZE) - 2*STROKE_WIDTH;
    QPainter painter(this);

    QRectF backgroundRectangle(0.0, 0.0, SIZE, SIZE);
    QBrush backgroundBrush(Qt::black);
    painter.fillRect(backgroundRectangle, backgroundBrush);

    QRectF innerRectangle(STROKE_WIDTH, STROKE_WIDTH, INNER_SIZE, INNER_SIZE);
    QBrush innerBrush(Qt::white);
    painter.fillRect(innerRectangle, innerBrush);
}
