#ifndef GRIDCELL_H
#define GRIDCELL_H

#include "gridcellid.h"
#include <QWidget>
#include <QSet>

const int SIZE = 50;

class GridCell : public QWidget
{
    Q_OBJECT
public:
    explicit GridCell(GridCellId id, QWidget *parent = nullptr);
    void updateSelectStatus(bool newStatus);
    void updateValue(int value);
    GridCellId getId() const;

signals:
    void notifyCellSelected(GridCellId id);

protected:
    QSize sizeHint() const override;
    void paintEvent(QPaintEvent* event) override;
    void mouseReleaseEvent(QMouseEvent* event) override;

private:
    bool selected = false;
    QSet<int> values;
    GridCellId id;
    void drawValues();
};

#endif // GRIDCELL_H
