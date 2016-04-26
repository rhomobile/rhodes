#include <StrucInf.h>
#include "cradleapi32.h"




#define		RET_CRADLE_ALREADY_OPENED	10
#define		RET_ERROR					-1
#define		SERIALNUMBER_LENGTH			15
#define		PARTNUMBER_LENGTH			13		


	typedef DWORD (*CRADLE_OPENPROC)(PHANDLE);
	typedef DWORD (*CRADLE_CLOSEPROC)(HANDLE);
	typedef DWORD (*CRADLE_UNLOCKPROC)(HANDLE,BYTE,LPCRADLE_UNLOCK_LED_INFO);
	typedef DWORD (*CRADLE_GETFASTCHARGESTATEPROC)(HANDLE,PBOOL);
	typedef DWORD (*CRADLE_GETCRADLEINFOPROC)(HANDLE,LPCRADLE_DEVICE_INFO);
	typedef DWORD (*CRADLE_GETCRADLECONFIGPROC)(HANDLE,LPCRADLE_MATRIX_CONFIG);
	typedef DWORD (*CRADLE_SETCRADLECONFIGPROC)(HANDLE,LPCRADLE_MATRIX_CONFIG);
	typedef DWORD (*CRADLE_GETVERSIONPROC)(HANDLE,LPCRADLE_VERSION_INFO);
	typedef DWORD (*CRADLE_SETFASTCHARGESTATEPROC)(HANDLE,BOOL);

static TCHAR* g_szCradleMatrixEventNames[] = 
{
	L"currentRowId",
	L"currentColumnId",
	L"currentWallId",
	NULL
};

static TCHAR* g_szCradleInfoEventNames[] = 
{
	L"hardwareId",
	L"partNumber",
	L"serialNumber",
	L"mfgDate",
	L"driverVersion",
	L"cApiVersion",
	L"firmwareVersion",
	L"chargeState",
	NULL
};

class CCradle
{
public:
	HANDLE	m_hCradle;
	DWORD m_CradleUnlockOnDuration;
	DWORD m_CradleUnlockOffDuration;
	BYTE m_CradleUnlockTimeOut;
	PBSTRUCT PBStructure;



	CCradle();
	virtual ~CCradle();
	void GetCradleInfo();
	void GetCradleMatrix();
	void GetCradleVersion();
	void UnlockCradle();
	void SetCradleUnlockOnDuration(DWORD OnDuration);
	void SetCradleUnlockOffDuration(DWORD OffDuration);
	void SetCradleUnlockTimeOut(int timeOut);
	void SetCradleMatrixColumn(WORD ColumnID);
	void SetCradleMatrixRow(WORD RowID);
	void SetCradleMatrixWall(WORD WallID);
	void SetCradleFastChargeState(BOOL CradleFastChargeState);
	void CleanUP();
	void SetDefaultValues();


	DWORD OpenCradle();
	DWORD CloseCradle();
	TCHAR m_CradleMatrixEvent[MAX_URL];
	TCHAR m_CradleInfoEvent[MAX_URL];


private:
	HMODULE hCradleModule;//Handle to cradle module DLL
	
	bool UnloadLibrary();
	void INIT();

	bool m_bcradledllLoaded;




	CRADLE_OPENPROC CRADLE_OPEN;
	CRADLE_CLOSEPROC CRADLE_CLOSE;
	CRADLE_UNLOCKPROC CRADLE_UNLOCK;
	CRADLE_GETFASTCHARGESTATEPROC CRADLE_GETFASTCHARGESTATE;
	CRADLE_GETCRADLEINFOPROC CRADLE_GETCRADLEINFO;
	CRADLE_GETCRADLECONFIGPROC CRADLE_GETCRADLECONFIG;
	CRADLE_SETCRADLECONFIGPROC CRADLE_SETCRADLECONFIG;
	CRADLE_GETVERSIONPROC CRADLE_GETVERSION;
	CRADLE_SETFASTCHARGESTATEPROC CRADLE_SETFASTCHARGESTATE;

	DWORD GetCradleInfoData();
	DWORD GetCradleMatrixData();
	DWORD GetCradleVersionData();
	DWORD GetCradleFastChargeState();
	DWORD SetCradleMatrixData(CRADLE_MATRIX_CONFIG* lpCradleMatrixData);
	DWORD UnlockCradle(BYTE bCradleTimeout,LPCRADLE_UNLOCK_LED_INFO lpCradleUnlockLEDInfo);
	DWORD SetCradleFastChargeStateData(BOOL CradleFastChargeState);
	DWORD SetCradleMatrixROWData(WORD RowID);
	DWORD SetCradleMatrixColumnData(WORD ColumnID);
	DWORD SetCradleMatrixWallData(WORD WallID);
	DWORD Unlock();
	

	

};
