#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    //fixate mainWindow size
    setFixedSize(qApp->screens().at(0)->geometry().width(),
                 qApp->screens().at(0)->geometry().height());
    setWindowFlags(Qt::FramelessWindowHint);

    QVBoxLayout *vLayout = new QVBoxLayout;
    vLayout->setContentsMargins(0, 0, 0, 0);
    centralWidget()->setLayout(vLayout);
    headerWidget = new HeaderWidget(this);
    vLayout->addWidget(headerWidget, 0, Qt::AlignCenter);

    settings = new Settings(this);
    bodyWidget = new BodyWidget(this);
    vLayout->addWidget(bodyWidget, 0, Qt::AlignCenter);
    vLayout->addStretch();
    setMouseTracking(true);
    setWindowTitle("MyProgress");
    bodyWidget->updateSliderWidget(); //this required due to scrollBar can be set only after all widgets show()
}

MainWindow::~MainWindow()
{
    delete bodyWidget;
    delete settings;
    delete headerWidget;
    delete ui;
}

