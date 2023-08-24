#include "animatezone.h"
#include <QPainter>
#include <QString>
#include "heads.h"
#include <QDebug>
#include <QPropertyAnimation>
AnimateZone::AnimateZone(QWidget *parent) : QWidget(parent)
{
    trainWidget = new TrainWidget(this);
    trainWidget->move(30,85);
    move(100,50);
    setFixedSize(1240,200);

}
void AnimateZone::reset(){
    trainWidget->move(30,85);
    update();
}
void AnimateZone::trainMove(){
    QPropertyAnimation *setOff= new QPropertyAnimation(trainWidget,"pos");
    setOff->setDuration(2000);
    setOff->setStartValue(trainWidget->pos());
    setOff->setEndValue(QPoint(busCond.pos*((1140/cfg.TOTAL_STATION)/cfg.DISTANCE)+30,trainWidget->y()));
    setOff->start();

}
void AnimateZone::paintEvent(QPaintEvent *){

    QPainter painter(this);
    QPen pen;
    pen.setWidth(5);
    painter.setPen(pen);

    painter.drawRect(0,0,1240,200);//边框
    painter.drawLine( 50,150,1190,150);//轨道

    QRect tpDoor1(0,50,80,100);//画传送门
    QRect tpDoor2(1185,50,80,100);
    painter.drawImage(tpDoor1,QImage(":/image/teleportor.png"));
    painter.drawImage(tpDoor2,QImage(":/image/teleportor.png"));

    painter.translate(100,150);
    painter.save();
    int pixdist=1140/cfg.TOTAL_STATION;//画车站
    int posdist=pixdist/cfg.DISTANCE;
    for(int i=0;i<cfg.TOTAL_STATION;i++){
        painter.drawRect(-4,-4,4,4);
        for(int j=1;j<cfg.DISTANCE;j++){
            painter.drawRect(-2+posdist*j,-2,2,2);
        }
        painter.translate(pixdist,0);
    }

    painter.restore();
    painter.save();
    painter.translate(0,-100);
    for(int i=0;i<cfg.TOTAL_STATION;i++){//画站台号
        QString num(QString::number(i+1));
        QFont font;
        font.setPointSize(20);
        painter.setFont(font);
        painter.drawText(-10,0,num);
        painter.translate(pixdist,0);
    }
    painter.restore();
    painter.translate(0,35);
    for(int i=0;i<cfg.TOTAL_STATION;i++){//画箭头指示
        QRect count_arr(-25,-5,25,10);
        QRect target_arr(-5,-25,10,25);
        QRect clock_arr(0,-5,25,10);
        if(reqList[i+1][2]>0){
            painter.drawImage(count_arr,QImage(":/counter_arrow.png"));
        }
        if(reqList[i+1][0]>0){
            painter.drawImage(target_arr,QImage(":/target_arrow.png"));
        }
        if(reqList[i+1][1]>0){
            painter.drawImage(clock_arr,QImage(":/clock_arrow.png"));
        }
        painter.translate(pixdist,0);
    }
}
