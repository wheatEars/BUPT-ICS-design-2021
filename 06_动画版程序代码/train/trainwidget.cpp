#include "trainwidget.h"
#include "heads.h"
#include <QPainter>
TrainWidget::TrainWidget(QWidget *parent) : QWidget(parent)
{

    setFixedSize(146,73);
}
void TrainWidget::paintEvent(QPaintEvent *){
    QPainter painter(this);
    QRect trainRect(-2,-2,150,75);
    painter.drawRect(trainRect);
    QImage img(":/image/bus.png");

    if(busCond.direction==0){
        painter.drawImage(trainRect,img.mirrored(true,false));
    }
    else{
        painter.drawImage(trainRect,img);

    }
}
