/*
 ============================================================================
 Author	    : Anton Antonov
 Version	: 1.0
 Copyright  : Copyright (C) 2008 Rhomobile. All rights reserved.

 This program is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with this program.  If not, see <http://www.gnu.org/licenses/>.
 ============================================================================
 */

#ifndef HTTPSERVER_H
#define HTTPSERVER_H

// INCLUDES
#include <e32std.h>

//FORWARD DECLARATION
struct shttpd_ctx;

// CLASS DECLARATION

/**
 *  CHttpServer
 * 
 */
class CHttpServer	
	{
public:
	// Constructors and destructor

	/**
	 * Destructor.
	 */
	~CHttpServer();

	/**
	 * Two-phased constructor.
	 */
	static CHttpServer* NewL();

	/**
	 * Two-phased constructor.
	 */
	static CHttpServer* NewLC();

	/**
	 * Resume thread
	 */
	void ResumeThread();
	
	/**
	 * Suspend thread
	 */
	void SuspendThread();
	
	/**
	 * Stop thread
	 */
	void StopThread();
	
	void StopRubyFramework() { iStopRubyFramework = true; }
	void StartRubyFramework() { iStartRubyFramework = true; }
	/**
	 * Main thread function
	 */
	TInt Execute(); 

	struct shttpd_ctx* GetHttpdContext();
private:

	/**
	 * Constructor for performing 1st stage construction
	 */
	CHttpServer();

	/**
	 * EPOC default constructor for performing 2nd stage construction
	 */
	void ConstructL();
	
protected:
	
	/**
	 * Initialize http server
	 */
	void InitHttpServer();
	
	/**
	 * Initialize ruby framework
	 */
	void InitRubyFramework();
	
	TInt ExecuteL();
	
	void SendWindowEvent(TInt aEvent);
	
private: //data
	
	RThread thread;
	
	struct shttpd_ctx* ctx;
	bool iClose;
	
	bool iStopRubyFramework;
	bool iStartRubyFramework;
	};

#endif // HTTPSERVER_H
