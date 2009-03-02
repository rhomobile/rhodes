extern void wake_up_sync_engine();

void dosync() {
	wake_up_sync_engine();
}

const char* RhoGetRootPath() {
	return "\\Data\\Rho\\";
}


