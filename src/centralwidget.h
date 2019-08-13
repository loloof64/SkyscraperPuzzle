#ifndef CENTRALWIDGET_H
#define CENTRALWIDGET_H

#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>
#include "gridcomponent.h"

class CentralWidget : public QWidget
{
    Q_OBJECT
public:
    explicit CentralWidget(QWidget *parent = nullptr);
    ~CentralWidget() override;
    void configureSolverGrid4x4();

signals:

public slots:
private:
    QLabel *welcomeLabel;
    GridComponent *gridComponent;
    QPushButton *solveButton;
    QVBoxLayout *layout;
};

#endif // CENTRALWIDGET_H
