/*
 ============================================================================
 Name		: SyncEngine.h
 Author	  : Anton Antonov
 Version	 : 1.0
 Copyright   :   Copyright (C) 2008 Rhomobile. All rights reserved.

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
 Description : CSyncEngineWrap declaration
 ============================================================================
 */

#ifndef SYNCENGINEWRAP_H
#define SYNCENGINEWRAP_H

// INCLUDES
#include <e32std.h>
#include <e32base.h>

struct sqlite3;

// CLASS DECLARATION

/**
 *  CSyncEngineWrap
 * 
 */
class CSyncEngineWrap : public CBase
	{
public:
	// Constructors and destructor

	/**
	 * Destructor.
	 */
	~CSyncEngineWrap();

	/**
	 * Two-phased constructor.
	 */
	static CSyncEngineWrap* NewL();

	/**
	 * Two-phased constructor.
	 */
	static CSyncEngineWrap* NewLC();

	/**
	 * Resume thread
	 */
	void ResumeThread();
	
	/**
	 * Suspend thread
	 */
	void SuspendThread();
	
	/**
	 * Terminate thread
	 */
	void TerminateThread();

	/**
	 * Main thread function
	 */
	TInt Execute();
private:

	/**
	 * Constructor for performing 1st stage construction
	 */
	CSyncEngineWrap();

	/**
	 * EPOC default constructor for performing 2nd stage construction
	 */
	void ConstructL();
	
	/**
	 * Initialize sync engine
	 */
	void StartSyncEngine();

	void StopSyncEngine();

protected:
	TInt ExecuteL();
	void SendWindowEvent(TInt aEvent);
	
private://data
		
		RThread thread;
		bool iClose;
	};

#endif // SYNCENGINEWRAP_H
