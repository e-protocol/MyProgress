#ifndef HEADERWIDGET_H
#define HEADERWIDGET_H

#pragma once
#include "src/resources/declaration.h"

class MainWindow;

class HeaderWidget : public QWidget
{
public:
    HeaderWidget(MainWindow *mainWindow);
    ~HeaderWidget();
    void showBtnBackToMain(bool status);

private:
    MainWindow *_mainWindow;
    QHBoxLayout *hLayout = nullptr;
    QPushButton *btnBackToMain = nullptr;
    const int headerHeight = 50,
              btnSide = 40;
    const QString version = "v1.0.0";

    void initHeaderWidget();
    void closeDialog();
    void aboutDialog();
};

#endif // HEADERWIDGET_H
