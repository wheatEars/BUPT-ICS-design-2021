#ifndef _HEADS_H_
#define _HEADS_H_
/*----------头文件引用----------*/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <limits.h>
/*----------常量定义----------*/
#define CMD_END 0
#define CMD_CLOCK 1
#define CMD_TARGET 2
#define CMD_CLOCKWISE 3
#define CMD_CON_CLOCKWISE 4
#define SPARE 0
#define MOVING 1
#define STOPPED 2
#define NO_REQUIRE 0//车站无请求
#define NO_DIRECTION 0//请求无向
#define CLOCKWISE 1//请求顺时针||车站有顺时针请求
#define CON_CLOCKWISE 2//请求逆时针||车站有逆时针请求
#define BOTH_DIRECTION 3//车站有双向的请求
/*----------数据结构定义----------*/
typedef struct{
	int TOTAL_STATION;
	int STRATEGY;
	int DISTANCE; 
} config;//配置结构体

typedef struct busInfo{
	int pos;
	int direction;
	int con;
	int target;
} busInfo;//公交状态

typedef struct{
	long long tick;
	long long maxWaitingTime;
	int taskCompleted;
	int taskToBeDeal;
} estPack;

typedef struct lnode{
	int des;
	int dir;
	struct lnode *next;
} node;

typedef struct{
	int state;//end:0;clock:1;target:2;clockwise:3;counterclockwise:4.
	int station;
} cmdPack;
#endif