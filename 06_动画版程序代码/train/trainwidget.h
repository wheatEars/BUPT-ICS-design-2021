#ifndef TRAINWIDGET_H
#define TRAINWIDGET_H

#include <QWidget>

class TrainWidget : public QWidget
{
    Q_OBJECT
public:
    explicit TrainWidget(QWidget *parent = nullptr);
    void paintEvent(QPaintEvent *event);
signals:

};

#endif // TRAINWIDGET_H
