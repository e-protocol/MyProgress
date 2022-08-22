#include "settings.h"
#include "src/ui/mainwindow.h"

Settings::Settings(MainWindow *mainWindow) : _mainWindow(mainWindow)
{
    setStyles(); //set global stylesheet
    vectorLineEdit = new QVector<QLineEdit*>;
    initWidget();
}

Settings::~Settings()
{
    //widget is added to inherited vLayout in ContentWidget
    //all QLineEdit are inherited to vLayout in ContentWidget
    vectorLineEdit->clear();
    delete vectorLineEdit;
}

void Settings::setStyles()
{
    QFile file(":/styles.qss");
    file.open(QFile::ReadOnly);
    auto styleSheet = QString(file.readAll());
    qApp->setStyleSheet(styleSheet);
}

QDomDocument Settings::getDocument()
{
    QDomDocument doc;
    QFile file(settingsPath);
    QString errorStr;
    int errorLine, errorColumn;

    if(!file.open(QIODevice::ReadOnly) || !doc.setContent(&file, &errorStr, &errorLine, &errorColumn))
    {
        MsgBox *msgBox = new MsgBox("Error", "Couldn't read settings file.<br>" +
                                    errorStr + " at line " + QString::number(errorLine) + " column " +
                                    QString::number(errorColumn), 1);
        msgBox->exec();
        delete msgBox;
    }

    file.close();
    return doc;
}

//read settings by chapter number
QDomNode Settings::readSettings(const QString &nodeName)
{
    QDomDocument doc = getDocument();
    QDomNode readNode;
    QDomNode domNode = doc.firstChild().nextSibling().firstChildElement(); //get first node in settings

    while(!domNode.isNull())
    {
        if(domNode.toElement().attribute("name", "") == nodeName)
        {
            readNode = domNode;
            break;
        }

        domNode = domNode.nextSibling(); //go to next node
    }

    return readNode;
}

void Settings::writeSettings(const QDomNode &newNode)
{
    QDomDocument doc = getDocument();
    QDomNode domNode = doc.firstChild().nextSibling().firstChildElement(); //get first direction node
    QDomNode oldNode;
    int index = 0;

    //find direction
    while(!domNode.isNull())
    {
        if(domNode.toElement().attribute("name", "") ==
                newNode.toElement().attribute("name", ""))
        {
            oldNode = domNode;
            break;
        }

        index++;
        domNode = domNode.nextSibling(); //go to next node
    }

    if(oldNode.isNull())
        return;

    QFile file(settingsPath);

    if(!file.open(QIODevice::WriteOnly))
        qDebug() << "couldn't write to file";

    doc.documentElement().replaceChild(newNode, oldNode);
    file.resize(0);

    QTextStream stream;
    stream.setDevice(&file);
    stream.setCodec("UTF-8");
    doc.save(stream, 4);
    file.close();
}

void Settings::initWidget()
{
    widget = new QWidget;
    widget->setFixedSize(widgetWidth, widgetHeight);
    QVBoxLayout *vLayout = new QVBoxLayout;
    vLayout->setContentsMargins(20, 0, 20, 0);
    widget->setLayout(vLayout);
    vLayout->addSpacing(spacingW);

    //PROGRAMMING
    QLabel *titleProg = new QLabel("Programming");
    titleProg->setObjectName("dataLabel");
    vLayout->addWidget(titleProg, 0, Qt::AlignCenter);

    QWidget *progW = new QWidget;
    QHBoxLayout *progHLayout = new QHBoxLayout;
    progHLayout->setContentsMargins(0, 10, 0, 0);
    progW->setLayout(progHLayout);
    QLabel *dateLabel = new QLabel("Beginning Date: ");
    dateLabel->setFixedWidth(widgetWidth / 3);
    dateLabel->setObjectName("dataLabel");
    progHLayout->addWidget(dateLabel, 0, Qt::AlignLeft);
    QLineEdit *progEdit = new QLineEdit;
    vectorLineEdit->push_back(progEdit);
    progHLayout->addWidget(progEdit, 0, Qt::AlignLeft);
    vLayout->addWidget(progW, 0, Qt::AlignLeft);
    vLayout->addStretch();
    QDomNode node = readSettings("Programming");
    QString date = node.firstChildElement().attribute("date", ""); //get date of base level
    progEdit->setText(date);

    //Btns
    QWidget *btnWidget = new QWidget;
    QHBoxLayout *hBtnLayout = new QHBoxLayout;
    btnWidget->setLayout(hBtnLayout);
    vLayout->addWidget(btnWidget);
    hBtnLayout->addStretch(0);

    QPushButton *applyBtn = new QPushButton("Apply");
    applyBtn->setObjectName("btnContent");
    hBtnLayout->addWidget(applyBtn, 0, Qt::AlignLeft);
    connect(applyBtn, &QPushButton::clicked, this, &Settings::btnApplyClicked);

    QPushButton *closeBtn = new QPushButton("Close");
    closeBtn->setObjectName("btnContent");
    hBtnLayout->addWidget(closeBtn, 0, Qt::AlignLeft);
    connect(closeBtn, &QPushButton::clicked, this, [=]
    {
        _mainWindow->getBodyWidget()->switchToTab(0);
    });
}

void Settings::btnApplyClicked()
{
    MsgBox *msgBox = new MsgBox("Save", "Save changes?", 2);

    if(msgBox->exec() == QMessageBox::Yes)
        saveSettings();

    _mainWindow->getBodyWidget()->switchToTab(0);
    delete msgBox;
}

void Settings::saveSettings()
{
    for(int k = 0; k < vectorLineEdit->size(); k++)
    {
        QDomNode node;
        switch(k)
        {
            case(0): node = readSettings("Programming");
            default: break;
        }

        if(!node.isNull())
        {
            node.firstChildElement().setAttribute("date", vectorLineEdit->at(k)->text());
            writeSettings(node);
        }
    }

    //update data
    _mainWindow->getBodyWidget()->updateData();
}
