#include "../../../shared/generated/cpp/InstrumentationBase.h"
#include "keybd.h"

#define ESCAPE_CHAR L'\\'	///< Definition of the escape character

static bool FileExists(const rho::String& str)
{
	bool res = false;
	// Call FindFirstFile on filename
	HANDLE hFind = INVALID_HANDLE_VALUE;
	WIN32_FIND_DATA ffd;

	// Convert char * to TCHAR *
	int requiredSize = mbstowcs(NULL, str.c_str(), 0);
	TCHAR *filename = (TCHAR *)malloc((requiredSize + 1) * sizeof(TCHAR));
	if (!filename)
		return false;
	int size = mbstowcs( filename, str.c_str(), requiredSize + 1); // C4996
	if (size == (size_t)(-1))
	{
		free(filename);
		return false;
	}

	hFind = FindFirstFile(filename, &ffd);

	if (hFind == INVALID_HANDLE_VALUE)
	{
		res = false;
	}
	res = true;

	return res;
}

static void SendKey(TCHAR cKey)
{
	BOOL bEscape = FALSE;
	int nHex = 0;
	BYTE bHex = 0;
	BOOL bVirtual = FALSE;
	const BYTE	CharToVk[256] =
		{
				//     0    1    2    3    4    5    6    7    8    9    A    B    C    D    E    F
				/*0*/ 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x08,0x09,0x00,0x00,0x00,0x0D,0x00,0x00,
				/*1*/ 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x1B,0x00,0x00,0x00,0x00,
				/*2*/ 0x20,0x31,0xDE,0x33,0x34,0x35,0x37,0xDE,0x39,0x30,0x38,0xBB,0xBC,0xBD,0xBE,0xBF,
				/*3*/ 0x30,0x31,0x32,0x33,0x34,0x35,0x36,0x37,0x38,0x39,0xBA,0xBA,0xBC,0xBB,0xBE,0xBF,
				/*4*/ 0x32,0x41,0x42,0x43,0x44,0x45,0x46,0x47,0x48,0x49,0x4A,0x4B,0x4C,0x4D,0x4E,0x4F,
				/*5*/ 0x50,0x51,0x52,0x53,0x54,0x55,0x56,0x57,0x58,0x59,0x5A,0xDB,0x5C,0xDD,0x36,0xBD,
				/*6*/ 0x60,0x41,0x42,0x43,0x44,0x45,0x46,0x47,0x48,0x49,0x4A,0x4B,0x4C,0x4D,0x4E,0x4F,
				/*7*/ 0x50,0x51,0x52,0x53,0x54,0x55,0x56,0x57,0x58,0x59,0x5A,0xDB,0x5C,0xDD,0x60,0x60,
				/*8*/ 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
				/*9*/ 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
				/*A*/ 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
				/*B*/ 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
				/*C*/ 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
				/*D*/ 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
				/*E*/ 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
				/*F*/ 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
				};

	//  Legacy
	bool l_bEscape = FALSE;
	if ( l_bEscape )
	{
		if ( cKey == ESCAPE_CHAR )
		{
			bEscape = !bEscape;

			if ( bEscape )
			{
				return;
			}
		}
	}
	
	if ( bEscape )
	{
		bEscape = FALSE;
		switch(cKey)
		{
			case TEXT('b'):
				cKey = TEXT('\b');
				break;
			case TEXT('f'):
				cKey = TEXT('\f');
				break;
			case TEXT('n'):
				cKey = TEXT('\n');;
				break;
			case TEXT('r'):
				cKey = TEXT('\r');;
				break;
			case TEXT('t'):
				cKey = TEXT('\t');
				break;
			case TEXT('x'):
				bVirtual = FALSE;
				nHex = 1;
				bHex = 0;
				return;
			case TEXT('v'):
				bVirtual = TRUE;
				nHex = 1;
				bHex = 0;
				return;
		}
	}

	switch(nHex)
	{
		case 1:
		case 2:
			if ( iswxdigit(cKey) )
			{
				bHex *= 16;
				if ( iswdigit(cKey) )
				{
					bHex += (BYTE) ( cKey & 0xF );
				}
				else
				{
					bHex += (BYTE)( ( cKey &0xF ) + 9 );
				}

				if ( nHex == 1 )
				{
					nHex++;
					return;
				}
				else
				{
					nHex = 0;
					cKey = (TCHAR)bHex;
					break;
				}
			}

			nHex = 0;
			bVirtual = FALSE;
			return;
		default:
			break;
	}
	
	if ( bVirtual )
	{
		UINT State1[1] = {KeyStateDownFlag|KeyShiftNoCharacterFlag};
		UINT State2[1] = {KeyStateToggledFlag|KeyShiftNoCharacterFlag};
		UINT Shift1[1] = {KeyStateDownFlag|KeyShiftNoCharacterFlag};
		UINT Shift2[1] = {KeyShiftNoCharacterFlag};
		UINT Key1[1] = {0};
		UINT Key2[1] = {0};
		BYTE bVK;
		bVK = (BYTE)cKey;

		PostKeybdMessage((HWND) -1, bVK, State1[0], 0, Shift1, Key1);
		PostKeybdMessage((HWND) -1, bVK, State2[0], 0, Shift2, Key2);
	}
	else
	{
		UINT State1[1] = {KeyStateDownFlag};
		UINT State2[1] = {KeyStateToggledFlag|KeyShiftNoCharacterFlag};
		UINT Shift1[1] = {KeyStateDownFlag};
		UINT Shift2[1] = {KeyShiftNoCharacterFlag};
		UINT Key1[1] = {cKey};
		UINT Key2[1] = {0};
		BYTE bVK;
	
		bVK = CharToVk[(BYTE)cKey];

		PostKeybdMessage((HWND) -1, bVK, State1[0], 1, Shift1, Key1);
		PostKeybdMessage((HWND) -1, bVK, State2[0], 1, Shift2, Key2);
	}

	bVirtual = FALSE;
}

// Save the bitmap 'hbmpSource' into the file specified by 'pszFilename'
static int SaveBitmap(const wchar_t* pszFilename, HDC hdcSource, HBITMAP hbmpSource)
{
	// Determine the size of the source bitmap
	BITMAP bm;
	GetObject(hbmpSource, sizeof(BITMAP), &bm);

	// Create a device independant bitmap (DIB)
	// that is the same size
	BITMAPINFO bmi;
	memset(&bmi, 0, sizeof(bmi));
	bmi.bmiHeader.biHeight = bm.bmHeight;
	bmi.bmiHeader.biWidth = bm.bmWidth;
	bmi.bmiHeader.biPlanes = 1;
	bmi.bmiHeader.biBitCount = 16;
	bmi.bmiHeader.biSize = sizeof(bmi.bmiHeader);
	int ls = (bmi.bmiHeader.biWidth + 3) & (~3);
	bmi.bmiHeader.biSizeImage = ls * bmi.bmiHeader.biHeight * bmi.bmiHeader.biBitCount / 8;
	bmi.bmiHeader.biXPelsPerMeter = 3780;
	bmi.bmiHeader.biYPelsPerMeter = 3780;

	void *pBuffer;
	HBITMAP hTargetBitmap = CreateDIBSection(hdcSource, &bmi, DIB_RGB_COLORS, (void**)&pBuffer, NULL, 0);

	// Copy the source bitmap into the DIB.
	HDC hdcDest = CreateCompatibleDC(hdcSource);
	if (!hdcDest)
		return -1;
	HBITMAP hOldBitmap = (HBITMAP)SelectObject(hdcDest, hTargetBitmap);
	BitBlt(hdcDest, 0, 0, bm.bmWidth, bm.bmHeight, hdcSource, 0, 0, SRCCOPY);
	SelectObject(hdcDest, hOldBitmap);
	DeleteDC(hdcDest);

	// Finally save the bits to disk
	HANDLE hFile = CreateFile(pszFilename, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	if (hFile != INVALID_HANDLE_VALUE)
	{
		BITMAPFILEHEADER hdr;
		hdr.bfType		= ((WORD) ('M' << 8) | 'B');
		hdr.bfSize		= sizeof(hdr) + sizeof(BITMAPINFOHEADER);
		hdr.bfReserved1 = 0;
		hdr.bfReserved2 = 0;
		hdr.bfOffBits	= hdr.bfSize;

		// Write the BITMAPFILEHEADER and BITMAPINFOHEADER
		// structures followed by the raw pixel data
		DWORD dwByteCount;
		WriteFile(hFile, &hdr, sizeof(hdr), &dwByteCount, NULL);
		WriteFile(hFile, &bmi.bmiHeader, sizeof(bmi.bmiHeader), &dwByteCount, NULL);
		WriteFile(hFile, pBuffer, bmi.bmiHeader.biSizeImage, &dwByteCount, NULL);

		CloseHandle(hFile);
	}

	// Free the bitmap resource
	DeleteObject(hTargetBitmap);
	return 1;
}

namespace rho {

using namespace apiGenerator;

class CInstrumentationImpl: public CInstrumentationBase
{
public:
    CInstrumentationImpl(const rho::String& strID): CInstrumentationBase()
    {
    }

    virtual void enable( const rho::Hashtable<rho::String, rho::String>& propertyMap, CMethodResult& oResult){}
    virtual void start(CMethodResult& oResult){}
    virtual void stop(CMethodResult& oResult){}
    virtual void disable(CMethodResult& oResult){}
    virtual void take( const rho::Hashtable<rho::String, rho::String>& propertyMap, CMethodResult& oResult){}

	virtual void CInstrumentationImpl::simulate_key_event_code( int keycode, rho::apiGenerator::CMethodResult& oResult)
	{
		SendKey(keycode);
	}

	virtual void simulate_key_event_string( const rho::String& str, rho::apiGenerator::CMethodResult& oResult)
	{
		for (unsigned int i = 0; i < str.length(); i++)
		{
			SendKey(str[i]);
		}
	}

	virtual void simulate_touch_event( int event_type,  int x,  int y, rho::apiGenerator::CMethodResult& oResult)
	{
		INPUT rawInput;

		long type = 0;
		// We will always use ABSOLUTE events as we want to position the cursor at an absolute point on the screen.
		// If we did not specify ABSOLUTE, we would have to supply how many pixels we have moved in both x and y since the
		// last send_input.
		if (event_type == 0)
		{
			type = MOUSEEVENTF_ABSOLUTE | MOUSEEVENTF_LEFTDOWN;
		}
		else if (event_type == 2)
		{
			type = MOUSEEVENTF_ABSOLUTE | MOUSEEVENTF_LEFTDOWN | MOUSEEVENTF_MOVE;
		}
		else if (event_type == 1)
		{
			type = MOUSEEVENTF_ABSOLUTE | MOUSEEVENTF_LEFTUP;
		}

		// Create a INPUT_MOUSE event using the supplied parameters.
		rawInput.type = INPUT_MOUSE;
		rawInput.mi.mouseData = 0;

		// Map x and y co-ordinates to actual screen co-ordinates of the device.
		rawInput.mi.dx = x*(65536/GetSystemMetrics(SM_CXSCREEN));
		rawInput.mi.dy = y*(65536/GetSystemMetrics(SM_CYSCREEN));
		//rawInput.mi.dwFlags = MOUSEEVENTF_ABSOLUTE | MOUSEEVENTF_LEFTDOWN;
		rawInput.mi.dwFlags = type;

		SendInput(1, &rawInput, sizeof(INPUT));
	}

	virtual void screen_capture( const rho::String& pszFilename, rho::apiGenerator::CMethodResult& oResult)
	{
		int res = -1;
		// Get a handle to the desktop window

		HDC hdcScreen = GetDC(NULL);
		if (!hdcScreen)
			return;

		// Create a new bitmap of the required size and a device
		// context to allow us to draw onto it.
		int width = GetDeviceCaps(hdcScreen, HORZRES);
		int height = GetDeviceCaps(hdcScreen, VERTRES);

		HDC hdcDest = CreateCompatibleDC(hdcScreen);
		if (!hdcDest)
		{
			oResult.set(-1);
			return;
		}
		HBITMAP hbmp = CreateCompatibleBitmap(hdcScreen, width, height);
		if (!hbmp)
		{
			oResult.set(-1);
			return;
		}
		SelectObject(hdcDest, hbmp);

		// Blit (copy) from the source device context (the screen)
		// to the device context that is associated with our
		// offscreen bitmap buffer.
		BitBlt(hdcDest, 0, 0, width, height, hdcScreen, 0, 0, SRCCOPY);

		// Finally save our bitmap to disk
		const size_t fileNameSize = pszFilename.length();
		wchar_t* fileNameW = new wchar_t[fileNameSize];
		mbstowcs (fileNameW, pszFilename.c_str(), fileNameSize);
		if (SaveBitmap(fileNameW, hdcDest, hbmp) > 0)
			res = 1;

		delete[] fileNameW;
		// Free the resources
		DeleteDC(hdcDest);
		DeleteObject(hbmp);

		ReleaseDC(HWND_DESKTOP, hdcScreen);

		oResult.set(res);
	}

	virtual void get_allocated_memory(rho::apiGenerator::CMethodResult& oResult)
	{
		MEMORYSTATUS lpBuffer;
		lpBuffer.dwLength = sizeof(MEMORYSTATUS);
		GlobalMemoryStatus(&lpBuffer);

		// There is no easy way to ask Windows Mobile or CE how much memory the current process is using
		// and it requires that we find the start of the process in memory and count the pages.
		oResult.set(lpBuffer.dwAvailPhys);
	}

	virtual void delete_file( const rho::String& str, rho::apiGenerator::CMethodResult& oResult)
	{
		if (FileExists(str) == true)
		{
			// Convert char * to TCHAR *
			int requiredSize = mbstowcs(NULL, str.c_str(), 0);
			TCHAR *filename = (TCHAR *)malloc((requiredSize + 1) * sizeof(TCHAR));
			if (!filename) {
				oResult.set(false);
				return;
			}
			int size = mbstowcs( filename, str.c_str(), requiredSize + 1); // C4996
			if (size == (size_t)(-1))
			{
				free(filename);
				oResult.set(false);
				return;
			}
			DeleteFile(filename);
			free(filename);
			oResult.set(true);
		}
		else
		{
			oResult.set(false);
		}
	}

	virtual void file_exists( const rho::String& str, rho::apiGenerator::CMethodResult& oResult)
	{
		oResult.set(FileExists(str));
	}

	virtual void re_simulate_navigation(rho::apiGenerator::CMethodResult& oResult)
	{

	}

};


class CInstrumentationSingleton: public CInstrumentationSingletonBase
{
    ~CInstrumentationSingleton(){}
    virtual rho::String getInitialDefaultID();
    virtual void enumerate(CMethodResult& oResult);
};

class CInstrumentationFactory: public CInstrumentationFactoryBase
{
    ~CInstrumentationFactory(){}
    virtual IInstrumentationSingleton* createModuleSingleton();
    virtual IInstrumentation* createModuleByID(const rho::String& strID);
};

extern "C" void Init_Instrumentation_extension()
{
    CInstrumentationFactory::setInstance( new CInstrumentationFactory() );
    Init_Instrumentation_API();
}

IInstrumentation* CInstrumentationFactory::createModuleByID(const rho::String& strID)
{
    return new CInstrumentationImpl(strID);
}

IInstrumentationSingleton* CInstrumentationFactory::createModuleSingleton()
{
    return new CInstrumentationSingleton();
}

void CInstrumentationSingleton::enumerate(CMethodResult& oResult)
{
    rho::Vector<rho::String> arIDs;
    arIDs.addElement("SC1");
    arIDs.addElement("SC2");

    oResult.set(arIDs);
}

rho::String CInstrumentationSingleton::getInitialDefaultID()
{
    CMethodResult oRes;
    enumerate(oRes);

    rho::Vector<rho::String>& arIDs = oRes.getStringArray();
        
    return arIDs[0];
}

}