#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "centralwidget.h"
#include <QMainWindow>
#include <QAction>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void configureSolverGrid4x4();

private:
    Ui::MainWindow *ui;
    CentralWidget *centralWidget;
};

#endif // MAINWINDOW_H
