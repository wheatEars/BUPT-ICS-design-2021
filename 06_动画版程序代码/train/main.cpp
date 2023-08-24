#include "mainwindow.h"
#include "heads.h"
#include <QApplication>

config cfg={5,1,2};
busInfo busCond={0,0,0,0};
estPack est={};
long long **reqList=nullptr;
int main(int argc, char *argv[])
{
    reqList=(long long **)malloc(sizeof(long long *)*31);
    for(int i=0;i<31;i++){
        reqList[i]=(long long *)malloc(sizeof(long long)*3);
        memset(reqList[i],0,sizeof(long long)*3);
    }
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}
