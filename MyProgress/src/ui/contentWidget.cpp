#include "contentWidget.h"
#include "bodyWidget.h"
#include "mainwindow.h"

ContentWidget::ContentWidget(MainWindow *mainWindow, int type,
                             const QString &title)
    : _mainWindow(mainWindow), _type(type), _title(title)
{
    initContent();
    setObjectName("contentWidget");
}

ContentWidget::~ContentWidget()
{
    //progWidget added to inherited vLayout
}

void ContentWidget::initContent()
{
    vLayout = new QVBoxLayout;
    vLayout->setContentsMargins(0, 0, 0, 0);
    setLayout(vLayout);

    //init content header
    QLabel *titleLabel = new QLabel(_title);
    titleLabel->setObjectName("dirTitle");
    vLayout->addWidget(titleLabel, 0, Qt::AlignCenter);
    vLayout->addSpacing(wSpacing);
    QWidget *contentW = nullptr;

    //init content by type
    switch(_type)
    {
        //PROGRAMMING
        case(1):
        {
            progWidget = new ProgWidget(_mainWindow->getSettings());
            contentW = progWidget;
            break;
        }
        //SETTINGS
        case(4):
        {
            contentW = _mainWindow->getSettings()->getWidget();
            break;
        }
        default: break;
    }

    vLayout->addWidget(contentW, 0, Qt::AlignCenter);
}

void ContentWidget::updateSliderWidget()
{
    if(progWidget)
        progWidget->updateSLiderWidget();
}

void ContentWidget::updateData()
{
    if(progWidget)
        progWidget->updateData();
}
