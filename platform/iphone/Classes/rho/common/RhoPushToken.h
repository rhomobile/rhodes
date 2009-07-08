#pragma once

#ifdef __cplusplus

#include "logging/RhoLog.h"
#include "common/RhoThread.h"
#include "RhoClassfactory.h"

namespace rho{
namespace common{

class CRhoPushToken : public common::CRhoThread
{
    DEFINE_LOGCLASS;
public:
	static CRhoPushToken* create(IRhoClassFactory* factory);
	static CRhoPushToken* getInstance() { return m_pInstance; }
	
	virtual void run();	

	void set(char* data);
private:	
	CRhoPushToken(common::IRhoClassFactory* factory);
	
	static CRhoPushToken* m_pInstance;
	String m_token;
	bool   m_set;

	bool post_token();
};

}
}

#endif //__cplusplus

#ifdef __cplusplus
extern "C" {
#endif //__cplusplus
	
void rho_push_token_create();
void rho_push_token_set(char* data);
	

#ifdef __cplusplus
};
#endif //__cplusplus
