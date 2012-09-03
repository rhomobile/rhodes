//
//  NetworkStatusMonitor.m
//  rhorunner
//
//  Created by Alex Epifanoff on 03.09.12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//


#import "NetworkStatusMonitor.h"
#import "Reachability.h"

#include "common/RhodesApp.h"
#include "common/RhoStd.h"
#include "common/AutoPointer.h"

namespace rho
{
	namespace common
	{
		class IPhoneNetworkStatusMonitor : public INetworkStatusMonitor
		{
		private:
			int m_pollInterval;
			INetworkStatusReceiver* m_receiver;
		public:
			IPhoneNetworkStatusMonitor() : m_pollInterval(20), m_receiver(0) {}
			virtual void setPollInterval(int pollInterval) { m_pollInterval = pollInterval; }
			virtual void setNetworkStatusReceiver(INetworkStatusReceiver* receiver) { m_receiver = receiver; }
			int getPollInterval() const { return m_pollInterval; }
			void callNotify(rho::common::enNetworkStatus status) {
				if ( m_receiver != 0 ) {
					m_receiver->onNetworkStatusChanged(status);
				}
			}
		};
	}
}

static rho::common::CAutoPtr<rho::common::IPhoneNetworkStatusMonitor> s_network_status_monitor(new rho::common::IPhoneNetworkStatusMonitor());

extern "C"
{
	void initNetworkMonitoring()
	{
		RHODESAPP().setNetworkStatusMonitor(s_network_status_monitor);
	}
	
    int getNetworkStatusPollInterval()
	{
		return s_network_status_monitor->getPollInterval();
	}
	
	void networkStatusNotify(int status)
	{
		s_network_status_monitor->callNotify(status==0?(rho::common::networkStatusDisconnected):(rho::common::networkStatusConnected));
	}	
}
