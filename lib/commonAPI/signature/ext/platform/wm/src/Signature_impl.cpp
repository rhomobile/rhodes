#include "../../../shared/generated/cpp/SignatureBase.h"
#include "common/RhodesApp.h"
#include "signature.h"

namespace rho {

using namespace apiGenerator;
using namespace common;

class CSignatureImpl: public CSignatureBase
{
public:
	CSignatureImpl (const rho::String& strID) {}
};

class CSignatureSingleton: public CSignatureSingletonBase, public IButtonNotification
{
private:
	HWND hwndWebkit;
	HINSTANCE hInstance;
	CSignature *pSignature;
	bool bFormatImage;
	bool bFullscreen;
	bool bTakeFullscreen;
	//CMethodResult& resultTakeFullscreen ;
	CMethodResult vectorCallback;
	CMethodResult takeFullScreenResult;
	bool vectorCallbackSet;
	rho::String compressionFormat;
	rho::String outputFormat;
	rho::String fileName;
	bool border;
	rho::String penColour;
	int penWidth;
	rho::String bgColour;
	int left;
	int top;
	int width;
	int height;


public:
	/**************************************************************************/
	CSignatureSingleton() : vectorCallback(NULL), takeFullScreenResult(NULL)
	{
		pSignature = 0;
		bFormatImage = true;
		bFullscreen = true;
		bTakeFullscreen = true;
		vectorCallback = NULL;
		takeFullScreenResult = NULL;
		vectorCallbackSet = false;
		//compressionFormat = COMPRESSION_FORMAT_PNG;
		compressionFormat = COMPRESSION_FORMAT_BMP;
		outputFormat = OUTPUT_FORMAT_IMAGE;
		fileName = "signature";
		border = TRUE;
		penColour = "#FF000000";
		penWidth = 3;
		bgColour = "#FFFFFFFF";
		left = 15;
		top = 60;
		width = 200;
		height = 150;
	}

	/**************************************************************************/
	~CSignatureSingleton()
	{
		OutputDebugString (L"==============> CSignatureImpl destructor\r\n");

		if (pSignature)
			delete pSignature;
	}

    void onBeforeNavigate(const wchar_t* szUrlBeingNavigatedTo, const CRhoExtData& oExtData)
    {
        rho::apiGenerator::CMethodResult oResult;
        hide(oResult);
    }

	/**************************************************************************/
	bool IsTrue (LPCTSTR value)
	{
		if (!_wcsicmp (value, L"true"))
			return true;

		if (!_wcsicmp (value, L"yes"))
			return true;

		if (!_wcsicmp (value, L"1"))
			return true;

		return false;
	}

	// Used by the SetPropertyOnSignature method to convert ARGB to RGB
	LPTSTR ARGBtoRGB(LPCTSTR ARGB)
	{
		LPTSTR newValue = new TCHAR[8];
		_wcsnset(newValue, L'\0', 7);

		newValue[0] = L'#';
		wcsncpy_s(newValue+1, 7, ARGB+3, 6);

		return newValue;
	}

	/**************************************************************************/
	void SetPropertyOnSignature (LPCTSTR name, LPCTSTR value)
	{
		LPTSTR newValue;

		if (!pSignature)
			return;

		// Ignore compressionFormat as it will always be BMP on Windows
		// For any of the colours, we need to ignore the ARGB, as the 2.2 API cannot support them

		if (!_wcsicmp (name, L"penwidth"))
		{
			int n = _wtoi (value);
			pSignature->SetLineWidth (n);
			this->penWidth = n;
			DEBUGMSG (TRUE, (L"penWidth set to %d\r\n", n));
		}
		else if (!_wcsicmp (name, L"pencolor"))
		{
			this->penColour = convertToStringA(value);
			if ((wcslen(value) != 7) && (wcslen(value) == 9)) {
				newValue = ARGBtoRGB(value);
				pSignature->SetLineColor (newValue);
				DEBUGMSG (TRUE, (L"ARGB converted penColor set to '%s'\r\n", newValue));
				delete[] newValue;
			} else {
				pSignature->SetLineColor (value);
			}
			DEBUGMSG (TRUE, (L"RGB penColor set to '%s'\r\n", value));
		}
		else if (!_wcsicmp (name, L"bgcolor"))
		{
			this->bgColour = convertToStringA(value);
			if ((wcslen(value) != 7) && (wcslen(value) == 9)) {
				newValue = ARGBtoRGB(value);
				pSignature->SetBgColor (newValue);
				DEBUGMSG (TRUE, (L"ARGB converted bgcolor set to '%s'\r\n", newValue));
				delete[] newValue;
			} else {
    			pSignature->SetBgColor (value);
			}
			DEBUGMSG (TRUE, (L"bgColor set to '%s'\r\n", value));
		}
		else if (!_wcsicmp (name, L"left"))
		{
			int n = _wtoi (value);
			pSignature->SetXpos (n);
			this->left = n;
			DEBUGMSG (TRUE, (L"left set to %d\r\n", n));
		}
		else if (!_wcsicmp (name, L"top"))
		{
			int n = _wtoi (value);
			pSignature->SetYpos (n);
			this->top = n;
			DEBUGMSG (TRUE, (L"top set to %d\r\n", n));
		}
		else if (!_wcsicmp (name, L"width"))
		{
			int n = _wtoi (value);
			pSignature->SetWidth (n);
			this->width = n;
			DEBUGMSG (TRUE, (L"width set to %d\r\n", n));
		}
		else if (!_wcsicmp (name, L"height"))
		{
			int n = _wtoi (value);
			pSignature->SetHeight (n);
			this->height = n;
			DEBUGMSG (TRUE, (L"height set to %d\r\n", n));
		}
		else if (!_wcsicmp (name, L"border"))
		{
			bool b = IsTrue (value);
			pSignature->SetBorder (b);
			this->border = b;
			DEBUGMSG (TRUE, (L"border set to %s\r\n", b ? L"true" : L"false"));
		}
		else if (!_wcsicmp (name, L"outputformat"))
		{
			if (!_wcsicmp (value, convertToStringW(OUTPUT_FORMAT_IMAGE).c_str()))
			{
				bFormatImage = true;
				outputFormat=OUTPUT_FORMAT_IMAGE;
			}
			else if (!_wcsicmp (value, convertToStringW(OUTPUT_FORMAT_DATAURI).c_str()))
			{
				bFormatImage = false;
				outputFormat=OUTPUT_FORMAT_DATAURI;
			}
			// else an error?
			DEBUGMSG (TRUE, (L"outputFormat set to '%s'\r\n", value));
		}
		else if (!_wcsicmp (name, L"fullScreen"))
		{
			bFullscreen = IsTrue (value);
			DEBUGMSG (TRUE, (L"fullScreen set to %s\r\n", bFullscreen ? L"true" : L"false"));
		}
		else if (!_wcsicmp (name, L"fileName"))
		{
			// SetName() adds the .bmp extension
			pSignature->SetName (value);
			this->fileName = convertToStringA( value );
			DEBUGMSG (TRUE, (L"name set to '%s'\r\n", value));
		}
		else
			DEBUGMSG (TRUE, (L"Unknown property name '%s'\r\n", name));
	}

	/**************************************************************************/
	void SetProperties (const rho::Hashtable<rho::String, rho::String>& properties)
	{
		if (!pSignature)
			return;

		// Set Fullscreen to true by default, check it again after properties have been processed
		bFullscreen = true;

		typedef std::map<rho::String, rho::String>::const_iterator it_type;
		for (it_type iterator = properties.begin(); iterator != properties.end(); iterator++)
		{
			SetPropertyOnSignature (convertToStringW(iterator->first).c_str(), convertToStringW(iterator->second).c_str());
		}

		// If Fullscreen or TakeFullscreen then override position and size to make it full screen
		if (bFullscreen || bTakeFullscreen)
		{
			pSignature->SetXpos (0);
			pSignature->SetYpos (0);

			RECT rect;
			GetClientRect (hwndWebkit, &rect);
			pSignature->SetWidth (rect.right);
			pSignature->SetHeight (rect.bottom);
		}
	}

	/**************************************************************************/
    virtual void takeFullScreen( const rho::Hashtable<rho::String, rho::String>& propertyMap, rho::apiGenerator::CMethodResult& oResult)
	{
		TCHAR* wPenColor;
		bTakeFullscreen = true;

		// Save the callback data for when a button is clicked
		takeFullScreenResult = oResult;

		// Get the parent window handle, i.e. the webkit window, and the application instance
		common::CRhoExtData rhodes_data = RHODESAPP().getExtManager().makeExtData();
		hwndWebkit = rhodes_data.m_hBrowserWnd;
		hInstance = (HINSTANCE) (rhodes_data.m_hInstance);

		// Free any existing signature object
		if (pSignature)
		{
			delete pSignature;
			pSignature = 0;
		}

		// Make a signature object
		pSignature = new CSignature (hInstance, hwndWebkit, 0, true);

		// Set properties of signature object before initialise - size/position will be overridden for full screen
		SetProperties (propertyMap);

		// MultiByteToWideChar
		// These are done separately as the properties may have been set by the accessor
		// methods rather than coming in through a property map.
		wPenColor = convertString(penColour);
		if ((wcslen(wPenColor) != 7) && (wcslen(wPenColor) == 9)) {
			// Need to create another string that represents the RGB value
			LPTSTR newValue = ARGBtoRGB(wPenColor);
			pSignature->SetLineColor(newValue);
			delete newValue;
		} else {
			pSignature->SetLineColor(wPenColor);
		}
		free(wPenColor);

		wPenColor = convertString(bgColour);
		if ((wcslen(wPenColor) != 7) && (wcslen(wPenColor) == 9)) {
			// Need to create another string that represents the RGB value
			LPTSTR newValue = ARGBtoRGB(wPenColor);
			pSignature->SetBgColor(newValue);
			delete newValue;
		} else {
			pSignature->SetBgColor(wPenColor);
		}
		free(wPenColor);

		wPenColor = convertString(fileName);
		pSignature->SetName(wPenColor);
		free(wPenColor);
		pSignature->SetLineWidth(this->penWidth);

		// Initialise and show the signature box with buttons
		pSignature->Initialise(true, this);
		pSignature->Show();

		return;
	}

	/**************************************************************************/
	void OnClick (int id)
	{
		DEBUGMSG (TRUE, (L"OnCapture (%d) called\r\n", id));
		LOG(INFO) + __FUNCTION__ + "OnCapture (" + id + ") called";

		switch (id)
		{
		case IButtonNotification::ID_CLEAR:
			pSignature->ClearScreen();
			break;

		case IButtonNotification::ID_CAPTURE:
			// Call capture() passing the saved Rho result data
			// Delete the signature object
			capture(takeFullScreenResult);

			if (pSignature) {
				delete pSignature;
				pSignature = 0;
			}
			break;

		case IButtonNotification::ID_CANCEL:
			if (takeFullScreenResult.hasCallback ())
			{
				rho::Hashtable<rho::String, rho::String> data;
				data.put ("status", "cancel");				
				data.put ("imageUri","");
                data.put ("signature_uri","");
				takeFullScreenResult.set(data);
			}
			pSignature->ClearScreen();
			if (pSignature) {
				delete pSignature;
				pSignature = 0;
			}
			break;
		}
	}

	// Called when MouseUp detected in PSignature
	void MUP(void) {
		// Call the pSignature->SendVectors() method
		if (vectorCallbackSet == true)
		{
			rho::Hashtable<rho::String, rho::Vector<rho::String>> data;
			data.put("vectorArray", pSignature->ReturnVectors());

			vectorCallback.set(data);

			DEBUGMSG (TRUE, (L"MUP called\r\n"));
		}
	}

	TCHAR* convertString(rho::String str)
	{
		int requiredSize = mbstowcs(NULL, str.c_str(), 0);
		TCHAR* tcharString = (TCHAR *)malloc( (requiredSize + 1) * sizeof( TCHAR ));
		int size = mbstowcs( tcharString, str.c_str(), requiredSize + 1); // C4996

		return tcharString;
	}

	/**************************************************************************/
	virtual void show (const rho::Hashtable<rho::String, rho::String>& propertyMap, rho::apiGenerator::CMethodResult& oResult)
	{
		TCHAR* wPenColor;
		bTakeFullscreen = false;

		// Get the parent window handle, i.e. the webkit window, and the application instance
		common::CRhoExtData rhodes_data = RHODESAPP().getExtManager().makeExtData();
		hwndWebkit = rhodes_data.m_hBrowserWnd;
		hInstance = (HINSTANCE) (rhodes_data.m_hInstance);

		// Free any existing signature object
		if (pSignature)
		{
			delete pSignature;
			pSignature = 0;
		}

		// Make a signature object
		pSignature = new CSignature (hInstance, hwndWebkit, 0, true);

		// Set properties of signature object before initialise
		SetProperties (propertyMap);

		// MultiByteToWideChar
		// These are done separately as the properties may have been set by the accessor
		// methods rather than coming in through a property map.
		wPenColor = convertString(penColour);
		if ((wcslen(wPenColor) != 7) && (wcslen(wPenColor) == 9)) {
			// Need to create another string that represents the RGB value
			LPTSTR newValue = ARGBtoRGB(wPenColor);
			pSignature->SetLineColor(newValue);
			delete newValue;
		} else {
			pSignature->SetLineColor(wPenColor);
		}
		free(wPenColor);

		wPenColor = convertString(bgColour);
		if ((wcslen(wPenColor) != 7) && (wcslen(wPenColor) == 9)) {
			// Need to create another string that represents the RGB value
			LPTSTR newValue = ARGBtoRGB(wPenColor);
			pSignature->SetBgColor(newValue);
			delete newValue;
		} else {
			pSignature->SetBgColor(wPenColor);
		}
		free(wPenColor);

		wPenColor = convertString(fileName);
		pSignature->SetName(wPenColor);
		free(wPenColor);

		pSignature->SetBorder(this->border);
		pSignature->SetLineWidth(this->penWidth);
		pSignature->SetXpos(this->left);
		pSignature->SetYpos(this->top);
		pSignature->SetWidth(this->width);
		pSignature->SetHeight(this->height);

		// Initialise and show the signature box without buttons
		pSignature->Initialise(false, this);
		pSignature->Show();

		return;
	}

	/**************************************************************************/
    virtual void capture (rho::apiGenerator::CMethodResult& oResult)
	{
		if (!pSignature) {
			LOG(INFO) + __FUNCTION__ + "No signature object, have you forgotten to call take().";
			return;		// GD - Indicate an error???
		}

		// We always need to create the DIB, even if we are using DataURI 
		pSignature->CreateDIBitmap();

		if (bFormatImage)
		{
			// Fire the callback, if any, passing ok status and filename
			if (oResult.hasCallback ())
			{
				rho::Hashtable<rho::String, rho::String> data;
				data.put ("status", "ok");

				// Get name from signature, prepend protocol
				StringW s (L"file://");
				s += pSignature->GetName();
				data.put ("imageUri", convertToStringA (s));
				DEBUGMSG (TRUE, (L"imageUri (%s) called\r\n", convertToStringA(s)));
                data.put ("signature_uri", convertToStringA (s)); //backward compatibility

				oResult.set (data);
			}
		}
		else
		{
			// Fire the callback, if any, passing ok status and image data
			if (oResult.hasCallback ())
			{
				String image;
				pSignature->GetDataURI (image);
				if (image.length() > 0)
				{
					rho::Hashtable<rho::String, rho::String> data;
					data.put ("status", "ok");
					data.put ("imageUri", image);
					DEBUGMSG (TRUE, (L"OnCapture (%s) called\r\n", image));
                data.put ("signature_uri", image); //backward compatibility

					oResult.set (data);
				}
			}
		}
		/*
		rho::String vectors;
		CreateVectors (vectors);
		data.put ("name-as-per-xml", vectors);
		oResult.set (data);
		*/

		// Call the pSignature->SendVectors() method
		if (vectorCallbackSet == true)
		{
			rho::Hashtable<rho::String, rho::Vector<rho::String>> data;
			data.put("vectorArray", pSignature->ReturnVectors());

			vectorCallback.set(data);
		}
	}

	/**************************************************************************/
	virtual void hide(rho::apiGenerator::CMethodResult& oResult)
	{
		if (!pSignature)
			return;

		// GD - change the behaviour from previously to free the signature object
		// instead of hiding the window. In any case there is no way to re-show
		// the window in the new API.

		delete pSignature;
		pSignature = 0;

		//pSignature->Hide ();
	}

	/**************************************************************************/
    virtual void clear(rho::apiGenerator::CMethodResult& oResult)
	{
		if (!pSignature)
			return;

		pSignature->ClearScreen ();
	}

	virtual void getCompressionFormat(rho::apiGenerator::CMethodResult& oResult) {
	//	oResult.set(compressionFormat);
		oResult.set(COMPRESSION_FORMAT_BMP);
	}

    virtual void setCompressionFormat( const rho::String& compressionFormat, rho::apiGenerator::CMethodResult& oResult) {
	/*	if ((compressionFormat.compare(COMPRESSION_FORMAT_BMP) == 0))
		{
			this->compressionFormat = compressionFormat;
		}
		else
		{
			oResult.setError("Invalid compression format");
		}
		*/		
		//Irrespective anything set we need to set it to BMP
		this->compressionFormat=COMPRESSION_FORMAT_BMP;
	}

    virtual void getOutputFormat(rho::apiGenerator::CMethodResult& oResult) {
		oResult.set(outputFormat);
	}

    virtual void setOutputFormat( const rho::String& outputFormat, rho::apiGenerator::CMethodResult& oResult) {
		if((outputFormat.compare(OUTPUT_FORMAT_IMAGE) == 0) || (outputFormat.compare(OUTPUT_FORMAT_DATAURI) == 0) ) {
			this->outputFormat = outputFormat;			
			if(outputFormat.compare(OUTPUT_FORMAT_IMAGE) == 0)
				bFormatImage=true;
			else if(outputFormat.compare(OUTPUT_FORMAT_DATAURI) == 0)
				bFormatImage=false;
			
		} else {
			oResult.setError("Invalid outputFormat");
		}
	}

    virtual void getFileName(rho::apiGenerator::CMethodResult& oResult) {
		oResult.set(fileName);
	}

    virtual void setFileName( const rho::String& fileName, rho::apiGenerator::CMethodResult& oResult) {
		if (fileName.length() != 0) {
			this->fileName = fileName;

			// MultiByteToWideChar
			int requiredSize = mbstowcs(NULL, fileName.c_str(), 0);
			TCHAR* wFileName = (TCHAR *)malloc( (requiredSize + 1) * sizeof( TCHAR ));
			int size = mbstowcs( wFileName, fileName.c_str(), requiredSize + 1); // C4996

			if (pSignature)
				pSignature->SetName (wFileName);
			// Release memory to stop leak
			free(wFileName);
		}
	}

    virtual void getBorder(rho::apiGenerator::CMethodResult& oResult) {
		oResult.set(border);
	}

    virtual void setBorder( bool border, rho::apiGenerator::CMethodResult& oResult) {
		this->border = border;
	}

    virtual void getPenColor(rho::apiGenerator::CMethodResult& oResult) {
		oResult.set(this->penColour);
	}

    virtual void setPenColor( const rho::String& penColor, rho::apiGenerator::CMethodResult& oResult) {
		this->penColour = penColor;

		// MultiByteToWideChar
		int requiredSize = mbstowcs(NULL, penColor.c_str(), 0);
		TCHAR* wPenColor = (TCHAR *)malloc( (requiredSize + 1) * sizeof( TCHAR ));
		int size = mbstowcs( wPenColor, penColor.c_str(), requiredSize + 1); // C4996

		if (pSignature)
			pSignature->SetLineColor(wPenColor);
		// Release memory to stop leak
		free(wPenColor);
	}

    virtual void getPenWidth(rho::apiGenerator::CMethodResult& oResult) {
		oResult.set(this->penWidth);
	}

    virtual void setPenWidth( int penWidth, rho::apiGenerator::CMethodResult& oResult) {
		// Do we need to actually check this to make sure it isn't a stupid value?
		this->penWidth = penWidth;

		if (pSignature) {
			pSignature->SetLineWidth(this->penWidth);
		}
	}

    virtual void getBgColor(rho::apiGenerator::CMethodResult& oResult) {
		oResult.set(this->bgColour);
	}

    virtual void setBgColor( const rho::String& bgColor, rho::apiGenerator::CMethodResult& oResult) {
		this->bgColour = bgColor;

		// MultiByteToWideChar
		int requiredSize = mbstowcs(NULL, bgColor.c_str(), 0);
		TCHAR* wBgColor = (TCHAR *)malloc( (requiredSize + 1) * sizeof( TCHAR ));
		int size = mbstowcs( wBgColor, bgColor.c_str(), requiredSize + 1); // C4996

		if (pSignature)
			pSignature->SetBgColor(wBgColor);
		// Release memory to stop leak
		free(wBgColor);		
	}

    virtual void getLeft(rho::apiGenerator::CMethodResult& oResult) {
		oResult.set( this->left );
	}

    virtual void setLeft( int left, rho::apiGenerator::CMethodResult& oResult) {
		if (left >= 0) {
			this->left = left;
			if (pSignature) {
				pSignature->SetXpos(this->left);
			}
		}
	}

    virtual void getTop(rho::apiGenerator::CMethodResult& oResult) {
		oResult.set( this->top );
	}

    virtual void setTop( int top, rho::apiGenerator::CMethodResult& oResult) {
		if (top >= 0) {
			this->top = top;
			if (pSignature) {
				pSignature->SetYpos(this->top);
			}
		}
	}

    virtual void getWidth(rho::apiGenerator::CMethodResult& oResult) {
		oResult.set(width);
	}

    virtual void setWidth( int width, rho::apiGenerator::CMethodResult& oResult) {
		if (width >= 0) {
			this->width = width;
			if (pSignature) {
				pSignature->SetWidth(this->width);
			}
		}
	}

    virtual void getHeight(rho::apiGenerator::CMethodResult& oResult) {
		oResult.set(this->height);
	}

    virtual void setHeight( int height, rho::apiGenerator::CMethodResult& oResult) {
		if (height >= 0) {
			this->height = height;
			if (pSignature) {
				pSignature->SetHeight(this->height);
			}
		}
	}
    
    virtual void setVectorCallback(rho::apiGenerator::CMethodResult& oResult)
	{
		LOG(INFO) + __FUNCTION__ + "Calling setVectorCallback";

		if (oResult.hasCallback())
		{
			vectorCallback = ((CMethodResult) oResult);
			vectorCallbackSet = true;
		}
		else
		{
			vectorCallback = NULL;
			vectorCallbackSet = false;
		}
	}

	// Called by Capture method when finished.  This will retrieve all the data as a vector.
	void sendVectors(rho::String &vectors)
	{
		LOG(INFO) + __FUNCTION__ + "Calling sendVectors";
		pSignature->ReturnVectors();
	}
};

class CSignatureFactory: public CSignatureFactoryBase
{
    ~CSignatureFactory(){}
	virtual ISignatureSingleton* createModuleSingleton();
};

extern "C" void Init_Signature_extension()
{
    CSignatureFactory::setInstance( new CSignatureFactory() );
    Init_Signature_API();

    RHODESAPP().getExtManager().requireRubyFile("RhoSignatureApi");

}

ISignatureSingleton* CSignatureFactory::createModuleSingleton()
{
    return new CSignatureSingleton();
}

}
