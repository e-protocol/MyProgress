#include "headerWidget.h"
#include "mainwindow.h"

HeaderWidget::HeaderWidget(MainWindow *mainWindow) : _mainWindow(mainWindow)
{
    setParent(_mainWindow);
    setFixedSize(_mainWindow->width(), headerHeight);
    hLayout = new QHBoxLayout;
    setLayout(hLayout);
    initHeaderWidget();
}

HeaderWidget::~HeaderWidget()
{

}

void HeaderWidget::initHeaderWidget()
{
    btnBackToMain = new QPushButton("<<");
    btnBackToMain->setFixedSize(btnSide, btnSide);
    btnBackToMain->setToolTip("back to main");
    btnBackToMain->setObjectName("menuBtn");
    hLayout->setContentsMargins(15, 0, 0, 0);
    hLayout->addWidget(btnBackToMain, 0, Qt::AlignLeft);
    hLayout->addStretch();

    connect(btnBackToMain, &QPushButton::clicked, this, [=]
    {
        _mainWindow->getBodyWidget()->switchToTab(0); //switch to main
        showBtnBackToMain(false);
    });

    //btn about
    QPushButton *btnAbout = new QPushButton("?");
    btnAbout->setToolTip("app info");
    btnAbout->setObjectName("menuBtnAbout");
    btnAbout->setFixedSize(btnSide, btnSide);
    hLayout->addWidget(btnAbout, 0, Qt::AlignLeft);
    connect(btnAbout, &QPushButton::clicked, this, &HeaderWidget::aboutDialog);

    //tripleBar sign aka settings
    QWidget *btnSettingsW = new QWidget;
    QVBoxLayout *vSettingsLayout = new QVBoxLayout;
    vSettingsLayout->setContentsMargins(0, 2, 0, 0);
    btnSettingsW->setLayout(vSettingsLayout);
    QPushButton *btnSettings = new QPushButton("≡");
    btnSettings->setFixedSize(btnSide, btnSide);
    btnSettings->setToolTip("settings");
    btnSettings->setObjectName("menuBtn");
    vSettingsLayout->addWidget(btnSettings, 0, Qt::AlignLeft);
    hLayout->addWidget(btnSettingsW, 0, Qt::AlignLeft);
    connect(btnSettings, &QPushButton::clicked, this, [=]
    {
        _mainWindow->getBodyWidget()->switchToTab(4); //switch to settings
        showBtnBackToMain(true);
    });

    //btn close
    QPushButton *btnClose = new QPushButton("x");
    btnClose->setFixedSize(btnSide, btnSide);
    btnClose->setToolTip("close app");
    btnClose->setObjectName("menuBtn");
    hLayout->addWidget(btnClose, 0, Qt::AlignLeft);
    connect(btnClose, &QPushButton::clicked, this, &HeaderWidget::closeDialog);
    showBtnBackToMain(false);
}

void HeaderWidget::showBtnBackToMain(bool status)
{
    btnBackToMain->setVisible(status);
}

void HeaderWidget::closeDialog()
{
    MsgBox *msgBox = new MsgBox("Exit", "Are you sure?", 2);

    if(msgBox->exec() == QMessageBox::Yes)
    {
        delete msgBox;
        qApp->quit();
    }
    else
        delete msgBox;
}

void HeaderWidget::aboutDialog()
{
    MsgBox *msgBox = new MsgBox("About " + version, "This app is for progress tracking as a Software Developer.<br>"
                                                    "Experience is counted as 200 calendar days per Level and<br>"
                                                    "starts with beginning date. This date can be set in Settings<br>"
                                                    "Created by E-Protocol<br>"
                                                    "<a href=\"https://github.com/e-protocol\" style=\"color: rgb(0, 98, 255)\">"
                                                              "https://github.com/e-protocol</a>", 1);
    msgBox->exec();
    delete msgBox;
}
