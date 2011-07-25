/*------------------------------------------------------------------------
* (The MIT License)
* 
* Copyright (c) 2008-2011 Rhomobile, Inc.
* 
* Permission is hereby granted, free of charge, to any person obtaining a copy
* of this software and associated documentation files (the "Software"), to deal
* in the Software without restriction, including without limitation the rights
* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
* copies of the Software, and to permit persons to whom the Software is
* furnished to do so, subject to the following conditions:
* 
* The above copyright notice and this permission notice shall be included in
* all copies or substantial portions of the Software.
* 
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
* THE SOFTWARE.
* 
* http://rhomobile.com
*------------------------------------------------------------------------*/

#pragma once

#include <string>
#include <map>
#include <vector>

#include <pimstore.h>
#include <simmgr.h>
#include "logging/RhoLog.h"

typedef void (*ab_callback_t)(const char*, const char*, void*);

class CABRecord {
    DEFINE_LOGCLASS;
public:
	CABRecord() {}
	virtual ~CABRecord() {}

	virtual std::string& getValue(std::string field);
	virtual int setValue(std::string field,std::string value);
	virtual void enumValues(ab_callback_t proc,void* param);

	virtual int load()   = 0;
	virtual int save()   = 0;
	virtual int remove() = 0;

	enum ContactType {
		eOutlookContact,
		eSimContact
	};
	ContactType getType() { return m_type; }

	void dump();

protected:
	ContactType m_type;
	bool		m_modifyed;

	std::map<std::string,std::string> m_props;
};


class CABOutlookRecord : public CABRecord {
    DEFINE_LOGCLASS;
public:
	CABOutlookRecord(IContact* pContact);
	virtual ~CABOutlookRecord();

	virtual int load();
	virtual int save();
	virtual int remove();

protected:
	void saveValues();

private:
	IContact* m_pContact;

	friend class CNativeAddressBook;
};

class CABSimRecord : public CABRecord {
	DEFINE_LOGCLASS;
public:
	CABSimRecord(int index, HSIM hSim);
	virtual ~CABSimRecord();

	virtual int load();
	virtual int save();
	virtual int remove();

protected:
	void saveValues();

private:
	int m_index;
	HSIM m_hSim;

	friend class CNativeAddressBook;
};

class CNativeAddressBook
{
    DEFINE_LOGCLASS;
public:
	CNativeAddressBook();
	virtual ~CNativeAddressBook();

	int openAB();

	int getAllRecords(std::vector<CABRecord*>& records);		
	CABRecord* getRecord(char* id);
	
	int addRecord(CABRecord* record);
	int deleteRecord(CABRecord* record);

protected:
	//outlook
	IPOutlookItemCollection* m_outlookItems;

	bool getOutlookItems();
	int getAllOutlookRecords(std::vector<CABRecord*>& records);
	CABRecord* getOutlookRecord(long oid);
	int addOutlookRecord(CABOutlookRecord* record);


	//sim
	HSIM	m_hSim;
	SIMCAPS	m_SimCaps;

	bool initSimAB();
	bool closeSimAB();
	int getAllSimPhonebookRecords(std::vector<CABRecord*>& records);
	CABRecord* getSimRecord(int id);
};
