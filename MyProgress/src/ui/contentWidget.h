#ifndef CONTENTWIDGET_H
#define CONTENTWIDGET_H


/*
 * class to view progress of direction
 */
#pragma once
#include "src/resources/declaration.h"
#include "src/ui/progWidget.h"

class MainWindow;

class ContentWidget : public QWidget
{
public:
    ContentWidget(MainWindow *bodyWidget, int type,
                  const QString &title);
    ~ContentWidget();
    int getContentType() { return _type; }
    void updateSliderWidget();
    void updateData();

private:
    MainWindow *_mainWindow = nullptr; //cross pointer
    int _type;
    QVBoxLayout *vLayout = nullptr;
    QString _title;
    const int wSpacing = 50;
    ProgWidget *progWidget = nullptr;

    void initContent();
};

#endif // CONTENTWIDGET_H
