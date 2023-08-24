#include "InputList.h"
#include "inputCache.h"
#include <string>
#include <QDebug>
#include <stdlib.h>
#include <windows.h>
using namespace std;
config getInit(string filePath)
{
	config ptr;
    ptr.DISTANCE=0;
    ptr.STRATEGY=0;
    ptr.TOTAL_STATION=0;
    FILE *fp = fopen(filePath.c_str(),"r");
	char b=0;//每行的首个字符
	char x=0;//不断向后读取暂存于此
	int a=0;//暂存x 
	int p=0;//暂存数据
	while(!feof(fp)){
		while(!feof(fp)){
			b=fgetc(fp); //每行的第一个字符
			if(b=='#'){
				while(b!='\n'){
					b=fgetc(fp);
				}
			}
			else{
				break;
			}
		}
		if(b=='T'){//TOTAL_STATION
			while(x!=' '){
				x=fgetc(fp);
			}
			x=fgetc(fp);//等号
			x=fgetc(fp);//空格
			x=fgetc(fp);//第一位数字
			p = x-48;
			if(p==1||p==2){
				x=fgetc(fp);
				if(x!='\n'){
					a = x-48;
					if(x=='0'){
						p=p*10;
					}
					else{
						p=p*10+a;
					}
				}
				else{
					p=p;
				}
				fseek(fp,-1,1);
			}
			ptr.TOTAL_STATION=p;
			while(x!='\n'&&x!=EOF){
				x=fgetc(fp);
			}
			//printf("%d\n",ptr.TOTAL_STATION);
		}
		else if(b=='S'){//STRATEGY
			while(x!=' '){
				x=fgetc(fp);
			}
			x=fgetc(fp);//等号
			x=fgetc(fp);//空格
			x=fgetc(fp);//首字母
			if(x=='F'){
				ptr.STRATEGY=1;//FCFS
			}
			else{
				x=fgetc(fp);//第二个字母
				if(x=='S'){
					ptr.STRATEGY=2;//SSTF
				}
				else{
					ptr.STRATEGY=3;//SCAN
				}
			}
			//printf("%d\n",ptr.STRATEGY);
			while(x!='\n'&&x!=EOF){
				x=fgetc(fp);
			}
		}
		else if(b=='D'){//DISTANCE
			while(x!=' '){
				x=fgetc(fp);
			}
			x=fgetc(fp);//等号
			x=fgetc(fp);//空格
			x=fgetc(fp);//此字符数字表示距离
			p=x-48;
			ptr.DISTANCE = p;
			//printf("%d\n",ptr.DISTANCE);
			while(x!='\n'&&x!=EOF){
				x=fgetc(fp);
			}
		}
	}
	fclose(fp);
	//以下为缺省值
	if(ptr.DISTANCE==0){
		ptr.DISTANCE=2;
	}
	else{
		ptr.DISTANCE=ptr.DISTANCE;
	}
	if(ptr.STRATEGY==0){
		ptr.STRATEGY=1;
	}
	else{
		ptr.STRATEGY=ptr.STRATEGY;
	}
	if(ptr.TOTAL_STATION==0){
		ptr.TOTAL_STATION=5;
	}
	else{
		ptr.TOTAL_STATION=ptr.TOTAL_STATION;
	}
	return ptr;
}
cmdPack getCommand()
{
    cmdPack inputing;//设置储存输入的结构体
    inputing.station=0;
    inputing.state=0;
    string cmd;
    while(cmd=cmdCache->readOut(),cmd=="NULL"){
        Sleep(100);
    }
    int state=0;//定义状态
	int station=0;//目标站台号 
    if(cmd[0]=='E'||cmd[0]=='e'){//end
		state=0;
	}
    else if(cmd=="clock"){//clock
		state=1;
	}
    else if(cmd[0]=='T'||cmd[0]=='t'){//target
		state=2;
        station=(int)strtol(cmd.c_str()+2,nullptr,10);
	}
    else if(cmd[0]=='C'||cmd[0]=='c'){//clockwise
		state=3;
        station=(int)strtol(cmd.c_str()+2,nullptr,10);
	}
	else{//counterclockwise
		state=4;
        station=(int)strtol(cmd.c_str()+2,nullptr,10);
	}
	inputing.station=station;
	inputing.state=state;
	//printf("%d\n",inputing.state);
	//printf("%d\n",inputing.station);
    return inputing;
}
