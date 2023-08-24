#include "estimate.h"
#include <QDebug>
/*列车停车时调用taskDeal，此时可能到达终点，可能没到达终点*/
void taskDeal(long long **req,int n,int m, estPack* est){
    //列车没有到达终点时（此时m不可能为2）
    if(m!=3){
        if(req[n][m]!=0){
            long long time,waitingtime;
            time=req[n][m];
            req[n][m]=0;

            waitingtime=est->tick-time;
            if(waitingtime>(est->maxWaitingTime)){
                est->maxWaitingTime=waitingtime;
            }
            est->taskCompleted++;
            est->taskToBeDeal--;
        }

    }
    
    //列车到达终点时
    if(m==3){
        int i;
        for(i=0;i<=2;i++){
            if(req[n][i]!=0){
                long long time,waitingtime;
                time=req[n][i];
                req[n][i]=0;
                //qDebug()<<"deal"<<n<<":"<<i;
                waitingtime=est->tick-time;
                if(waitingtime>(est->maxWaitingTime)){
                    est->maxWaitingTime=waitingtime;
                }
                est->taskCompleted++;
                est->taskToBeDeal--;
            }
        }
    }    
}
