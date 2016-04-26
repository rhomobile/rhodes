#include "../../../shared/generated/cpp/smartcradleBase.h"
#include "SmartCradle.h"
#include <algorithm>
#include <sstream>

namespace rho {

using namespace apiGenerator;
using namespace common;

CCradle* m_pSmartCradle;

class CSmartCradleImpl: public CSmartCradleBase
{
public:
    CSmartCradleImpl(const rho::String& strID): CSmartCradleBase()
    {
    }

    virtual void getPlatformName(rho::apiGenerator::CMethodResult& oResult) {
         oResult.set("WM");
    }

};

class CSmartCradleSingleton: public CSmartCradleSingletonBase
{
    ~CSmartCradleSingleton(){}

    void getRowId(rho::apiGenerator::CMethodResult& oResult) 
	{
		if (m_pSmartCradle && m_pSmartCradle->m_hCradle) 
		{LOG(WARNING) + L"Smart Cradle API is not available on this device"; return;}
		DWORD row;
		if (m_pSmartCradle->GetCradleMatrixRow(&row))
			oResult.set(row);
		else
			LOG(WARNING) + L"Unable to retrieve Smart Cradle Row ID";
	}
    
	void setRowId( int rowId, rho::apiGenerator::CMethodResult& oResult) 
	{
		if (m_pSmartCradle && m_pSmartCradle->m_hCradle) 
		{LOG(WARNING) + L"Smart Cradle API is not available on this device"; return;}

		if (rowId >= 0)
			m_pSmartCradle->SetCradleMatrixRow((WORD)rowId);
		else
			LOG(WARNING) + L"Specified row ID is out of range.  Must be positive";
		oResult.set(true);
	}
    
	void getColumnId(rho::apiGenerator::CMethodResult& oResult)
	{
		if (m_pSmartCradle && m_pSmartCradle->m_hCradle) 
		{LOG(WARNING) + L"Smart Cradle API is not available on this device"; return;}
		DWORD column;
		if (m_pSmartCradle->GetCradleMatrixColumn(&column))
			oResult.set(column);
		else
			LOG(WARNING) + L"Unable to retrieve Smart Cradle Column ID";	
	}
    
	void setColumnId( int columnId, rho::apiGenerator::CMethodResult& oResult) 
	{
		if (m_pSmartCradle && m_pSmartCradle->m_hCradle) 
		{LOG(WARNING) + L"Smart Cradle API is not available on this device"; return;}

		if (columnId >= 0)
			m_pSmartCradle->SetCradleMatrixColumn((WORD)columnId);
		else
			LOG(WARNING) + L"Specified column ID is out of range.  Must be positive";	
		oResult.set(true);
	}
    void getWallId(rho::apiGenerator::CMethodResult& oResult) 
	{
		if (m_pSmartCradle && m_pSmartCradle->m_hCradle) 
		{LOG(WARNING) + L"Smart Cradle API is not available on this device"; return;}
		DWORD wall;
		if (m_pSmartCradle->GetCradleMatrixWall(&wall))
			oResult.set(wall);
		else
			LOG(WARNING) + L"Unable to retrieve Smart Cradle Wall ID";	
	}
    
	void setWallId( int wallId, rho::apiGenerator::CMethodResult& oResult) 
	{
		if (m_pSmartCradle && m_pSmartCradle->m_hCradle) 
		{LOG(WARNING) + L"Smart Cradle API is not available on this device"; return;}

		if (wallId >= 0)
			m_pSmartCradle->SetCradleMatrixWall((WORD)wallId);
		else
			LOG(WARNING) + L"Specified Wall ID is out of range.  Must be positive";	
	
		oResult.set(true);
	}

    void getChargeState(rho::apiGenerator::CMethodResult& oResult) 
	{
		if (m_pSmartCradle && m_pSmartCradle->m_hCradle) 
		{LOG(WARNING) + L"Smart Cradle API is not available on this device"; return;}		
		
		m_pSmartCradle->GetCradleFastChargeState(oResult);
	}

    void setChargeState( const rho::String& chargeState, rho::apiGenerator::CMethodResult& oResult) 
	{
		if (m_pSmartCradle && m_pSmartCradle->m_hCradle) 
		{LOG(WARNING) + L"Smart Cradle API is not available on this device"; return;}

		rho::String comparitor;
		comparitor.assign(chargeState);
			std::transform(comparitor.begin(), comparitor.end(), comparitor.begin(), ::tolower);
			if (comparitor.compare("fast") == 0)
				m_pSmartCradle->SetCradleFastChargeState(true);
			else if (comparitor.compare("slow") == 0)
				m_pSmartCradle->SetCradleFastChargeState(false);
			else
				LOG(WARNING) + L"Specified Charge State (" + chargeState + ") is unrecognised.  Expected 'Fast' or 'Slow'";

		oResult.set(true);
	}

	void getHardwareId(rho::apiGenerator::CMethodResult& oResult) 
	{
		if (m_pSmartCradle && m_pSmartCradle->m_hCradle) 
		{LOG(WARNING) + L"Smart Cradle API is not available on this device"; return;}

		m_pSmartCradle->GetCradleInfoProperty(CRADLE_INFO_HARDWARE, oResult);
	}

	void getPartNumber(rho::apiGenerator::CMethodResult& oResult) 
	{
		if (m_pSmartCradle && m_pSmartCradle->m_hCradle) 
		{LOG(WARNING) + L"Smart Cradle API is not available on this device"; return;}

		m_pSmartCradle->GetCradleInfoProperty(CRADLE_INFO_PART, oResult);
	}

    void getSerialNumber(rho::apiGenerator::CMethodResult& oResult)
	{
		if (m_pSmartCradle && m_pSmartCradle->m_hCradle) 
		{LOG(WARNING) + L"Smart Cradle API is not available on this device"; return;}

		m_pSmartCradle->GetCradleInfoProperty(CRADLE_INFO_SERIAL, oResult);
	}

	void getMfgDate(rho::apiGenerator::CMethodResult& oResult) 
	{
		if (m_pSmartCradle && m_pSmartCradle->m_hCradle) 
		{LOG(WARNING) + L"Smart Cradle API is not available on this device"; return;}

		m_pSmartCradle->GetCradleInfoProperty(CRADLE_INFO_MFG, oResult);
	}

    void getDriverVersion(rho::apiGenerator::CMethodResult& oResult) 
	{
		if (m_pSmartCradle && m_pSmartCradle->m_hCradle) 
		{LOG(WARNING) + L"Smart Cradle API is not available on this device"; return;}

		m_pSmartCradle->GetCradleVersionProperty(CRADLE_VERSION_DRIVER, oResult);
	}

    void getCApiVersion(rho::apiGenerator::CMethodResult& oResult) 
	{
		if (m_pSmartCradle && m_pSmartCradle->m_hCradle) 
		{LOG(WARNING) + L"Smart Cradle API is not available on this device"; return;}

		m_pSmartCradle->GetCradleVersionProperty(CRADLE_VERSION_CAPI, oResult);
	}

    void getFirmwareVersion(rho::apiGenerator::CMethodResult& oResult) 
	{
		if (m_pSmartCradle && m_pSmartCradle->m_hCradle) 
		{LOG(WARNING) + L"Smart Cradle API is not available on this device"; return;}

		m_pSmartCradle->GetCradleVersionProperty(CRADLE_VERSION_FIRMWARE, oResult);	
	}

	void setHardwareId( const rho::String& hardwareId, rho::apiGenerator::CMethodResult& oResult) 
	{LOG(ERROR) + L"Attempted to Set a Read Only parameter";oResult.set(true);return;}
    void setPartNumber( const rho::String& partNumber, rho::apiGenerator::CMethodResult& oResult)
	{LOG(ERROR) + L"Attempted to Set a Read Only parameter";oResult.set(true);return;}
    void setSerialNumber( const rho::String& serialNumber, rho::apiGenerator::CMethodResult& oResult)
	{LOG(ERROR) + L"Attempted to Set a Read Only parameter";oResult.set(true);return;}
    void setMfgDate( const rho::String& mfgDate, rho::apiGenerator::CMethodResult& oResult)
	{LOG(ERROR) + L"Attempted to Set a Read Only parameter";oResult.set(true);return;}
    void setDriverVersion( const rho::String& driverVersion, rho::apiGenerator::CMethodResult& oResult)
	{LOG(ERROR) + L"Attempted to Set a Read Only parameter";oResult.set(true);return;}
    void setFirmwareVersion( const rho::String& firmwareVersion, rho::apiGenerator::CMethodResult& oResult)
	{LOG(ERROR) + L"Attempted to Set a Read Only parameter";oResult.set(true);return;}
    void setCApiVersion( const rho::String& cApiVersion, rho::apiGenerator::CMethodResult& oResult)
	{LOG(ERROR) + L"Attempted to Set a Read Only parameter";oResult.set(true);return;}

	void unlock( int ledOnDuration,  int ledOffDuration,  int timeout, rho::apiGenerator::CMethodResult& oResult) 
	{
		if (m_pSmartCradle && m_pSmartCradle->m_hCradle) 
		{LOG(WARNING) + L"Smart Cradle API is not available on this device"; return;}

		m_pSmartCradle->SetCradleUnlockOnDuration(ledOnDuration);
		m_pSmartCradle->SetCradleUnlockOffDuration(ledOffDuration);
		m_pSmartCradle->SetCradleUnlockTimeOut(timeout);
		m_pSmartCradle->UnlockCradle();
		oResult.set(true);
	}
        // unlockEx Unlocks the terminal from the cradle for the specified period. For Android, the smoothing Effect by dafault is false. 
	void unlockEx( const rho::Hashtable<rho::String, rho::String>& propertyMap, rho::apiGenerator::CMethodResult& oResult) 
	{
		LOG(INFO) + L"The unlockEx method is not supported on WM/CE platform.";
	} 
	// flashLed The LED can be flashed to identify device location without unlocking the cradle. 
	void flashLed( const rho::Hashtable<rho::String, rho::String>& propertyMap, rho::apiGenerator::CMethodResult& oResult) 
	{
		LOG(INFO) + L"The flashLed method is not supported on WM/CE platform.";		
	} 
	void setProperty( const rho::String& propertyName,  const rho::String& propertyValue, rho::apiGenerator::CMethodResult& oResult)
	{
		rho::String comparitor;
		comparitor.assign(propertyName);
		std::transform(comparitor.begin(), comparitor.end(), comparitor.begin(), ::tolower);
		if (comparitor.compare("chargestate") == 0)
			setChargeState(propertyValue, oResult);
		else if (comparitor.compare("rowid") == 0)
			setRowId(atoi(propertyValue.c_str()), oResult);
		else if (comparitor.compare("columnid") == 0)
			setColumnId(atoi(propertyValue.c_str()), oResult);
		else if (comparitor.compare("wallid") == 0)
			setWallId(atoi(propertyValue.c_str()), oResult);
		else
			LOG(WARNING) + L"Unrecognised writable property (" + propertyName + ") in call to setProperty";

		oResult.set(true);
	}

	void getProperty( const rho::String& propertyName, rho::apiGenerator::CMethodResult& oResult) 
	{
		rho::String comparitor;
		comparitor.assign(propertyName);
		std::transform(comparitor.begin(), comparitor.end(), comparitor.begin(), ::tolower);
		if (comparitor.compare("chargestate") == 0)
			getChargeState(oResult);
		else if (comparitor.compare("rowid") == 0)
			getRowId(oResult);
		else if (comparitor.compare("columnid") == 0)
			getColumnId(oResult);
		else if (comparitor.compare("wallid") == 0)
			getWallId(oResult);
		else if (comparitor.compare("hardwareid") == 0)
			getHardwareId(oResult);
		else if (comparitor.compare("partnumber") == 0)
			getPartNumber(oResult);
		else if (comparitor.compare("serialnumber") == 0)
			getSerialNumber(oResult);
		else if (comparitor.compare("mfgdate") == 0)
			getMfgDate(oResult);
		else if (comparitor.compare("driverversion") == 0)
			getDriverVersion(oResult);
		else if (comparitor.compare("capiversion") == 0)
			getCApiVersion(oResult);
		else if (comparitor.compare("firmwareversion") == 0)
			getFirmwareVersion(oResult);
		else
			LOG(WARNING) + L"Unrecognised readable property (" + propertyName + ") in call to setProperty";

	}

	void setProperties( const rho::Hashtable<rho::String, rho::String>& propertyMap, rho::apiGenerator::CMethodResult& oResult) 
	{
		//  Set multiple properties
		typedef std::map<rho::String, rho::String>::const_iterator it_type;
		for (it_type iterator = propertyMap.begin(); iterator != propertyMap.end(); iterator++)
		{
			DEBUGMSG(TRUE, (L"Setting Property %s value %s", convertToStringW(iterator->first).c_str(), convertToStringW(iterator->second).c_str()));
			setProperty(iterator->first, iterator->second, oResult);
		}
		oResult.set(true);
	}

	void getProperties( const rho::Vector<rho::String>& arrayofNames, rho::apiGenerator::CMethodResult& oResult) 
	{
		rho::Hashtable<rho::String, rho::String> propsHash;
		typedef std::vector<rho::String>::const_iterator it_type;
		for (it_type iterator = arrayofNames.begin(); iterator != arrayofNames.end(); iterator++)
		{
			rho::apiGenerator::CMethodResult* propertyValue = new rho::apiGenerator::CMethodResult();
			getProperty(*iterator, *propertyValue);
			if (propertyValue->getType() == rho::apiGenerator::CMethodResult::eInt)
			{
				std::ostringstream szValue;
				szValue << propertyValue->getInt();
				propsHash.put(*iterator, szValue.str());
			}
			else if (propertyValue->getType() == rho::apiGenerator::CMethodResult::eStringW)
				propsHash.put(*iterator, convertToStringA(propertyValue->getStringW()));
			else
				propsHash.put(*iterator, propertyValue->getString());
		}
		oResult.set(propsHash);		
	}

	void getAllProperties(rho::apiGenerator::CMethodResult& oResult) 
	{
		rho::Vector<rho::String> properties;
		properties.addElement("chargeState");
		properties.addElement("rowId");
		properties.addElement("columnId");
		properties.addElement("wallId");
		properties.addElement("hardwareId");
		properties.addElement("partNumber");
		properties.addElement("serialNumber");
		properties.addElement("mfgDate");
		properties.addElement("driverVersion");
		properties.addElement("cApiVersion");
		properties.addElement("firmwareVersion");
		getProperties(properties, oResult);
	}

};

class CSmartCradleFactory: public CSmartCradleFactoryBase
{
    ~CSmartCradleFactory(){}
    virtual ISmartCradleSingleton* createModuleSingleton();
};

extern "C" void Init_Smartcradle_extension()
{
    m_pSmartCradle = NULL;
	CSmartCradleFactory::setInstance( new CSmartCradleFactory() );
    Init_SmartCradle_API();
}


ISmartCradleSingleton* CSmartCradleFactory::createModuleSingleton()
{
	//  Create the SmartCradle object here
	m_pSmartCradle = new CCradle();
    return new CSmartCradleSingleton();
}


}
