#ifndef GRIDCELL_H
#define GRIDCELL_H

#include <QWidget>

const int SIZE = 50;

class Gridcell : public QWidget
{
    Q_OBJECT
public:
    explicit Gridcell(QWidget *parent = nullptr);

signals:

public slots:
private:
    QSize sizeHint() const;
    void paintEvent(QPaintEvent* event);
};

#endif // GRIDCELL_H
