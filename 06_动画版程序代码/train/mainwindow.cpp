#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "cfginputdialog.h"
#include "inputCache.h"
#include "historylist.h"
#include "statetable.h"
#include "animatezone.h"
#include "heads.h"
#include "InputList.h"
#include "corethread.h"
#include <QFileDialog>
#include <QDebug>
#include <string>
#include <QString>
#include <QDockWidget>
#include <QInputDialog>
#include <QStringList>
#include <deque>
#include <iostream>
#include <stdlib.h>
#include <bits.h>


using namespace std;




//实例化指令缓存
inputCache *cmdCache=new inputCache;




MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    //动画区
    az = new AnimateZone(this);

    //建立核心进程
    CoreThread *thread = new CoreThread(this);



    //构建历史记录栏
    HistoryList *his=new HistoryList(this);
    his->move(950,300);
    his->setFixedSize(400,500);

    //读取输入框部分
    QPushButton *subBtn=ui->submitBtn;
    cmdInput=ui->cmdInputEdit;
    connect(subBtn,&QPushButton::pressed,[=](){
       cmdCache->pushIn(cmdInput->text().toStdString());
       his->newCmd(cmdInput->text());
       cmdInput->clear();
    });
    connect(cmdInput,&QLineEdit::returnPressed,subBtn,&QPushButton::pressed);

    //设置输入框颜色
    QPalette palette = cmdInput->palette();
    palette.setColor(QPalette::Base, Qt::lightGray);
    ui->cmdInputEdit->setPalette(palette);

    //设置状态栏

    stateTable = new StateTable(this,reqList,cmdInput);
    stateTable->move(100,300);
    stateTable->setFixedSize(800,550);


    //控制按钮
    QPushButton *startBtn = new QPushButton("开始",this);
    QPushButton *pauseBtn = new QPushButton("停止",this);
    QPushButton *resetBtn = new QPushButton("重置",this);
    startBtn->resize(75,50);
    pauseBtn->resize(75,50);
    resetBtn->resize(75,50);
    startBtn->move(1350,50);
    pauseBtn->move(1350,125);
    resetBtn->move(1350,200);
    connect(startBtn,&QPushButton::pressed,[=](){
        thread->start();
        //设置时钟
        timerId1=startTimer(4000);
        timerId2=startTimer(500);
        az->timerId=timerId2;

    });
    connect(pauseBtn,&QPushButton::pressed,[=](){
        cmdInput->setText("end");
        killTimer(timerId1);
        killTimer(timerId2);
        emit cmdInput->returnPressed();
    });
    connect(resetBtn,&QPushButton::pressed,[=](){
        est={};
        cfg={5,1,2};
        busCond={};
        for(int i=0;i<=30;i++){
            for(int j=0;j<=2;j++){
                reqList[i][j]=0;
            }
        }
        stateTable->reloadStationList();
        stateTable->updateInfoList(0);
        stateTable->stationList->setCurrentRow(0);
        az->reset();
    });


    //打开配置对话框
    QAction *actionSetCfg=ui->actionSetCfg;
    connect(actionSetCfg,&QAction::triggered,[=](){
        CfgInputDialog dlg;
        dlg.setWindowTitle("手动配置");
        connect(&dlg,&CfgInputDialog::passed,stateTable,&StateTable::reloadStationList);
        dlg.resize(300,200);
        dlg.exec();
    });

    //打开配置文件
    QAction *actionFileInput=ui->actionFileInput;
    connect(actionFileInput,&QAction::triggered,[=](){
        QFileDialog dlg;
        QString fileName = QFileDialog::getOpenFileName(this, tr("选择配置文件"),
            "/dict",
            tr("Config file (dict.dic)")
        );
        cfg=getInit(fileName.toStdString());
        stateTable->reloadStationList();
    });

}
MainWindow::~MainWindow()
{
    delete ui;
}
void MainWindow::timerEvent(QTimerEvent *e){
    if(e->timerId()==timerId1){
        cmdInput->setText("clock");
        emit cmdInput->returnPressed();
        Sleep(300);
        az->trainMove();
    }
    if(e->timerId()==timerId2){
        az->update();
        stateTable->updateInfoList(stateTable->stationList->currentRow());
    }
}
