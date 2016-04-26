/**
*  \file SNTP.h
*  \brief File to encapsulate the SNTP protocol.  Largely copied from PB2.x, 
*  the only modification for PB3 has been to allow logging.
*/

#include "windows.h"
#include "../../common/Public/PBPlugin.h"

/**
*  Representation of an NTP timestamp.  
*  Ported from PB2.x
*/
struct CNtpTimePacket
{
  DWORD m_dwInteger;
	DWORD m_dwFractional;
};

/**
*  Helper class to encapulate NTP time stamps.
*  Ported from PB2.x
*/
class CNtpTime
{
public:

	/**
	*  This method has been ported unmodified, direct from PB2.x
	*/
	CNtpTime();

	/**
	*  This method has been ported unmodified, direct from PB2.x
	*/
	CNtpTime(const CNtpTime& time);

	/**
	*  This method has been ported unmodified, direct from PB2.x
	*/
	CNtpTime(CNtpTimePacket& packet);

	/**
	*  This method has been ported unmodified, direct from PB2.x
	*/
	CNtpTime(const SYSTEMTIME& st);

	//General functions

	/**
	*  This method has been ported unmodified, direct from PB2.x
	*/
	CNtpTime& operator=(const CNtpTime& time);

	/**
	*  This method has been ported unmodified, direct from PB2.x
	*/
	double operator-(const CNtpTime& time) const;

	/**
	*  This method has been ported unmodified, direct from PB2.x
	*/
	CNtpTime operator+(const double& timespan) const;

	/**
	*  This method has been ported unmodified, direct from PB2.x
	*/
	operator SYSTEMTIME() const;

	/**
	*  This method has been ported unmodified, direct from PB2.x
	*/
	operator CNtpTimePacket() const;

	/**
	*  This method has been ported unmodified, direct from PB2.x
	*/
	operator unsigned __int64() const { return m_Time; };

	/**
	*  This method has been ported unmodified, direct from PB2.x
	*/
	DWORD Seconds() const;

	/**
	*  This method has been ported unmodified, direct from PB2.x
	*/
	DWORD Fraction() const;

	//Static functions
	/**
	*  This method has been ported unmodified, direct from PB2.x
	*/
	static CNtpTime GetCurrentTime();

	/**
	*  This method has been ported unmodified, direct from PB2.x
	*/
	static DWORD MsToNtpFraction(WORD wMilliSeconds);

	/**
	*  This method has been ported unmodified, direct from PB2.x
	*/
	static WORD NtpFractionToMs(DWORD dwFraction);

	/**
	*  This method has been ported unmodified, direct from PB2.x
	*/
	static double NtpFractionToSecond(DWORD dwFraction);

protected:
	//Internal static functions and data

	/**
	*  This method has been ported unmodified, direct from PB2.x
	*/
	static long GetJulianDay(WORD Year, WORD Month, WORD Day);

	/**
	*  This method has been ported unmodified, direct from PB2.x
	*/
	static void GetGregorianDate(long JD, WORD& Year, WORD& Month, WORD& Day);

	static DWORD m_MsToNTP[1000];	///< This attribute has been ported unmodified, direct from PB2.x
	///< The Actual Data.  This attribute has been ported unmodified, direct from PB2.x
	unsigned __int64 m_Time;

};

/**
*  Ported from PB2.x
*/
struct NtpServerResponse
{
	/**
	*  Ported from PB2.x
	*  0: no warning
    *  1: last minute in day has 61 seconds
    *  2: last minute has 59 seconds
    *  3: clock not synchronized
	*/
	int m_nLeapIndicator; 

	/**
	*  Ported from PB2.x
	*  0: unspecified or unavailable
	*  1: primary reference (e.g., radio clock)
    *  2-15: secondary reference (via NTP or SNTP)
    *  16-255: reserved
	*/
	int m_nStratum; 

	CNtpTime     m_OriginateTime;    ///<  Time when the request was sent from the client to the SNTP server.
	CNtpTime     m_ReceiveTime;      ///<  Time when the request was received by the server
	CNtpTime     m_TransmitTime;     ///<  Time when the server sent the request back to the client
	CNtpTime     m_DestinationTime;  ///<  Time when the reply was received by the client
	double       m_RoundTripDelay;   ///<  Round trip time in seconds
	double       m_LocalClockOffset; ///<  Local clock offset relative to the server
};
                  
/**
*  The actual SNTP class
*  Ported direct from PB2.x
*/
class CSNTPClient //: public CObject
{
public:

	/**
	*  This method has been ported unmodified, direct from PB2.x
	*/
	CSNTPClient(PBModule* pModule);

//General functions
	/**
	*  This method has been ported unmodified, direct from PB2.x
	*/
	BOOL  GetServerTime(char* pszHostName, NtpServerResponse& response, int nPort = 123, BOOL modem = FALSE);

	/**
	*  This method has been ported unmodified, direct from PB2.x
	*/
	DWORD GetTimeout() const { return m_dwTimeout; };

	/**
	*  This method has been ported unmodified, direct from PB2.x
	*/
	void  SetTimeout(DWORD dwTimeout) { m_dwTimeout = dwTimeout; };

	/**
	*  This method has been ported unmodified, direct from PB2.x
	*/
	BOOL  SetClientTime(const CNtpTime& NewTime);

	/**
	*  Set the PB Module associated with this object
	*/
	void SetPBModule(PBModule* pModule);

protected:
	/**
	*  This method has been ported unmodified, direct from PB2.x
	*/
	BOOL EnableSetTimePriviledge();

	/**
	*  This method has been ported unmodified, direct from PB2.x
	*/
	void RevertSetTimePriviledge();

	DWORD            m_dwTimeout;		///<  Ported Unmodified from PB2.x
	HANDLE           m_hToken;			///<  Ported Unmodified from PB2.x
	TOKEN_PRIVILEGES m_TokenPriv;		///<  Ported Unmodified from PB2.x
	BOOL             m_bTakenPriviledge;///<  Ported Unmodified from PB2.x
	PBModule*	m_pModule;				///<  Pointer to PBModule associated with this object, used to send logs to the core.

};

