#ifndef PROGWIDGET_H
#define PROGWIDGET_H

#pragma once
#include "src/resources/declaration.h"
#include "src/process/settings.h"
#include "sliderWidget.h"
#include "src/process/level.h"

/*
 * class for direction of programming
 */
class ProgWidget : public QWidget
{
    Q_OBJECT
public:
    ProgWidget(Settings *settings);
    ~ProgWidget();
    void updateSLiderWidget();
    void updateData();

private:
    QVBoxLayout *vLayout = nullptr;
    Settings *_settings = nullptr; //cross pointer
    int curLevel = -1;
    const int daysPerLevel = 200,
              progressBarWidth = 420,
              progressBarHeight = 50,
              defaultMargin = 8,
              levelLabelWidth = 100,
              dataHeight = 500,
              animationDuration = 500,
              timerDuration = 10000;
    SliderWidget *sliderWidget = nullptr;
    QWidget *progressW = nullptr,
            *spacingW = nullptr;
    QLabel *curLevelLabel = nullptr,
           *nextLevelLabel = nullptr;
    QProgressBar *progressBar = nullptr;
    QPushButton *btnEdit = nullptr,
                *btnApply = nullptr;
    QVector<Level*> *levelVector = nullptr;
    QDomNode _savedNode;
    QTimer *timer = nullptr;
    bool isAnimation = false;

    void initProgWidget();
    void setupGrade();
    void parseNode(const QDomNode &node);
    void initProgressBar();
    void updateProgressBar();
    void setEditMode(bool status);
    void initDataWidgets();
    void saveChanges();
    void checkRequiremets();
    int getDaysPassed();

private slots:
    void sliderNewIndex(int newIndex, int oldIndex);
};

#endif // PROGWIDGET_H
