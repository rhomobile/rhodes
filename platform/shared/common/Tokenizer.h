#ifndef _RHOTOKENIZER_H_
#define _RHOTOKENIZER_H_

#include "RhoStd.h"

namespace rho{
namespace common{

class CTokenizer 
{
	String m_str;
	String m_delims;
	int m_length;
	int m_position;

public:
    CTokenizer(String str,String delims);
	String nextToken();
    boolean hasMoreTokens();

    int getCurPos(){ return m_position; }
private:
  	void eatDelimeters();

};

}
}

#endif //_RHOCONF_H_
