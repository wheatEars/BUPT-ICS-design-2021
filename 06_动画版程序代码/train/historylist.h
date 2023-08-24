#ifndef HISTORYLIST_H
#define HISTORYLIST_H
#include <QListWidget>
#include <string>
#include <QString>
using namespace std;
class HistoryList : public QListWidget
{
public:
    void newCmd(QString cmd);
    HistoryList(QWidget *parent = Q_NULLPTR,int len = 10);
private:
    int length;//缓存的最大指令数
};

#endif // HISTORYLIST_H
