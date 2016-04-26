#include <common/RhodesApp.h>
using namespace std;
using namespace rho;

enum CMD
{
	Register,
	Unregister
};

class CAnsSync : common::CThreadQueue
{
public:
	void sendCommand(int cmd);
	CAnsSync(); 
	virtual ~CAnsSync();
	struct AnsCommand : public IQueueCommand
	{
		AnsCommand(int cmd){m_cmd = cmd;};
		int m_cmd;

		bool equals(IQueueCommand const &) {return false;}
		String toString() {return "";}
	};
	static CAnsSync* Create();
	static void Destroy();
	static CAnsSync* getInstance() { return m_pInstance; }
private:
	static CAnsSync* m_pInstance;
	void processCommand(IQueueCommand* pCmd);
};

