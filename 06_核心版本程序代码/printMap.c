#include "printMap.h"
void printMap(long long req[30][3],busInfo busCond,estPack est)
{
	printf("TIME:%lld\n",est.tick);
	printf("BUS:\n");
	printf("position:%d\n",busCond.pos);
	printf("target:");
	int i=0,j=0;
	long long p=req[0][0];
	for(j=0,i=1;i<=p;i++){
		if(req[i][j]!=0){
			printf("1");
		}
		else{
			printf("0");
		}
	}
	printf("\n");
	printf("STATION:\n");
	printf("clockwise:");
	for(j=1,i=1;i<=p;i++){
		if(req[i][j]!=0){
			printf("1");
		}
		else{
			printf("0");
		}
	}
	printf("\n");
	printf("counterclockwise:");
	for(j=2,i=1;i<=p;i++){
		if(req[i][j]!=0){
			printf("1");
		}
		else{
			printf("0");
		}
	}
	printf("\n");
}