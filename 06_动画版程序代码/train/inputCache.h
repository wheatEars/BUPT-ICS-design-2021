#include <string>
#include<queue>
using namespace std;
class inputCache{
	public:
		string readOut();
		void pushIn(string="clock");
		inputCache();
		~inputCache();
	private:
		queue<string> qu;
};
extern inputCache *cmdCache;
