package com.rhomobile.rhosync;

public interface RhomModel {
	RhoSyncNotify sync();
	/* TODO:
	create();
	find(String text);
	findFirst();
	findAll();
	save();
	destroy();
	*/
	void startBulkUpdate();
	void stopBulkUpdate();

}
