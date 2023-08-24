#include "strategy.h"
#include "InputList.h"

int main(void){
	config cfg;
	cfg=getInit();
	switch(cfg.STRATEGY){
		case 1:
			FCFS(cfg);
			break;
		case 2:
			SSTF(cfg);
			break;
		case 3:
			SCAN(cfg);
	}
	printf("end");
}

