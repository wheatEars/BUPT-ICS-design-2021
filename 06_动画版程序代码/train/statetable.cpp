#include "statetable.h"
#include <QTableWidget>
#include <QListWidget>
#include <QStackedWidget>
#include <QHBoxLayout>
#include <QTableWidgetItem>
#include <QString>
#include <QLabel>
#include <QPushButton>
#include <QLineEdit>
#include <QHeaderView>
#include <QDebug>
#include "heads.h"

void StateTable::reloadStationList(){
    disconnect(stationList,0,0,0);
    stationList->clear();
    QListWidgetItem *glb = new QListWidgetItem("总览",stationList);
    glb->setBackgroundColor(QColor(255,255,0,255));
    glb->setSizeHint(QSize(50, 50));
    QFont font;
    font.setPointSize(20);
    glb->setFont(font);
    stationList->addItem(glb);
    stationList->setCurrentItem(glb);
    for(int i=1;i<=cfg.TOTAL_STATION;i++){
        QListWidgetItem *stationId = new QListWidgetItem("站台 "+QString::number(i),stationList);
        stationId->setFont(font);
        stationId->setSizeHint(QSize(50, 50));
        stationList->addItem(stationId);
    }
    connect(stationList,&QListWidget::currentItemChanged,[=](QListWidgetItem *current, QListWidgetItem *previous){
        current->setBackgroundColor(QColor(255,255,0,255));
        previous->setBackgroundColor(QColor(255,255,255,255));
        updateInfoList(stationList->currentRow());
    });
}
void StateTable::updateInfoList(int curRow){
    if(curRow==0){
        st->setCurrentIndex(0);
        globalData[0]->setText(QString::number(est.tick));
        globalData[1]->setText(QString::number(busCond.pos));
        switch(busCond.direction){
            case 0:
                globalData[2]->setText("顺时针");
                break;
            case 1:
                globalData[2]->setText("逆时针");
        }
        globalData[3]->setText(QString::number(busCond.target));
        globalData[4]->setText(QString::number(est.maxWaitingTime));
        globalData[5]->setText(QString::number(est.taskToBeDeal));
        globalData[6]->setText(QString::number(est.taskCompleted));

    }
    else{
        st->setCurrentIndex(1);
        for(int i=0;i<3;i++){
            stationData[i]->setText(QString::number(req[curRow][i]));
        }
    }
}

StateTable::StateTable(QWidget *parent,long long **req_,QLineEdit *line) : QWidget(parent),req(req_)
{
    setFixedSize(800,550);
    stationList = new QListWidget(this);//左侧选站栏
    connect(stationList,&QListWidget::currentItemChanged,[=](QListWidgetItem *current, QListWidgetItem *previous){
        current->setBackgroundColor(QColor(255,255,0,255));
        previous->setBackgroundColor(QColor(255,255,255,255));
        updateInfoList(stationList->currentRow());
    });
    stationList->setFixedSize(300,550);
    stationList->move(0,0);
    reloadStationList();

    QFont font;
    font.setPointSize(15);

    QTableWidget *stationInfo = new QTableWidget(this);//构建站台信息界面
    stationInfo->setFixedSize(500,550);
    stationInfo->setColumnCount(2);
    stationInfo->setRowCount(6);
    stationInfo->horizontalHeader()->setHidden(true);
    stationInfo->verticalHeader()->setHidden(true);
    stationInfo->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    stationInfo->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    QTableWidgetItem *item1 = new QTableWidgetItem("顺时针:");
    QTableWidgetItem *item2 = new QTableWidgetItem("逆时针:");
    QTableWidgetItem *item3 = new QTableWidgetItem("下车:");
    QTableWidgetItem *data1 = new QTableWidgetItem("0");
    QTableWidgetItem *data2 = new QTableWidgetItem("0");
    QTableWidgetItem *data3 = new QTableWidgetItem("0");
    QPushButton *pushBtn1 = new QPushButton("顺时针",stationInfo);
    QPushButton *pushBtn2 = new QPushButton("逆时针",stationInfo);
    QPushButton *pushBtn3 = new QPushButton("下车",stationInfo);
    QTableWidgetItem *btntext1 = new QTableWidgetItem("新建顺时针:");
    QTableWidgetItem *btntext2 = new QTableWidgetItem("新建逆时针:");
    QTableWidgetItem *btntext3 = new QTableWidgetItem("新建下车:");
    item1->setFont(font);
    item2->setFont(font);
    item3->setFont(font);
    data1->setFont(font);
    data2->setFont(font);
    data3->setFont(font);
    btntext1->setFont(font);
    btntext2->setFont(font);
    btntext3->setFont(font);
    pushBtn1->setFont(font);
    pushBtn2->setFont(font);
    pushBtn3->setFont(font);
    stationInfo->setItem(0,0,item1);
    stationInfo->setItem(1,0,item2);
    stationInfo->setItem(2,0,item3);
    stationInfo->setItem(0,1,data1);
    stationInfo->setItem(1,1,data2);
    stationInfo->setItem(2,1,data3);
    stationInfo->setItem(3,0,btntext1);
    stationInfo->setItem(4,0,btntext2);
    stationInfo->setItem(5,0,btntext3);
    stationInfo->setCellWidget(3,1,pushBtn1);
    stationInfo->setCellWidget(4,1,pushBtn2);
    stationInfo->setCellWidget(5,1,pushBtn3);
    stationData[0]=data3;
    stationData[1]=data1;
    stationData[2]=data2;
    connect(pushBtn1,&QPushButton::pressed,[=](){
        line->setText(QString("C ")+QString::number(stationList->currentRow(),10));
        emit line->returnPressed();
    });
    connect(pushBtn2,&QPushButton::pressed,[=](){
        line->setText(QString("U ")+QString::number(stationList->currentRow(),10));
        emit line->returnPressed();
    });
    connect(pushBtn3,&QPushButton::pressed,[=](){
        line->setText(QString("T ")+QString::number(stationList->currentRow(),10));
        emit line->returnPressed();
    });


    QTableWidget *globalInfo = new QTableWidget(this);//构建全局信息界面
    //globalInfo->setFixedSize(500,550);
    globalInfo->setColumnCount(2);
    globalInfo->setRowCount(7);
    globalInfo->horizontalHeader()->setHidden(true);
    globalInfo->verticalHeader()->setHidden(true);
    globalInfo->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    globalInfo->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    QTableWidgetItem *item4 = new QTableWidgetItem("时间:");
    QTableWidgetItem *item5 = new QTableWidgetItem("列车位置:");
    QTableWidgetItem *item6 = new QTableWidgetItem("行驶方向");
    QTableWidgetItem *item7 = new QTableWidgetItem("目的地");
    QTableWidgetItem *item8 = new QTableWidgetItem("最长候车:");
    QTableWidgetItem *item9 = new QTableWidgetItem("待完成:");
    QTableWidgetItem *item10 = new QTableWidgetItem("已完成:");
    QTableWidgetItem *data4 = new QTableWidgetItem("0");
    QTableWidgetItem *data5 = new QTableWidgetItem("0");
    QTableWidgetItem *data6 = new QTableWidgetItem("NULL");
    QTableWidgetItem *data7 = new QTableWidgetItem("0");
    QTableWidgetItem *data8 = new QTableWidgetItem("0");
    QTableWidgetItem *data9 = new QTableWidgetItem("0");
    QTableWidgetItem *data10 = new QTableWidgetItem("0");
    item4->setFont(font);
    item5->setFont(font);
    item6->setFont(font);
    item7->setFont(font);
    item8->setFont(font);
    item9->setFont(font);
    item10->setFont(font);
    data4->setFont(font);
    data5->setFont(font);
    data6->setFont(font);
    data7->setFont(font);
    data8->setFont(font);
    data9->setFont(font);
    data10->setFont(font);
    globalInfo->setItem(0,0,item4);
    globalInfo->setItem(1,0,item5);
    globalInfo->setItem(2,0,item6);
    globalInfo->setItem(3,0,item7);
    globalInfo->setItem(4,0,item8);
    globalInfo->setItem(5,0,item9);
    globalInfo->setItem(6,0,item10);
    globalInfo->setItem(0,1,data4);
    globalInfo->setItem(1,1,data5);
    globalInfo->setItem(2,1,data6);
    globalInfo->setItem(3,1,data7);
    globalInfo->setItem(4,1,data8);
    globalInfo->setItem(5,1,data9);
    globalInfo->setItem(6,1,data10);
    globalData[0]=data4;
    globalData[1]=data5;
    globalData[2]=data6;
    globalData[3]=data7;
    globalData[4]=data8;
    globalData[5]=data9;
    globalData[6]=data10;


    QStackedWidget *infoView = new QStackedWidget(this);//构建栈组件
    st=infoView;
    infoView->setFixedSize(500,550);
    infoView->move(300,0);
    infoView->addWidget(globalInfo);
    infoView->addWidget(stationInfo);

}
