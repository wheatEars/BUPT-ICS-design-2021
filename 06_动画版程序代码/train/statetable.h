#ifndef STATETABLE_H
#define STATETABLE_H

#include <QWidget>
#include <QTableWidgetItem>
#include <QStackedWidget>
#include <QListWidget>
#include "heads.h"
#include <QTimerEvent>

class StateTable : public QWidget
{
    Q_OBJECT
private:
    long long **req;
    QTableWidgetItem *stationData[3];
    QTableWidgetItem *globalData[7];
public:

    QStackedWidget *st;
    QListWidget *stationList;
    explicit StateTable(QWidget *parent = nullptr,long long **req_=nullptr,QLineEdit *lineedit=nullptr);
    void reloadStationList();
    void updateInfoList(int curRow=0);
};

#endif // STATETABLE_H
