#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->centralWidget = new CentralWidget();
    this->setCentralWidget(this->centralWidget);

    QObject::connect(this->ui->actionGrid_4x4, SIGNAL(triggered()), this, SLOT(configureSolverGrid4x4()));
}

MainWindow::~MainWindow()
{
    delete this->centralWidget;
    delete this->ui;
}

void MainWindow::configureSolverGrid4x4()
{
    this->centralWidget->configureSolverGrid4x4();
}
