#ifndef SETTINGS_H
#define SETTINGS_H

#pragma once
#include "src/resources/declaration.h"

/*
 * class to store app settings
 * settings is devided into chapters
 * each chapter separates by symbols "//"
 * first line of chapter contains (direction name, level name)
 */

class MainWindow;

class Settings : public QObject
{
    Q_OBJECT
public:
    Settings(MainWindow *mainWindow);
    ~Settings();
    QDomNode readSettings(const QString &nodeName);
    void writeSettings(const QDomNode &newNode);
    QWidget *getWidget() { return widget; }

private:
    MainWindow *_mainWindow = nullptr;
    const QString settingsPath = QCoreApplication::applicationDirPath() + "/settings.xml";
    QWidget *widget = nullptr;
    const int spacingW = 50,
              widgetWidth = 620,
              widgetHeight = 500;
    QVector<QLineEdit *> *vectorLineEdit;
    QDomNode nodeSettings;

    void setStyles();
    void parseNode(const QDomNode &node);
    QDomDocument getDocument();
    void initWidget();
    void saveSettings();

private slots:
    void btnApplyClicked();
};

#endif // SETTINGS_H
