#include "progWidget.h"

ProgWidget::ProgWidget(Settings* settings)
{
    vLayout = new QVBoxLayout;
    vLayout->setContentsMargins(0, 0, 0, 0);
    vLayout->setSpacing(0);
    levelVector = new QVector<Level*>;
    setLayout(vLayout);
    _settings = settings;
    initProgWidget();
    checkRequiremets(); //check requirements on app start

    //init timer for checking date for progressBar update
    timer = new QTimer(this);
    timer->setInterval(timerDuration);
    connect(timer, &QTimer::timeout, this, [=]
    {
        if(!isAnimation && getDaysPassed() &&
                levelVector->at(curLevel)->checkRequiremetsMatch())
            saveChanges();
    });
    timer->start();
}

ProgWidget::~ProgWidget()
{
    qDeleteAll(*levelVector);
    levelVector->clear();
    delete levelVector;
}

void ProgWidget::setupGrade()
{
    QDomNode domNode = _settings->readSettings("Programming");

    if(!domNode.isNull() &&
            domNode.toElement().tagName() == "direction")
        parseNode(domNode);
}

void ProgWidget::parseNode(const QDomNode &node)
{
    QDomElement nodeElement = node.firstChildElement(); //go to first nested tag

    if(nodeElement.tagName() != "level")
        return;

    while(!nodeElement.isNull())
    {
        //create level
        Level *lvl = new Level(nodeElement);
        QDomElement element = nodeElement.firstChildElement(); //get nested tag
        levelVector->push_back(lvl);
        nodeElement = nodeElement.nextSiblingElement(); //go to next node level
    }

    for(int k = 0; k < levelVector->size(); k++)
        if(levelVector->at(k)->getName() == node.toElement().attribute("currentLevelName", ""))
        {
            curLevel = k;
            break;
        }

    _savedNode = node; //save node for further writeSettings();
}

void ProgWidget::initProgWidget()
{
    setupGrade();
    QVector<QWidget*> *wVector = new QVector<QWidget*>;
    wVector->push_back(new QLabel); //left empty widget for slider

    for(auto &elem : *levelVector)
    {
        QLabel *label = new QLabel(elem->getName());
        label->setAlignment(Qt::AlignCenter);
        label->setObjectName("progLevelLabel");
        wVector->push_back(label);
    }

    wVector->push_back(new QLabel); //right empty widget for slider
    sliderWidget = new SliderWidget(wVector, curLevel);
    sliderWidget->setFixedWidth(progressBarWidth);
    connect(sliderWidget, &SliderWidget::indexChanged, this, &ProgWidget::sliderNewIndex);
    vLayout->addWidget(sliderWidget, 0, Qt::AlignCenter);

    spacingW = new QWidget;
    vLayout->addWidget(spacingW);
    initProgressBar(); //create XP bar
    initDataWidgets();
}

void ProgWidget::initProgressBar()
{
    //init layer
    QHBoxLayout *hProgressLayout = new QHBoxLayout;
    hProgressLayout->setContentsMargins(defaultMargin, 0, defaultMargin, 0);
    progressW = new QWidget;
    progressW->setLayout(hProgressLayout);
    vLayout->addWidget(progressW, 0, Qt::AlignCenter);

    curLevelLabel = new QLabel;
    curLevelLabel->setObjectName("progLevelLabel");
    curLevelLabel->setFixedWidth(levelLabelWidth);
    hProgressLayout->addWidget(curLevelLabel, 0, Qt::AlignLeft);

    //init progressBar
    progressBar = new QProgressBar;
    hProgressLayout->addWidget(progressBar, 0, Qt::AlignLeft);
    progressBar->setFixedSize(progressBarWidth, progressBarHeight);
    progressBar->setMaximum(daysPerLevel);
    progressBar->setMinimum(0);

    nextLevelLabel = new QLabel;
    nextLevelLabel->setObjectName("progLevelLabel");
    nextLevelLabel->setFixedWidth(levelLabelWidth);
    hProgressLayout->addWidget(nextLevelLabel, 0, Qt::AlignLeft);
    updateProgressBar();
}

void ProgWidget::initDataWidgets()
{
    //init data widgets

    for(int k = 0; k < levelVector->size(); k++)
    {
        vLayout->addWidget(levelVector->at(k)->getWidget(), 0, Qt::AlignCenter);
        k == curLevel ? levelVector->at(k)->getWidget()->setVisible(true)
                      : levelVector->at(k)->getWidget()->setVisible(false);
    }

    //init btns
    QHBoxLayout *hBtnLayout = new QHBoxLayout;
    vLayout->addLayout(hBtnLayout);

    btnEdit = new QPushButton("Edit");
    btnEdit->setObjectName("btnContent");
    connect(btnEdit, &QPushButton::clicked, this, [=]
    {
        if(isAnimation)
            return;

        setEditMode(true);
        sliderWidget->blockInteraction(true);
    });
    hBtnLayout->addWidget(btnEdit, 0, Qt::AlignRight);

    btnApply = new QPushButton("Apply");
    connect(btnApply, &QPushButton::clicked, this, [=]
    {
        setEditMode(false);
        sliderWidget->blockInteraction(false);
        saveChanges();
    });
    btnApply->setObjectName("btnContent");
    hBtnLayout->addWidget(btnApply, 0, Qt::AlignRight);
    hBtnLayout->addSpacing(levelLabelWidth + 10);
    setEditMode(false);
}

void ProgWidget::sliderNewIndex(int newIndex, int oldIndex)
{
    //fade out
    isAnimation = true;
    QGraphicsOpacityEffect *fadeOutOpacity = new QGraphicsOpacityEffect;
    levelVector->at(oldIndex)->getWidget()->setGraphicsEffect(fadeOutOpacity);
    QPropertyAnimation *aFadeOut = new QPropertyAnimation(fadeOutOpacity, "opacity"); //free on stop
    aFadeOut->setStartValue(1);
    aFadeOut->setEndValue(0);
    aFadeOut->setEasingCurve(QEasingCurve::OutBack);
    aFadeOut->setDuration(animationDuration);
    aFadeOut->start(QPropertyAnimation::DeleteWhenStopped);

    connect(aFadeOut, &QPropertyAnimation::finished, this, [=]
    {
        aFadeOut->stop();
        delete fadeOutOpacity;
        levelVector->at(oldIndex)->getWidget()->setVisible(false);
        levelVector->at(newIndex)->getWidget()->setVisible(true);
        //fade in
        QGraphicsOpacityEffect *fadeInOpacity = new QGraphicsOpacityEffect;
        levelVector->at(newIndex)->getWidget()->setGraphicsEffect(fadeInOpacity);
        QPropertyAnimation *aFadeIn = new QPropertyAnimation(fadeInOpacity, "opacity"); //free on stop
        aFadeIn->setStartValue(0);
        aFadeIn->setEndValue(1);
        aFadeIn->setEasingCurve(QEasingCurve::InBack);
        aFadeIn->setDuration(animationDuration);
        aFadeIn->start(QPropertyAnimation::DeleteWhenStopped);

        connect(aFadeIn, &QPropertyAnimation::finished, this, [=]
        {
            aFadeIn->stop();
            delete fadeInOpacity;
            sliderWidget->blockInteraction(false);
            isAnimation = false;
        });
    });
}

void ProgWidget::updateProgressBar()
{
    //init value
    int daysPassed = getDaysPassed() - (curLevel + 1) * daysPerLevel;

    if(daysPassed == progressBar->value())
        return;

    if(daysPassed < 0)
        progressBar->setValue(0);
    else if(daysPassed > daysPerLevel)
        progressBar->setValue(daysPerLevel);
    else
        progressBar->setValue(daysPassed);

    int percent = (float)daysPassed / (float)daysPerLevel * 100.0;
    QString valueText = "%1 / %2 (%3%)";
    progressBar->setFormat(valueText.arg(daysPassed).
                           arg(daysPerLevel).arg(percent));

    //update labels
    curLevelLabel->setText(levelVector->at(curLevel)->getName());

    curLevel < levelVector->size() - 1
        ? nextLevelLabel->setText(levelVector->at(curLevel + 1)->getName())
        : nextLevelLabel->setText("");
}

void ProgWidget::setEditMode(bool status)
{
    status ? (btnEdit->setVisible(false),
              btnApply->setVisible(true))
           : (btnEdit->setVisible(true),
              btnApply->setVisible(false));

    for(auto &elem : *levelVector)
        elem->setEditMode(status);
}

//save changes
void ProgWidget::saveChanges()
{
    timer->stop();
    checkRequiremets();

    if(_savedNode.isNull() ||
            _savedNode.toElement().tagName() != "direction")
        return;

    QDomElement nodeElement = _savedNode.firstChildElement(); //go to first nested tag

    if(nodeElement.tagName() != "level")
        return;

    bool newData = false;

    for(int k = 0; k < levelVector->size(); k++)
    {
        //compare data
        QDomElement savedData = levelVector->at(k)->getSavedData();
        QDomElement levelElem = nodeElement; //get nested tag
        _savedNode.replaceChild(savedData, levelElem);
        nodeElement = nodeElement.nextSiblingElement(); //go to next node level
    }

    _savedNode.toElement().setAttribute("currentLevelName", levelVector->at(curLevel)->getName());
    _settings->writeSettings(_savedNode);
    timer->start();
}

void ProgWidget::updateSLiderWidget()
{
    sliderWidget->updateSliderWidget();
}

void ProgWidget::updateData()
{
    _savedNode = _settings->readSettings("Programming");
    QDomElement nodeElement = _savedNode.firstChildElement(); //go to first nested tag

    if(nodeElement.tagName() != "level")
        return;

    int index = 0;

    while(!nodeElement.isNull())
    {
        //create level
        levelVector->at(index++)->updateData(nodeElement);
        nodeElement = nodeElement.nextSiblingElement(); //go to next node level
    }

    updateProgressBar();
    checkRequiremets();
}

//check requirements match and change current level
void ProgWidget::checkRequiremets()
{
    int newLevel = 0;

    for(int k = 0; k < levelVector->size(); k++)
        if(levelVector->at(k)->checkRequiremetsMatch()
                && (getDaysPassed() - daysPerLevel *
                (newLevel + 1)) >= daysPerLevel)
            newLevel++;

    if(newLevel != curLevel)
    {
        curLevel = newLevel;
        updateProgressBar();
        sliderWidget->setLevel(curLevel);
    }
}

//find how mush time passed since beginning date
int ProgWidget::getDaysPassed()
{
    return  QDate::currentDate().toJulianDay() - levelVector->at(0)->getDate().toJulianDay();
}
