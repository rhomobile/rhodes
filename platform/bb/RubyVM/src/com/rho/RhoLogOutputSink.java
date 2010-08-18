package com.rho;

public class RhoLogOutputSink implements IRhoLogSink {

	private static RhoLogConf m_oLogConf;

	RhoLogOutputSink( RhoLogConf conf ){
		m_oLogConf = conf;
	}
	
	public void close(){
	}
	
	private RhoLogConf getLogConf(){
		return m_oLogConf;
	}
	
	public int getCurPos() {
		// TODO Auto-generated method stub
		return 0;
	}

	public void writeLogMessage(String strMsg) {
		System.out.print(strMsg);
		System.out.flush();
	}
	
	public void clear(){
		
	}

}
