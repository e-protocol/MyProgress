#ifndef SLIDERWIDGET_H
#define SLIDERWIDGET_H

#pragma once
#include "src/resources/declaration.h"

/*
 * this class is used to create a widget with sliding functions,
 * widgets can be swing left/right by btn press
 */
class SliderWidget : public QWidget
{
    Q_OBJECT
public:
    SliderWidget(QVector<QWidget*> *widgetVector, int activeIndex);
    ~SliderWidget();
    void updateSliderWidget() { moveWidgets(_activeIndex, false); }

    //setters
    void setLevel(int newLevel);
    void blockInteraction(bool status);

private:
    QVector<QWidget*> *_widgetVector = nullptr;
    int _activeIndex = 0,
        prevIndex = 0;
    const int wWidth = 110,
              wHeight = 20,
              animeDuration = 100,
              fontMinSize = 18,
              fontMaxSize = 28,
              timerPause = 3,
              btnSide = 30;
    QScrollArea *scrollArea = nullptr;
    QPushButton *btnLeft = nullptr,
                *btnRight = nullptr;

    void initSliderWidget();
    void moveWidgets(int index, bool enableAnimation);
    void paintEvent(QPaintEvent* event);

signals:
    void indexChanged(int newIndex, int oldIndex);
};

#endif // SLIDERWIDGET_H
