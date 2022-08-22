#include "dirWidget.h"
#include "bodyWidget.h"

DirWidget::DirWidget(const QString &title, const QString &picName,
                     BodyWidget *bodyWidget, int type)
{
    _bodyWidget = bodyWidget;
    _type = type;
    installEventFilter(this);
    setMouseTracking(true);
    QVBoxLayout *widgetVLayout = new QVBoxLayout;
    setLayout(widgetVLayout);

    _title = title;
    _picName = picName;

    picLabel = new QLabel;
    picLabel->setFixedSize(QSize(dirPicSide, dirPicSide));
    picLabel->setPixmap(drawPixmapLabel(picName, dirPicSide));
    widgetVLayout->addWidget(picLabel, 0, Qt::AlignCenter);

    titleLabel = new QLabel(title);
    titleLabel->setFixedWidth(dirPicSide / 2);
    titleLabel->setAlignment(Qt::AlignCenter);
    titleLabel->setObjectName("titleDirLabel");
    widgetVLayout->addWidget(titleLabel, 0, Qt::AlignCenter);
}

DirWidget::~DirWidget()
{
    //_bodyWidget free in MainWindow
    //titleLabel and picLabel are set in widgetVLayout
}

QPixmap DirWidget::drawPixmapLabel(const QString &picName, int side)
{
    QPixmap pixmap(":/images/" + picName);
    pixmap = pixmap.scaled(QSize(side, side), Qt::KeepAspectRatio, Qt::SmoothTransformation);
    QBitmap map(side, side);
    map.fill(Qt::color0);

    QPainter painter(&map);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setPen(Qt::NoPen);
    painter.setBrush(Qt::color1);
    painter.drawRoundedRect(0, 0, side, side, side * dirPicRadius, side * dirPicRadius);
    pixmap.setMask(map);
    return pixmap;
}

void DirWidget::rescaleEvent()
{
    isProcessEvent = true;
    int step = 0;

    if(picSide == dirPicSide)
        step = 1;
    else if(picSide > dirPicSide)
        step = -1;
    else
    {
        isProcessEvent = false;
        return;
    }

    for(int k = 1; k < 50; k++)
    {
        //rescale picture
        picSide += step;
        picLabel->setFixedSize(QSize(picSide, picSide));
        picLabel->setPixmap(drawPixmapLabel(_picName, picSide));

        //rescale title
        if(k % 10 == 0)
        {
            fontSize += step;
            QFont font = titleLabel->font();
            font.setPixelSize(fontSize);
            titleLabel->setFont(font);
            titleLabel->setFixedWidth(picSide / 2);
        }

        qApp->processEvents();
    }

    isProcessEvent = false;
}

bool DirWidget::eventFilter(QObject *obj, QEvent *event)
{
    if(obj != this)
        return QObject::eventFilter(obj, event);

    if((event->type() == QEvent::Enter || event->type() == QEvent::Leave) &&
            !isProcessEvent)
        rescaleEvent();

    return QObject::eventFilter(obj, event);
}

void DirWidget::mousePressEvent(QMouseEvent *event)
{
    if(event->button() == Qt::LeftButton)
        _bodyWidget->switchToTab(_type);
}
