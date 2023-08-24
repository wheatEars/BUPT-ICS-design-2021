#include "historylist.h"
#include <QString>
#include <iostream>
#include <QDebug>
using namespace std;
HistoryList::HistoryList(QWidget *parent,int len):QListWidget(parent),length(len)
{
}
void HistoryList::newCmd(QString cmd){
    QListWidgetItem *newItem = new QListWidgetItem(cmd,this);
    QFont font=newItem->font();
    font.setPointSize(20);
    newItem->setFont(font);
    addItem(newItem);
    if(count()>length){//维持缓存个数
        QListWidgetItem *del=takeItem(0);
        delete del;
    }
}
