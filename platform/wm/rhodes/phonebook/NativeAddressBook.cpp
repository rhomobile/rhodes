#include "stdafx.h"

#include "ext/phonebook/phonebook.h" 
#include "NativeAddressBook.h"
#include <common/RhoStd.h>

#define INITGUID
#include <pimstore.h>
#include <comutil.h>

using namespace rho;
using namespace rho::common;

IMPLEMENT_LOGCLASS(CABRecord,"ABRecord");
IMPLEMENT_LOGCLASS(CABOutlookRecord,"ABOutlookRecord");
IMPLEMENT_LOGCLASS(CABSimRecord,"ABSimRecord");
IMPLEMENT_LOGCLASS(CNativeAddressBook,"NativeAddressBook");

// Note that return type is std::string, not LPCSTR!!!
// It is required to get live object even after function exit
// In case if it is LPCSTR we actually get pointer to the
// dead object internal member
std::string bstr2str(BSTR s) {
	_bstr_t bstr(s);
	return (LPCSTR)bstr;
}

std::string& CABRecord::getValue(std::string field) {
	return m_props[field];
}

int CABRecord::setValue(std::string field,std::string value) {
	m_props[field] = value;
	return 1;
}

void CABRecord::enumValues(ab_callback_t proc,void* param) {
	std::map<std::string,std::string>::iterator it;
	for ( it = m_props.begin(); it != m_props.end(); it++ ) {
		(*proc)((*it).first.c_str(),(*it).second.c_str(),param);
	}
}

void CABRecord::dump() {
	LOG(INFO) + "Record dump ================================================";
	std::map<std::string,std::string>::iterator it;
	for ( it = m_props.begin(); it != m_props.end(); it++ ) {
		LOG(INFO) + "[" + (*it).first.c_str() + "] -> [" + (*it).second.c_str() + "]";
	}
	LOG(INFO) + "End of record dump =========================================";
}

//=============================================================================

CABOutlookRecord::CABOutlookRecord(IContact* pContact) :
m_pContact(pContact) {  
	m_type = eOutlookContact; 
	m_modifyed = false; 
}

CABOutlookRecord::~CABOutlookRecord() {
	if (m_pContact) {
		m_pContact->Release();
	}
}

int CABOutlookRecord::load() {
	if (m_pContact) {
		long oid;
		if(SUCCEEDED(m_pContact->get_Oid(&oid) ) ) {
			char buf[128];
			sprintf(buf,"{outlook %ld}",oid);
			setValue(RUBY_PB_ID,buf);
		}

		BSTR pwsz = NULL;
		if(SUCCEEDED(m_pContact->get_MobileTelephoneNumber(&pwsz) ) ) {
			if(pwsz[0]!=0) {
				std::string const &number = bstr2str(pwsz);
				setValue(RUBY_PB_MOBILE_NUMBER,number);
			}
			::SysFreeString( pwsz);
		}
		
		if(SUCCEEDED(m_pContact->get_HomeTelephoneNumber(&pwsz) ) ) {
			if(pwsz[0]!=0) {
				std::string const &number = bstr2str(pwsz);
				setValue(RUBY_PB_HOME_NUMBER,number);
			}
			::SysFreeString( pwsz);
		}	
		
		if(SUCCEEDED(m_pContact->get_BusinessTelephoneNumber(&pwsz) ) ) {
			if(pwsz[0]!=0) {
				std::string const &number = bstr2str(pwsz);
				setValue(RUBY_PB_BUSINESS_NUMBER,number);
			}
			::SysFreeString( pwsz);
		}

		if(SUCCEEDED(m_pContact->get_FirstName(&pwsz) ) ) {
			if(pwsz[0]!=0) {
				std::string const &name = bstr2str(pwsz);
				setValue(RUBY_PB_FIRST_NAME,name);
			}
			::SysFreeString(pwsz);
		}

		if(SUCCEEDED(m_pContact->get_LastName(&pwsz) ) ) {
			if(pwsz[0]!=0) {
				std::string const &name = bstr2str(pwsz);
				setValue(RUBY_PB_LAST_NAME,name);
			}
			::SysFreeString(pwsz);
		}

		if(SUCCEEDED(m_pContact->get_CompanyName(&pwsz) ) ) {
			if(pwsz[0]!=0) {
				std::string const &name = bstr2str(pwsz);
				setValue(RUBY_PB_COMPANY_NAME,name);
			}
			::SysFreeString(pwsz);
		}
		
		if(SUCCEEDED(m_pContact->get_Email1Address(&pwsz) ) ) {
			if(pwsz[0]!=0) {
				std::string const &address = bstr2str(pwsz);
				setValue(RUBY_PB_EMAIL_ADDRESS,address);
			}
			::SysFreeString(pwsz);
		}

	}
	return 1;
}

void CABOutlookRecord::saveValues() {
	const char* value = getValue(RUBY_PB_MOBILE_NUMBER).c_str();
	if (value!=NULL) {
		m_pContact->put_MobileTelephoneNumber(_bstr_t(value));
	}
	value = getValue(RUBY_PB_HOME_NUMBER).c_str();
	if (value!=NULL) {
		m_pContact->put_HomeTelephoneNumber(_bstr_t(value));
	}
	value = getValue(RUBY_PB_BUSINESS_NUMBER).c_str();
	if (value!=NULL) {
		m_pContact->put_BusinessTelephoneNumber(_bstr_t(value));
	}

	value = getValue(RUBY_PB_FIRST_NAME).c_str();
	if (value!=NULL) {
		m_pContact->put_FirstName(_bstr_t(value));
	}
	value = getValue(RUBY_PB_LAST_NAME).c_str();
	if (value!=NULL) {
		m_pContact->put_LastName(_bstr_t(value));
	}
	value = getValue(RUBY_PB_COMPANY_NAME).c_str();
	if (value!=NULL) {
		m_pContact->put_CompanyName(_bstr_t(value));
	}
	value = getValue(RUBY_PB_EMAIL_ADDRESS).c_str();
	if (value!=NULL) {
		m_pContact->put_Email1Address(_bstr_t(value));
	}
}

int CABOutlookRecord::save() {
	if (m_pContact) {
		saveValues();
		m_pContact->Save();
	}
	return 1;
}

int CABOutlookRecord::remove() {
	if (m_pContact) {
		m_pContact->Delete();
	}
	return 1;
}

//=============================================================================

CNativeAddressBook::CNativeAddressBook() :
m_openedOutlookAB(false), m_outlookApp(NULL), m_outlookItems(NULL), m_hSim(0) {
}

CNativeAddressBook::~CNativeAddressBook() {
	if (m_openedOutlookAB)
		closeOutlookAB();

	if (m_hSim)
		closeSimAB();
}

int CNativeAddressBook::openAB() {
	initSimAB();
	return initOutlookAB();
}

int CNativeAddressBook::getAllRecords(std::vector<CABRecord*>& records) {
	getAllSimPhonebookRecords(records);

	return getAllOutlookRecords(records);
}

CABRecord* CNativeAddressBook::getRecord(char* id) 
{
	char recordType[20];
	long recordId;
	if ( sscanf(id, "{%s %ld}", recordType, &recordId) == 2 ) {
		LOG(INFO) + "get Record " + recordId + " of " + recordType +" type";
		if (strcmp(recordType,"outlook")==0) {
			return getOutlookRecord(recordId);
		} else if (strcmp(recordType,"sim")==0) {
			return getSimRecord(recordId);
		}
	}
	return NULL;
}
	
int CNativeAddressBook::addRecord(CABRecord* record) {
	return addOutlookRecord((CABOutlookRecord*)record);
}

int CNativeAddressBook::deleteRecord(CABRecord* record) {
	return 1;
}

//=============================================================================

int CNativeAddressBook::initOutlookAB() {
	m_openedOutlookAB = false;

    HRESULT hr = CoInitializeEx( NULL, 0);
	// subsequent valid calls return S_FALSE
    /*if (hr != S_OK) {
        // CoInitializeEx failed.
        LOG(ERROR) + "CoInitializeEx failed.";
        return false;  // Replace with specific error handling.;
    }*/

    IUnknown* pUnknown = NULL;
    hr = CoCreateInstance(__uuidof(Application),//CLSID_Application, 
        NULL, CLSCTX_INPROC_SERVER, 
        IID_IUnknown, 
        (void **)&pUnknown);
    if (hr != S_OK) {
        // CoCreateInstance failed.
        LOG(ERROR) + "CoCreateInstance failed.";
        CoUninitialize();
        return false;  // Replace with specific error handling.;
    }

    hr = pUnknown->QueryInterface(__uuidof(IPOutlookApp)/*IID_IPOutlookApp*/, (void**)&m_outlookApp); 
    if (hr != S_OK) {
        // QueryInterface failed.
        LOG(ERROR) + "QueryInterface failed.";
        CoUninitialize();
        return false;  // Replace with specific error handling.;
    }

    hr = m_outlookApp->Logon(NULL);
    if (hr != S_OK) {
        // Logon failed.
        LOG(ERROR) + "Logon failed.";
        m_outlookApp->Release();
        CoUninitialize();
        return false;  // Replace with specific error handling.;
    }

    m_openedOutlookAB = true;
    return m_openedOutlookAB;
}

int CNativeAddressBook::closeOutlookAB() {
	if(m_outlookItems) {
		m_outlookItems->Release();
	}
	if(m_outlookApp) {
		m_outlookApp->Release();
		CoUninitialize();
	}
	return 1;
}

bool CNativeAddressBook::getOutlookItems() {
	if( (m_outlookApp!=NULL)&&(m_outlookItems==NULL)) {
		//
		IFolder* polFolder;
		HRESULT res = m_outlookApp->GetDefaultFolder(olFolderContacts, &polFolder);
		if (res != S_OK) {
			// QueryInterface failed.
			LOG(ERROR) + "GetDefaultFolder failed.";
			return false;
		}

		res = polFolder->get_Items(&m_outlookItems);
		if (res != S_OK) {
			// QueryInterface failed.
			polFolder->Release();
			LOG(ERROR) + "get_Items failed.";
			return false;
		}

		polFolder->Release();
	}
	return true;
}

int CNativeAddressBook::getAllOutlookRecords(std::vector<CABRecord*>& records) {
	if(m_outlookApp && getOutlookItems()) { 
		int size; 
		m_outlookItems->get_Count(&size);

		IContact * pContact;
		CABOutlookRecord *pRecord;
		for(int index = 1; index <= size; ++index) {
			m_outlookItems->Item(index, (IDispatch**)&pContact);
			pRecord = new CABOutlookRecord(pContact);
			if(pRecord) {
				pRecord->load();
				records.push_back(pRecord);
			} else {
				pContact->Release();
			}
		}
	}
	return 1;
}

CABRecord* CNativeAddressBook::getOutlookRecord(long oid) {
	if(m_outlookApp && getOutlookItems()) { 
		DWORD index;
		if(S_OK==GetItemIndexFromOid(m_outlookItems,oid,&index)) {
			IContact * pContact;
			CABOutlookRecord *pRecord;
			if (SUCCEEDED(m_outlookItems->Item(1+index, (IDispatch**)&pContact))) {
				pRecord = new CABOutlookRecord(pContact);
				if(pRecord) {
					pRecord->load();
					return pRecord;
				} else {
					pContact->Release();
				}
			}
		}
	}
	return NULL;
}

int CNativeAddressBook::addOutlookRecord(CABOutlookRecord* record) {
	if(m_outlookApp && getOutlookItems()) { 
		if(SUCCEEDED(m_outlookItems->Add((IDispatch**)&record->m_pContact))) {
			record->save();
		}
	}
	return 1;
}

bool CNativeAddressBook::initSimAB()
{
	HRESULT h = SimInitialize(0, NULL, 0, &m_hSim);
	if (SUCCEEDED(h))
	{
		if (SUCCEEDED(SimGetDevCaps(m_hSim, SIM_CAPSTYPE_ALL, &m_SimCaps))) 
		{
				return true;				
		}
		LOG(ERROR) + "Failed to get SIM capabilities.";
		SimDeinitialize(m_hSim);
		m_hSim = NULL;
	}
	LOG(ERROR) + "Failed to init SIM. Error=" + (int)h;

	return false;
}

bool CNativeAddressBook::closeSimAB()
{
	SimDeinitialize(m_hSim);
	return false;
}

CABRecord* CNativeAddressBook::getSimRecord(int id)
{
	CABSimRecord *pRecord;
	HRESULT hr = 0;

	pRecord = new CABSimRecord(id, m_hSim);

	if(pRecord) {
		if (SUCCEEDED(hr = pRecord->load())) {
			return pRecord;
		} else {
			if (SIM_E_NOTFOUND)
				LOG(ERROR) + "Failed to get SIM entry";
		}
	} else {
		LOG(ERROR) + "No more memory";
	}
	return NULL;
}

int CNativeAddressBook::getAllSimPhonebookRecords(std::vector<CABRecord*>& records)
{
	CABSimRecord *pRecord;
	HRESULT hr = 0;

	for(DWORD i = m_SimCaps.dwMinPBIndex; i <= m_SimCaps.dwMaxPBIndex; i++) {
		pRecord = new CABSimRecord(i, m_hSim);
		if(pRecord) {
			if (SUCCEEDED(hr = pRecord->load()))
				records.push_back(pRecord);
			else {
				if (SIM_E_NOTFOUND)
					break;
				LOG(ERROR) + "Failed to get SIM entry";
			}
		} else {
			LOG(ERROR) + "No more memory";
		}
	}

	
	return SUCCEEDED(hr) ? 1 : 0;
}

CABSimRecord::CABSimRecord(int index, HSIM hSim)
{
	m_hSim = hSim;
	m_index = index;
}

CABSimRecord::~CABSimRecord()
{
}

int CABSimRecord::load()
{
	SIMPHONEBOOKENTRY entry;
	
	String name, address;
	HRESULT hr = SimReadPhonebookEntry(m_hSim, SIM_PBSTORAGE_SIM, m_index, &entry);
	if (SUCCEEDED(hr))
	{
		char buf[128];
		sprintf(buf,"{sim %ld}", m_index);
		setValue(RUBY_PB_ID,buf);

		setValue(RUBY_PB_FIRST_NAME, convertToStringA(entry.lpszText));
		setValue(RUBY_PB_MAIN_MUMBER, convertToStringA(entry.lpszAddress));
	}

	return hr;
}

int CABSimRecord::save()
{
	return 0;
}

int CABSimRecord::remove()
{
	return 0;
}

void CABSimRecord::saveValues()
{
}