//
//  NetworkStatusMonitor.h
//  rhorunner
//
//  Created by Alex Epifanoff on 03.09.12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#ifdef __cplusplus
extern "C" {
#endif //__cplusplus
	void initNetworkMonitoring();
    int getNetworkStatusPollInterval();
	void networkStatusNotify(int status);
#ifdef __cplusplus
}
#endif
