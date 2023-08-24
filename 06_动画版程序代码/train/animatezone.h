#ifndef ANIMATEZONE_H
#define ANIMATEZONE_H

#include <QWidget>
#include "trainwidget.h"
#include <QTimerEvent>

class AnimateZone : public QWidget
{
    Q_OBJECT
private:
    TrainWidget *trainWidget;
public:
    int timerId;
    explicit AnimateZone(QWidget *parent = nullptr);
    void paintEvent(QPaintEvent *event);
    //void timerEvent(QTimerEvent *e);
    void trainMove();
    void reset();

signals:

};

#endif // ANIMATEZONE_H
