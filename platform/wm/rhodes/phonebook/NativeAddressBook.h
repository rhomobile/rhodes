#pragma once

#include <string>
#include <map>
#include <vector>

#include <pimstore.h>

typedef void (*ab_callback_t)(const char*, const char*, void*);

class CABRecord {
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
	bool	m_modifyed;

	std::map<std::string,std::string> m_props;
};

class CABOutlookRecord : public CABRecord {
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

class CNativeAddressBook
{
public:
	CNativeAddressBook();
	virtual ~CNativeAddressBook();

	int openAB();

	int getAllRecords(std::vector<CABRecord*>& records);		
	CABRecord* getRecord(char* id);
	
	int addRecord(CABRecord* record);
	int deleteRecord(CABRecord* record);

protected:
	bool m_openedOutlookAB;
	IPOutlookApp* m_outlookApp;
	IPOutlookItemCollection* m_outlookItems;

	int initOutlookAB();
	int closeOutlookAB();
	bool getOutlookItems();
	int getAllOutlookRecords(std::vector<CABRecord*>& records);
	CABRecord* getOutlookRecord(long oid);
	int addOutlookRecord(CABOutlookRecord* record);

	/*int initSimAB();
	int closeSimAB();*/
};
