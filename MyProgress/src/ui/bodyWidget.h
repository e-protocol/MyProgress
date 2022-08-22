#ifndef BODYWIDGET_H
#define BODYWIDGET_H

#pragma once
#include "src/resources/declaration.h"
#include "dirWidget.h"
#include "contentWidget.h"

class MainWindow;

class BodyWidget : public QWidget
{
public:
    BodyWidget(MainWindow *mainWindow);
    ~BodyWidget();
    void switchToTab(int type);
    MainWindow *getMainWindow() { return _mainWindow; }
    void updateSliderWidget();
    void updateData();

private:
    MainWindow *_mainWindow = nullptr; //cross pointer
    QVBoxLayout *vLayout = nullptr;
    QWidget *directionsW = nullptr,
            *contentW = nullptr;

    enum TabType
    {
        MAIN,
        PROGRAMMING,
        ELECTRONICS,
        SPECIAL,
        SETTINGS
    };
    TabType activeTab = MAIN;
    QVector<DirWidget*> *dirVector = nullptr; //store directions widgets
    QVector<ContentWidget*> *contentVector = nullptr; //store content of directions

    void initBody();
    void createDirWidget(const QString &title, const QString &picName,
                         TabType type, QHBoxLayout *layout);
    void createContentWidget(const QString &title, TabType type,
                             QHBoxLayout *layout);
    bool eventFilter(QObject *obj, QEvent *event);
};

#endif // BODYWIDGET_H
