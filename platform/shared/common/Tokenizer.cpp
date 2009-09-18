#include "Tokenizer.h"

namespace rho{
namespace common{

CTokenizer::CTokenizer(String str,String delims) {
	m_delims = delims;
	m_str = str;
	m_length = str.length();
	m_position = 0;
}
	
String CTokenizer::nextToken() 
{
	eatDelimeters();
	int start = m_position;
	while (m_position<m_length && m_delims.find(m_str.at(m_position)) == String::npos ) {
		m_position++;
	}
	
	return m_str.substr(start,m_position-start);
}
	
void CTokenizer::eatDelimeters() 
{
	while (m_position<m_length) {
		char c = m_str.at(m_position);
		if (m_delims.find(c) != String::npos ) {
			m_position++;		
		} else {
			return;
		}
		
	}
}
	
boolean CTokenizer::hasMoreTokens() 
{
	eatDelimeters();
	return (m_position < m_length);
}

}	
}
