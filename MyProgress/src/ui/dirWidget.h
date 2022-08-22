#ifndef DIRWIDGET_H
#define DIRWIDGET_H

#pragma once
#include "src/resources/declaration.h"

class BodyWidget;

class DirWidget : public QWidget
{
public:
    DirWidget(const QString &title, const QString &picName,
              BodyWidget *bodyWidget, int type);
    ~DirWidget();
    void rescaleEvent();

private:
    BodyWidget *_bodyWidget = nullptr; //cross pointer
    QLabel *picLabel = nullptr,
           *titleLabel = nullptr;
    QString _title,
            _picName;
    bool isProcessEvent = false;
    int picSide = 300,
        fontSize = 20,
        _type;
    const int dirPicSide = 300;
    const double dirPicRadius = 0.2; //20% of dirPicSide

    void mousePressEvent(QMouseEvent *event);
    QPixmap drawPixmapLabel(const QString &picName, int side);
    bool eventFilter(QObject *obj, QEvent *event);
};

#endif // DIRWIDGET_H
