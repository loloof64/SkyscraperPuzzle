#include "centralwidget.h"

CentralWidget::CentralWidget(QWidget *parent) : QWidget(parent)
{
    this->layout = new QVBoxLayout();
    this->setLayout(this->layout);
    this->welcomeLabel = new QLabel(tr("Welcome to the Skyscraper puzzle game."));
    this->welcomeLabel->setAlignment(Qt::Alignment(Qt::AlignHCenter | Qt::AlignVCenter));
    this->gridComponent = new GridComponent();
    this->solveButton = new QPushButton(tr("Solve"));
    this->layout->addWidget(this->welcomeLabel);
    this->layout->addWidget(this->gridComponent);
    this->layout->addWidget(this->solveButton);
    this->gridComponent->setVisible(false);
    this->solveButton->setVisible(false);

    QObject::connect(this->solveButton, SIGNAL(clicked()), this->gridComponent, SLOT(solve()));
}

CentralWidget::~CentralWidget()
{
    delete this->solveButton;
    delete this->gridComponent;
    delete this->welcomeLabel;
    delete this->layout;
}

void CentralWidget::configureSolverGrid4x4()
{
    this->gridComponent->configureAsSolver(4);
    this->welcomeLabel->setVisible(false);
    this->gridComponent->setVisible(true);
    this->solveButton->setVisible(true);
}


void CentralWidget::configureSolverGrid5x5()
{
    this->gridComponent->configureAsSolver(5);
    this->welcomeLabel->setVisible(false);
    this->gridComponent->setVisible(true);
    this->solveButton->setVisible(true);
}
