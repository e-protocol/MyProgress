#include "bodyWidget.h"
#include "src/ui/mainwindow.h"

BodyWidget::BodyWidget(MainWindow *mainWindow) : _mainWindow(mainWindow)
{
    setParent(_mainWindow);
    setFixedWidth(_mainWindow->width());
    dirVector = new QVector<DirWidget*>;
    contentVector = new QVector<ContentWidget*>;
    initBody();
    setMouseTracking(true);
}

BodyWidget::~BodyWidget()
{
    qDeleteAll(*dirVector);
    dirVector->clear();
    delete dirVector;
    qDeleteAll(*contentVector);
    contentVector->clear();
    delete contentVector;
}

void BodyWidget::initBody()
{
    vLayout = new QVBoxLayout;
    vLayout->setContentsMargins(0, 0, 0, 0);
    vLayout->setSpacing(0);
    setLayout(vLayout);

    ///!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! add 2 tabs directions electronics and special projects
    //create directions widgets
    directionsW = new QWidget; //this widget can be hidden
    vLayout->addWidget(directionsW, 0, Qt::AlignCenter);
    QHBoxLayout *directionsHLayout = new QHBoxLayout;
    directionsHLayout->setContentsMargins(0, 0, 0, 0);
    directionsW->setLayout(directionsHLayout);
    createDirWidget("Programming", "progImg.png", PROGRAMMING, directionsHLayout);//Programming

    ///!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! add tab of each direction
    //create content widgets
    contentW = new QWidget; //this widget is alwyas visible
    vLayout->addWidget(contentW, 0, Qt::AlignCenter);
    QHBoxLayout *contentHLayout = new QHBoxLayout;
    contentHLayout->setContentsMargins(0, 0, 0, 0);
    contentW->setLayout(contentHLayout);
    createContentWidget("Programming", PROGRAMMING, contentHLayout);
    createContentWidget("Settings", SETTINGS, contentHLayout);
}

void BodyWidget::createDirWidget(const QString &title, const QString &picName,
                                 TabType type, QHBoxLayout *layout)
{
    DirWidget *dirWidget = new DirWidget(title, picName,
                                         this, static_cast<int>(type));
    layout->addWidget(dirWidget, 0, Qt::AlignCenter);
    dirVector->push_back(dirWidget);
}

void BodyWidget::createContentWidget(const QString &title, TabType type, QHBoxLayout *layout)
{
    ContentWidget *contentWidget = new ContentWidget(_mainWindow, static_cast<int>(type), title);
    layout->addWidget(contentWidget);
    contentVector->push_back(contentWidget);
    contentWidget->setVisible(false); //default non visible
}

bool BodyWidget::eventFilter(QObject *obj, QEvent *event)
{

    return QObject::eventFilter(obj, event);
}

void BodyWidget::switchToTab(int type)
{
    TabType tabType = static_cast<TabType>(type);

    //ow only one content widget at a time
    for(auto &elem : *contentVector)
        elem->getContentType() == type
                ? elem->setVisible(true)
                : elem->setVisible(false);

    switch(tabType)
    {
        case(PROGRAMMING):
        case(ELECTRONICS):
        case(SPECIAL):
        case(SETTINGS):
        {
            activeTab = tabType;
            directionsW->setVisible(false);
            _mainWindow->getHeaderWidget()->showBtnBackToMain(true);
            break;
        }
        case(MAIN):
        {
            activeTab = tabType;
            directionsW->setVisible(true);
            break;
        }
        default: break;
    }
}

void BodyWidget::updateSliderWidget()
{
    for(auto &elem : *contentVector)
        elem->updateSliderWidget();
}

void BodyWidget::updateData()
{
    for(auto &elem : *contentVector)
        elem->updateData();
}
