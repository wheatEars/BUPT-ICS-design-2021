#include "corethread.h"
#include "heads.h"
#include "strategy.h"
#include <QDebug>
CoreThread::CoreThread(QObject *parent):QThread(parent)

{

}
void CoreThread::run(){
    switch(cfg.STRATEGY){
        case 1:
            FCFS();
            break;
        case 2:
            SSTF();
            break;
        case 3:
            SCAN();
    }
    quit();
}
