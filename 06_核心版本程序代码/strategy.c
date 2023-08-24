#include "strategy.h"
#include "printMap.h"
#include "inputList.h"
#include "estimate.h"
estPack initEstPack(){
	estPack e={};
	e.tick=0;
	return e;
}

void freeLL(node *head){
	node *p;
	while(head!=NULL){
		p=head;
		head=head->next;
		free(p);
	}
}
void FCFS(config cfg){
	long long reqList[30][3]={};
	reqList[0][0]=cfg.TOTAL_STATION;
	node *reqOrder=(node*)malloc(sizeof(node));
	reqOrder->next=NULL;
	busInfo busCond;
	busCond.con=SPARE;
	estPack est=initEstPack();
	cmdPack outerCmd;
	int actFlag=0;//为1代表已经决定了本回合的行动，不收后续指令影响
	printMap(reqList,busCond,est);
	est.tick++;
	while(outerCmd=getCommand(),outerCmd.state!=CMD_END){
		if(busCond.con==SPARE){//空闲状态 寻找目标
			//先处理请求
			while(reqOrder->next!=NULL&&reqOrder->next->des==busCond.pos/(cfg.DISTANCE)+1){//该站点的请求最先发出可以完成
				taskDeal(reqList,reqOrder->next->des,reqOrder->next->dir,&est);
				node *p=reqOrder->next;
				reqOrder->next=p->next;
				free(p);
			}
			
			if(!actFlag&&reqOrder->next!=NULL){//然后执行状态转移
				busCond.target=reqOrder->next->des;
				busCond.con=MOVING;
			}
		}
		if(busCond.con==MOVING){
			int targetPos=(busCond.target-1)*(cfg.DISTANCE);//目的地的坐标表示
			long long sumLen=cfg.TOTAL_STATION*cfg.DISTANCE;//轨道总长度
			if(actFlag==0){
				if(targetPos==busCond.pos){//到站
					busCond.target=0;
					busCond.con=SPARE;
					actFlag=1;
					while(reqOrder->next!=NULL&&reqOrder->next->des==busCond.pos/(cfg.DISTANCE)+1){//该站点的请求最先发出可以完成
						taskDeal(reqList,reqOrder->next->des,reqOrder->next->dir,&est);
						node *p=reqOrder->next;
						reqOrder->next=p->next;
						free(p);
					}
				}
				else if(actFlag!=1){//若actflag真，这一秒已经选择了停车或者行驶，这会耗尽这一秒，不能进行其他行动。
					if((targetPos-busCond.pos+sumLen)%sumLen>(busCond.pos-targetPos+sumLen)%sumLen){//逆时针走 判断依据是：顺时针距离是否超过全程的一半。
						busCond.direction=1;
						busCond.pos=(sumLen+busCond.pos-1)%sumLen;
					}
					else{//顺时针走
						busCond.direction=0;
						busCond.pos=(sumLen+busCond.pos+1)%sumLen;
					}
					actFlag=1;
				}	
			}	
		}
		switch(outerCmd.state){
			case CMD_TARGET://target指令
				if(reqList[outerCmd.station][0]==0){//该位置不存在未完成的target请求
					reqList[outerCmd.station][0]=est.tick;//在数组中记录该请求建立时间
					node *p=(node*)malloc(sizeof(node));
					p->dir=0;//向顺序链表中加入该请求
					p->des=outerCmd.station;
					p->next=NULL;
					node *ptr=reqOrder;
					while(ptr->next!=NULL){
						ptr=ptr->next;
					}
					ptr->next=p;
					est.taskToBeDeal++;//待办请求加一
				}
				break;
			case CMD_CLOCKWISE://clockwise指令
				if(reqList[outerCmd.station][1]==0){//该位置不存在未完成的clockwise请求
					reqList[outerCmd.station][1]=est.tick;
					node *p=(node*)malloc(sizeof(node));
					p->dir=1;//向顺序链表中加入该请求
					p->des=outerCmd.station;
					p->next=NULL;
					node *ptr=reqOrder;
					while(ptr->next!=NULL){
						ptr=ptr->next;
					}
					ptr->next=p;
					est.taskToBeDeal++;//待办请求加一
				}
				break;
			case CMD_CON_CLOCKWISE://counterclockwise指令
				if(reqList[outerCmd.station][2]==0){//该位置不存在未完成的clockwise请求
					reqList[outerCmd.station][2]=est.tick;
					node *p=(node*)malloc(sizeof(node));
					p->dir=2;//向顺序链表中加入该请求
					p->des=outerCmd.station;
					p->next=NULL;
					node *ptr=reqOrder;
					while(ptr->next!=NULL){
						ptr=ptr->next;
					}
					ptr->next=p;
					est.taskToBeDeal++;//待办请求加一
				}
				break;
			case CMD_CLOCK://clock指令 
				actFlag=0;
				printMap(reqList,busCond,est);
				est.tick++;
		}
	}
	freeLL(reqOrder);
	return;
}

void SSTF(config cfg){//最短优先
	long long reqList[30][3]={};//请求存储数组
	reqList[0][0]=cfg.TOTAL_STATION;//在[0][0]赋值总站数
	busInfo busCond;
	busCond.con=SPARE;//初始状态为
	estPack est=initEstPack();
	cmdPack outerCmd;
	int actFlag=0;//为1代表已经决定了本回合的行动，不收后续指令影响
	printMap(reqList,busCond,est);
	est.tick++;
	while(outerCmd=getCommand(),outerCmd.state!=CMD_END){
		if(busCond.con==SPARE){//空闲状态 寻找目标
			int busStation=(busCond.pos/cfg.DISTANCE)+1;
			//先处理请求
			taskDeal(reqList,busStation,3,&est);
			//找最近的请求	
			if(outerCmd.state==CMD_CLOCK){		
				for(int i=1;i<=cfg.TOTAL_STATION/2;i++){
					int index1=(busStation-1+i)%cfg.TOTAL_STATION+1;
					int index2=(busStation-1+cfg.TOTAL_STATION-i)%cfg.TOTAL_STATION+1;
					for(int j=0;j<=2;j++){
						if(reqList[index1][j]!=0){//找到顺时针最近位置存在请求
							busCond.target=index1;
							break;
						}
					}
					if(busCond.target!=0){
						break;
					}
					for(int j=0;j<=2;j++){
						if(reqList[index2][j]!=0){//找到逆时针最近位置存在请求
							busCond.target=index2;
							break;
						}
					}
					if(busCond.target!=0){
						break;
					}
				}
			}
			if(!actFlag&&busCond.target!=0){//如果这一秒没行动过就起步
				busCond.con=MOVING;
			}
		}
		if(busCond.con==STOPPED){//停车状态，等待时钟信号发出后继续行动，期间处理一切顺路请求
			int busStation=(busCond.pos/cfg.DISTANCE)+1;
			taskDeal(reqList,busStation,0,&est);//target
			taskDeal(reqList,busStation,busCond.direction+1,&est);//对应方向的请求
			if(outerCmd.state==CMD_CLOCK){
				busCond.con=MOVING;
			}
		}
		if(busCond.con==MOVING){
			int targetPos=(busCond.target-1)*(cfg.DISTANCE);//目的地的坐标表示
			long long sumLen=(cfg.TOTAL_STATION)*(cfg.DISTANCE);//轨道总长度
			int busStation=(busCond.pos/cfg.DISTANCE)+1;
			if(actFlag!=1){
				if(targetPos==busCond.pos){//到站
					busCond.target=0;
					busCond.con=SPARE;
					actFlag=1;
					taskDeal(reqList,busStation,3,&est);
				}
				else if(busCond.pos%cfg.DISTANCE==0){//列车进站
					if(reqList[busStation][0]!=0){//存在target请求
						busCond.con=STOPPED;
						actFlag=1;
						taskDeal(reqList,busStation,0,&est);	
					}
					if(reqList[busStation][1]!=0&&busCond.direction==0){//存在clockwise请求
						busCond.con=STOPPED;
						actFlag=1;
						taskDeal(reqList,busStation,1,&est);	
					}
					if(reqList[busStation][2]!=0&&busCond.direction==1){//存在clockwise请求
						busCond.con=STOPPED;
						actFlag=1;
						taskDeal(reqList,busStation,2,&est);	
					}
				}
				if(!actFlag){//不停车 继续走
					if((targetPos-busCond.pos+sumLen)%sumLen>(busCond.pos-targetPos+sumLen)%sumLen){//逆时针走 判断依据是：顺时针距离是否超过全程的一半。
						busCond.direction=1;
						busCond.pos=(sumLen+busCond.pos-1)%sumLen;
					}
					else{//顺时针走
						busCond.direction=0;
						busCond.pos=(sumLen+busCond.pos+1)%sumLen;
					}
					actFlag=1;
				}	
			}
		}
		switch(outerCmd.state){
			case CMD_TARGET://target指令
				if(reqList[outerCmd.station][0]==0){//该位置不存在未完成的target请求
					reqList[outerCmd.station][0]=est.tick;//在数组中记录该请求建立时间
					est.taskToBeDeal++;//待办请求加一
				}
				break;
			case CMD_CLOCKWISE://clockwise指令
				if(reqList[outerCmd.station][1]==0){//该位置不存在未完成的clockwise请求
					reqList[outerCmd.station][1]=est.tick;
					est.taskToBeDeal++;//待办请求加一
				}
				break;
			case CMD_CON_CLOCKWISE://counterclockwise指令
				if(reqList[outerCmd.station][2]==0){//该位置不存在未完成的clockwise请求
					reqList[outerCmd.station][2]=est.tick;
					est.taskToBeDeal++;//待办请求加一
				}
				break;
			case CMD_CLOCK://clock指令 
				actFlag=0;
				printMap(reqList,busCond,est);
				est.tick++;
		}
	}
}
void SCAN(config cfg){
	long long reqList[30][3]={};
	reqList[0][0]=cfg.TOTAL_STATION;
	busInfo busCond;
	busCond.con=SPARE;
	estPack est=initEstPack();
	cmdPack outerCmd;
	int actFlag=0;//为1代表已经决定了本回合的行动，不收后续指令影响
	printMap(reqList,busCond,est);
	est.tick++;
	int spareOrRunning=0;
	while(outerCmd=getCommand(),outerCmd.state!=CMD_END){
		if(busCond.con==SPARE){//空闲状态 寻找目标
			int busStation=(busCond.pos/cfg.DISTANCE)+1;
			//先处理请求
			taskDeal(reqList,busStation,3,&est);
			//找最近的请求	
			if(outerCmd.state==CMD_CLOCK){
				if(spareOrRunning==0){//前一秒由于没有请求处于空闲状态
					for(int i=1;i<=cfg.TOTAL_STATION/2;i++){//最短寻路，优先顺时针
						int index1=(busStation-1+i)%cfg.TOTAL_STATION+1;
						int index2=(busStation-1+cfg.TOTAL_STATION-i)%cfg.TOTAL_STATION+1;
						for(int j=0;j<=2;j++){
							if(reqList[index1][j]!=0){//找到顺时针最近位置存在请求
								busCond.target=index1;
								break;
							}
						}
						if(busCond.target!=0){
							spareOrRunning=1;
							break;
						}
						for(int j=0;j<=2;j++){
							if(reqList[index2][j]!=0){//找到逆时针最近位置存在请求
								busCond.target=index2;
								break;
							}
						}
						if(busCond.target!=0){
							spareOrRunning=1;
							break;
						}
					}
				}
				else{//运行中，优先当前行进方向
					int reqExi=0;//是否存在请求
					for(int i=1;i<=cfg.TOTAL_STATION;i++){
						for(int j=0;j<=2;j++){
							if(reqList[i][j]!=0){
								reqExi=1;
								break;
							}
						}
						if(reqExi==1){
							break;
						}
					}
					if(reqExi==0){//没请求可做
						spareOrRunning=0;
					}
					else{//可以继续做
						if(busCond.direction==0){//顺时针行驶
							for(int i=1;i<=cfg.TOTAL_STATION/2;i++){//顺时针找一半路程
								int index1=(busStation-1+i)%cfg.TOTAL_STATION+1;//车站号
								for(int j=0;j<=2;j++){
									if(reqList[index1][j]!=0){//找到顺时针最近位置存在请求
										busCond.target=index1;
										break;
									}
								}
								if(busCond.target!=0){//及时退出 防止被更远的覆盖
									break;
								}
							}
							if(busCond.target==0){//顺时针没找到
								for(int i=1;i<=cfg.TOTAL_STATION/2;i++){//逆时针找一半路程
									int index2=(busStation-1+cfg.TOTAL_STATION-i)%cfg.TOTAL_STATION+1;
									for(int j=0;j<=2;j++){
										if(reqList[index2][j]!=0){//找到逆时针最近位置存在请求
											busCond.target=index2;
											break;
										}
									}
									if(busCond.target!=0){
										break;
									}
								}
							}
						}
						else{//逆时针行驶
							for(int i=1;i<=cfg.TOTAL_STATION/2;i++){//逆时针找一半路程
								int index1=(busStation-1+cfg.TOTAL_STATION-i)%cfg.TOTAL_STATION+1;//车站号
								for(int j=0;j<=2;j++){
									if(reqList[index1][j]!=0){//找到逆时针最近位置存在请求
										busCond.target=index1;
										break;
									}
								}
								if(busCond.target!=0){//及时退出 防止被更远的覆盖
									break;
								}
							}
							if(busCond.target==0){//逆时针没找到
								for(int i=1;i<=cfg.TOTAL_STATION/2;i++){//顺时针找一半路程
									int index2=(busStation-1+i)%cfg.TOTAL_STATION+1;
									for(int j=0;j<=2;j++){
										if(reqList[index2][j]!=0){//找到顺时针最近位置存在请求
											busCond.target=index2;
											break;
										}
									}
									if(busCond.target!=0){
										break;
									}
								}
							}
						}
					}		
				}	
			}
			if(!actFlag&&busCond.target!=0){//如果这一秒没行动过就起步
				busCond.con=MOVING;
			}
		}
		int justTakeOff=0;
		if(busCond.con==STOPPED){//停车状态，等待时钟信号发出后继续行动，期间处理一切请求
			// int busStation=(busCond.pos/cfg.DISTANCE)+1;
			// taskDeal(reqList,busStation,3,&est);
			if(outerCmd.state==CMD_CLOCK){
				busCond.con=MOVING;
				justTakeOff=1;
			}
		}
		if(busCond.con==MOVING){
			int targetPos=(busCond.target-1)*(cfg.DISTANCE);//目的地的坐标表示
			long long sumLen=(cfg.TOTAL_STATION)*(cfg.DISTANCE);//轨道总长度
			int busStation=(busCond.pos/cfg.DISTANCE)+1;
			if(actFlag!=1){
				if(justTakeOff==0){
					if(targetPos==busCond.pos){//到终点站
						busCond.target=0;
						busCond.con=SPARE;
						actFlag=1;
						taskDeal(reqList,busStation,3,&est);
					}
					else if(busCond.pos%cfg.DISTANCE==0){//列车进站
						for(int i=0;i<=2;i++){
							if(reqList[busStation][i]!=0){//该站有请求
								busCond.con=STOPPED;
								actFlag=1;
								taskDeal(reqList,busStation,3,&est);
								break;
							}
						}
					}
				}
				justTakeOff=0;
				if(!actFlag){//不停车 继续走
					if((targetPos-busCond.pos+sumLen)%sumLen>(busCond.pos-targetPos+sumLen)%sumLen){//逆时针走 判断依据是：顺时针距离是否超过全程的一半。
						busCond.direction=1;
						busCond.pos=(sumLen+busCond.pos-1)%sumLen;
					}
					else{//顺时针走
						busCond.direction=0;
						busCond.pos=(sumLen+busCond.pos+1)%sumLen;
					}
					actFlag=1;
				}	
			}
		}
		switch(outerCmd.state){
			case CMD_TARGET://target指令
				if(reqList[outerCmd.station][0]==0){//该位置不存在未完成的target请求
					reqList[outerCmd.station][0]=est.tick;//在数组中记录该请求建立时间
					est.taskToBeDeal++;//待办请求加一
				}
				break;
			case CMD_CLOCKWISE://clockwise指令
				if(reqList[outerCmd.station][1]==0){//该位置不存在未完成的clockwise请求
					reqList[outerCmd.station][1]=est.tick;
					est.taskToBeDeal++;//待办请求加一
				}
				break;
			case CMD_CON_CLOCKWISE://counterclockwise指令
				if(reqList[outerCmd.station][2]==0){//该位置不存在未完成的clockwise请求
					reqList[outerCmd.station][2]=est.tick;
					est.taskToBeDeal++;//待办请求加一
				}
				break;
			case CMD_CLOCK://clock指令 
				actFlag=0;
				printMap(reqList,busCond,est);
				est.tick++;
		}
	}
}
