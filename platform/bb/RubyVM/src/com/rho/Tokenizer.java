package com.rho;

public class Tokenizer {

	String m_str;
	String m_delims;
	int m_length;
	int m_position;
	
	public Tokenizer(String str,String delims) {
		m_delims = delims;
		m_str = str;
		m_length = str.length();
		m_position = 0;
	}
	
	public String nextToken() {
		eatDelimeters();
		int start = m_position;
		while (m_position<m_length && m_delims.indexOf(m_str.charAt(m_position))==-1) {
			m_position++;
		}
		
		return m_str.substring(start,m_position);
	}
	
	public void eatDelimeters() {
		while (m_position<m_length) {
			char c = m_str.charAt(m_position);
			if (m_delims.indexOf(c)>=0) {
				m_position++;		
			} else {
				return;
			}
			
		}
	}
	
    public boolean hasMoreTokens() {
    	eatDelimeters();
    	return (m_position < m_length);
    }
	

}
