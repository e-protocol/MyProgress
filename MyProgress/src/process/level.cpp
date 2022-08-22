#include "level.h"

Level::Level(const QDomElement &element)
{
    dataVector = new QVector<DataWidget*>;
    initTags();
    parseLevel(element);
    initLevelWidget();
}

Level::~Level()
{
    qDeleteAll(*dataVector);
    dataVector->clear();
    delete dataVector;
    tags->clear();
    delete tags;
    delete levelW;
}

void Level::parseLevel(const QDomElement &element)
{
    //init DataWidget
    for(int k = 0; k < numberLines; k++)
    {
        DataWidget *data = new DataWidget;
        data->dataEdit = new QTextEdit(levelW);
        data->dataEdit->setFixedWidth(labelTextWidth);
        data->dataLabel = new QLabel(levelW);
        data->dataLabel->setFixedWidth(labelTextWidth);
        data->dataLabel->setObjectName("dataLabel");
        data->picLabel = new QLabel(levelW);
        data->picLabel->setFixedSize(picSide, picSide);
        data->picLabel->installEventFilter(this);
        dataVector->push_back(data);
    }

    updateData(element);
}

void Level::initAchievedStatus(const QDomElement &elem, int &index)
{
    QString text = textSeparator(elem.attribute("data", ""));
    dataVector->at(index)->dataLabel->setText(text);
    dataVector->at(index)->dataEdit->setText(text);
    //init achieved status
    QString status = elem.attribute("status", "");
    status == "true" ? dataVector->at(index)->status = true
                     : dataVector->at(index)->status = false;
    setStatusPic(index++);
}
void Level::setStatusPic(int index)
{
    bool status = dataVector->at(index)->status;
    QString picName;
    status ? picName = "checked.png" : picName = "unchecked.png";
    QPixmap pixmap(":/images/" + picName);
    pixmap = pixmap.scaled(QSize(picSide, picSide), Qt::KeepAspectRatio, Qt::SmoothTransformation);
    dataVector->at(index)->picLabel->setPixmap(pixmap);
}

void Level::initTags()
{
    tags = new QVector<QString>
    {
        "skills",
        "tasks"
    };
}

void Level::initLevelWidget()
{
    levelW = new QWidget;
    QScrollArea *scrollArea = new QScrollArea;
    scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    grid = new QGridLayout(levelW);
    scrollArea->setLayout(grid);
    levelW->setLayout(scrollArea->layout());
    levelW->setFixedSize(widgetWidth, widgetHeight);
    int row = 0, col = 0;

    for(int k = 0; k < dataVector->size(); k++)
    {
        switch(k)
        {
            case(0): //skills
            case(4): //tasks
            {
                QString tagName;
                k == 4 ? tagName = tags->at(1) : tagName = tags->at(k);
                QLabel *labelTitle = new QLabel(tagName + ":", levelW);
                labelTitle->setObjectName("dataLabel");
                grid->addWidget(labelTitle, row, col = 0, Qt::AlignLeft);
                break;
            }
            default: break;
        }

        createLine(row, col = 0, dataVector->at(k));
        row++;
    }

    //add spacing widget
    QWidget *wSpace = new QWidget(levelW);
    wSpace->setFixedHeight(spaceHeight);
    grid->addWidget(wSpace, row++, 0);
}

void Level::createLine(int &row, int &col, DataWidget* dataWidget)
{
    grid->addWidget(dataWidget->picLabel, row, ++col, Qt::AlignLeft);
    grid->addWidget(dataWidget->dataLabel, row, ++col, Qt::AlignLeft);
    grid->addWidget(dataWidget->dataEdit, row, col++, Qt::AlignLeft);
    dataWidget->dataEdit->setVisible(false);
}

void Level::setEditMode(bool status)
{
    editMode = status;

    for(auto &elem : *dataVector)
        status ? (elem->dataLabel->setVisible(false),
                  elem->dataEdit->setVisible(true))
               : (elem->dataLabel->setVisible(true),
                  elem->dataEdit->setVisible(false));

    if(!status)
        saveData();
}

void Level::saveData()
{
    for(auto &elem : *dataVector)
        elem->dataLabel->setText(elem->dataEdit->toPlainText());

    QDomElement levelData = _savedData.firstChildElement();
    int index = 0;

    for(; !levelData.isNull(); levelData = levelData.nextSiblingElement())
    {
        //parse nested elements skills and tasks
        if(levelData.tagName() == tags->at(0) ||
                levelData.tagName() == tags->at(1))
        {
            int elemIndex = 0;

            for(QDomElement e = levelData.firstChildElement();
                    !e.isNull(); e = e.nextSiblingElement())
                compareData(e, index, elemIndex++);
        }
        else
            compareData(levelData, index, 0);
    }
}

void Level::compareData(const QDomElement &elem, int &index,
                        const int elemIndex)
{
    QString curText = textCombiner(dataVector->at(index)->dataLabel->text());
    QString savedText = elem.attribute("data", "");
    QString curStatus;
    dataVector->at(index++)->status ? curStatus = "true" : curStatus = "false";
    QString savedStatus = elem.attribute("status", "");

    if(curText != savedText)
        _savedData.elementsByTagName(elem.tagName()).
                at(elemIndex).toElement().setAttribute("data", curText);

    if(curStatus != savedStatus)
        _savedData.elementsByTagName(elem.tagName()).
                at(elemIndex).toElement().setAttribute("status", curStatus);
}

bool Level::checkRequiremetsMatch()
{
    for(auto &elem : *dataVector)
        if(!elem->status)
            return false; 

    return true;
}

//insert new line symbol if line is too long
QString Level::textSeparator(const QString &text)
{
    QString outputText = "",
            word = "";
    int index = 0;

    for(int k = 0; k < text.size(); k++)
    {
        word.push_back(text[k]);
        index++;

        if(index == maxLineLength ||
                (text[k] == ' ' && index < maxLineLength))
        {
            if(index == maxLineLength)
            {
                outputText.push_back('\n');
                index = 0;
            }

            for(auto &elem : word)
                outputText.push_back(elem);

            word.clear();
        }
    }

    if(!word.isEmpty())
        for(auto &elem : word)
            outputText.push_back(elem);

    return outputText;
}

//remove newLine symbols
QString Level::textCombiner(const QString &text)
{
    QStringList strList = text.split(QRegExp("\n"), Qt::KeepEmptyParts);
    QString outputText;

    for(auto &elem : strList)
        outputText += elem;

    return outputText;
}

bool Level::eventFilter(QObject *obj, QEvent *event)
{
    if(!editMode)
        return QObject::event(event);

    QMouseEvent *mouseEvent = static_cast<QMouseEvent*>(event);

    for(int k = 0; k < dataVector->size(); k++)
        if(dataVector->at(k)->picLabel == obj &&
                mouseEvent->button() == Qt::LeftButton
                && event->type() == QEvent::MouseButtonPress)
        {
            dataVector->at(k)->status = !dataVector->at(k)->status;
            setStatusPic(k);

            break;
        }

    return QObject::event(event);
}

void Level::updateData(const QDomElement &element)
{
    _savedData = element;
    levelName = element.attribute("name", "");
    date = QDate::fromString(element.attribute("date", ""), "dd.MM.yyyy");

    QDomElement levelData = element.firstChildElement();
    int index = 0;

    for(; !levelData.isNull(); levelData = levelData.nextSiblingElement())
    {
        //parse nested elements skills and tasks
        if(levelData.tagName() == tags->at(0) ||
                levelData.tagName() == tags->at(1))
            for(QDomElement e = levelData.firstChildElement();
                    !e.isNull(); e = e.nextSiblingElement())
                initAchievedStatus(e, index);
        else
            initAchievedStatus(levelData, index);
    }
}
