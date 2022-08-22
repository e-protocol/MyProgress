#ifndef LEVEL_H
#define LEVEL_H

#pragma once
#include "src/resources/declaration.h"

/*
 * This class holds level direction data
 */
class Level : public QObject
{
public:
    Level(const QDomElement &element);
    ~Level();
    void updateData(const QDomElement &element);

    //getters
    const QString &getName() { return levelName; }
    const QDate &getDate() { return date; }
    QWidget *getWidget() { return levelW; }
    bool checkRequiremetsMatch();
    const QDomElement &getSavedData() { return _savedData; }

    //setters
    void setEditMode(bool status);

private:
    QWidget *levelW = nullptr;
    QString levelName = "";
    QDate date;
    QVector<QString> *tags = nullptr;
    QDomElement _savedData;
    const int numberLines = 8,
              picSide = 30,
              widgetWidth = 640,
              labelTextWidth = 500,
              widgetHeight = 500,
              maxLineLength = 60,
              spaceHeight = 100;
    QGridLayout *grid = nullptr;
    bool editMode = false;

    struct DataWidget //store each line of data
    {
        QLabel *picLabel = nullptr;
        QLabel *dataLabel = nullptr;
        QTextEdit *dataEdit = nullptr;
        bool status = false;
    };
    QVector<DataWidget*> *dataVector = nullptr;

    void parseLevel(const QDomElement &element);
    void initTags();
    void initLevelWidget();
    void createLine(int &row, int &col, DataWidget* dataWidget);
    void initAchievedStatus(const QDomElement &elem, int &index);
    void setStatusPic(int index); 
    QString textSeparator(const QString &text);
    QString textCombiner(const QString &text);
    bool eventFilter(QObject *obj, QEvent *event);
    void saveData();
    void compareData(const QDomElement &elem, int &index,
                     const int elemIndex);
};
#endif // LEVEL_H
