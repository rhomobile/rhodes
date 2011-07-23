package com.rho.sync;

import java.util.Vector;

import com.rho.RhoClassFactory;
import com.rho.sync.SyncNotify.SyncNotification;

public class SyncNotify {

	public class SyncNotification {
	}

	public static Object enUpdate;
	public static Object enCreate;
	public static Object enDelete;

	public SyncNotify(SyncEngine syncEngine) {
		// TODO Auto-generated constructor stub
	}

	public void setFactory(RhoClassFactory factory) {
		// TODO Auto-generated method stub
		
	}

	public void fireBulkSyncNotification(boolean b, String string,
			String strPartition, int errNone) {
		// TODO Auto-generated method stub
		
	}

	public void callLoginCallback(SyncNotification oNotify, int nErrCode,
			String charData) {
		// TODO Auto-generated method stub
		
	}

	public void callLoginCallback(SyncNotification oNotify, String errRuntime,
			String charData) {
		// TODO Auto-generated method stub
		
	}

	public void cleanCreateObjectErrors() {
		// TODO Auto-generated method stub
		
	}

	public void cleanLastSyncObjectCount() {
		// TODO Auto-generated method stub
		
	}

	public void onObjectChanged(Integer id, String strObject, Object enUpdate2) {
		// TODO Auto-generated method stub
		
	}

	public void fireObjectsNotification() {
		// TODO Auto-generated method stub
		
	}

	public void fireSyncNotification(SyncSource syncSource, boolean b,
			int errNone, String string) {
		// TODO Auto-generated method stub
		
	}

	public void fireAllSyncNotifications(boolean b, int m_nErrCode,
			String m_strError) {
		// TODO Auto-generated method stub
		
	}

	public void fireAllSyncNotifications(boolean b, String errRuntime,
			String m_strError) {
		// TODO Auto-generated method stub
		
	}

	public int incLastSyncObjectCount(Integer id) {
		// TODO Auto-generated method stub
		return 0;
	}

	public void onSyncSourceEnd(int i, Vector m_sources) {
		// TODO Auto-generated method stub
		
	}

	public void reportSyncStatus(String string, int m_nErrCode,
			String m_strError) {
		// TODO Auto-generated method stub
		
	}

}
