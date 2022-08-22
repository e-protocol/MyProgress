#include "sliderWidget.h"

SliderWidget::SliderWidget(QVector<QWidget*> *widgetVector, int activeIndex)
    : _widgetVector(widgetVector), _activeIndex(activeIndex)
{
    prevIndex = activeIndex;
    initSliderWidget();
}

SliderWidget::~SliderWidget()
{
    if(_widgetVector != nullptr)
    {
        qDeleteAll(*_widgetVector);
        _widgetVector->clear();
        delete _widgetVector;
    }
}

void SliderWidget::initSliderWidget()
{
    scrollArea = new QScrollArea;
    QHBoxLayout *hLayout = new QHBoxLayout;
    hLayout->setContentsMargins(0, 0, 0, 0);
    setLayout(hLayout);
    hLayout->addStretch();
    //add left btn
    btnLeft = new QPushButton("<");
    btnLeft->setObjectName("menuBtn");
    btnLeft->setFixedSize(btnSide, btnSide);
    hLayout->addWidget(btnLeft);
    connect(btnLeft, &QPushButton::clicked, this, [=]
    {
        if(_activeIndex > 0)
            moveWidgets(--_activeIndex, true);
    });

    //create central slider widget
    QWidget *centralW = new QWidget;
    centralW->setObjectName("sliderWidget");
    centralW->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);
    QHBoxLayout *centralHLayout = new QHBoxLayout;
    centralHLayout->setContentsMargins(0, 0, 0, 0);
    centralHLayout->setSpacing(0);
    scrollArea->setWidget(centralW);
    scrollArea->setFrameShape(QFrame::NoFrame);
    scrollArea->setWidgetResizable(true);
    centralW->setLayout(centralHLayout);
    hLayout->addWidget(scrollArea, 0, Qt::AlignLeft);
    scrollArea->setFixedWidth(wWidth * 3);
    scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    for(auto &elem : *_widgetVector)
    {
        elem->setFixedSize(QSize(wWidth, wHeight));
        centralHLayout->addWidget(elem, 0, Qt::AlignLeft);
    }

    //add right btn
    btnRight = new QPushButton(">");
    btnRight->setObjectName("menuBtn");
    btnRight->setFixedSize(btnSide, btnSide);
    hLayout->addWidget(btnRight);
    hLayout->addStretch();
    connect(btnRight, &QPushButton::clicked, this, [=]
    {
        if(_activeIndex < _widgetVector->size() - 3)
            moveWidgets(++_activeIndex, true);
    });
}

//disallow multiple btn signal call, allow after animation finished
void SliderWidget::moveWidgets(int index, bool enableAnimation)
{
    blockInteraction(true);
    QString style = "font: %1px \"Arial\";";

    for(int k = 1; k < _widgetVector->size() - 1; k++)
        k == index + 1
            ? _widgetVector->at(k)->setStyleSheet(style.arg(QString::number(fontMaxSize)))
            : _widgetVector->at(k)->setStyleSheet(style.arg(QString::number(fontMinSize)));

    //scroll animation imitation
    if(!enableAnimation)
    {
        scrollArea->horizontalScrollBar()->setValue(wWidth * index);
        blockInteraction(false);
        prevIndex = index;
        return;
    }

    QElapsedTimer *timer = new QElapsedTimer;
    long millis = 0;
    timer->start();
    int step = wWidth * index - wWidth * prevIndex;
    step /= abs(step);
    int fontIndex = 0;
    const int fontStep = wWidth / (fontMaxSize - fontMinSize);
    emit indexChanged(index, prevIndex);

    for(int k = wWidth * prevIndex; k != wWidth * index; k += step)
    {
        //resize new index font
        _widgetVector->at(index + 1)->setStyleSheet(style.arg(
                                  QString::number(fontMinSize + fontIndex / fontStep)));
        //resize old index font
        _widgetVector->at(prevIndex + 1)->setStyleSheet(style.arg(
                                  QString::number(fontMaxSize - fontIndex / fontStep)));
        fontIndex++;

        //update scrollArea
        scrollArea->horizontalScrollBar()->setValue(k);
        scrollArea->update();
        scrollArea->show();
        qApp->processEvents();

        //create pause between widget repaints
        while(timer->elapsed() - millis < timerPause){}
        millis = timer->elapsed();
    }

    prevIndex = index;
    delete timer;
}

void SliderWidget::blockInteraction(bool status)
{
    btnLeft->blockSignals(status);
    btnRight->blockSignals(status);
}

//need to use styleSheet for subclass of QWidget
void SliderWidget::paintEvent(QPaintEvent* event)
{
    QStyleOption opt;
    opt.init(this);
    QPainter p(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);

    QWidget::paintEvent(event);
}

void SliderWidget::setLevel(int newLevel)
{
    _activeIndex = newLevel;
    moveWidgets(_activeIndex, true);
}
