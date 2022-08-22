#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#pragma once
#include <QMainWindow>
#include "src/resources/declaration.h"
#include "bodyWidget.h"
#include "headerWidget.h"
#include "src/process/settings.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    Settings *getSettings() { return settings; }
    BodyWidget* getBodyWidget() { return bodyWidget; }
    HeaderWidget* getHeaderWidget() { return headerWidget; }

private:
    Ui::MainWindow *ui;
    HeaderWidget *headerWidget = nullptr;
    BodyWidget *bodyWidget = nullptr;
    Settings *settings = nullptr;
};
#endif // MAINWINDOW_H
