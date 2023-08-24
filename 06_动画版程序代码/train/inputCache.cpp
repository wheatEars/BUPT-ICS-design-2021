#include <string>
#include "inputCache.h"
using namespace std;
string inputCache::readOut(){
    if(qu.empty()){
		return "NULL";
	}
	else{
        string res=qu.front();
        qu.pop();
		return res;
	}
}

void inputCache::pushIn(string cmd){
    qu.push(cmd);
}
inputCache::inputCache(){}

inputCache::~inputCache(){}
