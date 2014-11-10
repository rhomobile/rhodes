#ifndef _CAMERA_H
#define _CAMERA_H
#include "logging/RhoLog.h"
#include "common/RhoStd.h"
#include "ViewFinder.h"

enum eCamType
{
	eColorCam,
	eImager
};
class ICam
{
public:
	virtual BOOL enumerate(rho::Vector<rho::String>& arIDs, rho::Hashtable<rho::String, eCamType>& camLookUp);
	virtual BOOL getProperty(LPCTSTR szParameterName, WCHAR* szParameterValue)=0;
	virtual BOOL setProperty(LPCTSTR szPropertyName, LPCTSTR szPropertyValue)=0;
	virtual void getSupportedPropertyList(rho::Vector<rho::String>& arrayofNames) =0;
};

class CCamera : public ICam
{
protected:
	CViewFinder m_ViewFinder;
	int m_DesiredWidth;
	int m_DesiredHeight;
	int m_PreviewLeft;
	int m_PreviewTop;
	int m_PreviewWidth;
	int m_PreviewHeight;
	rho::StringW m_CaptureSound;
	rho::StringW m_FileName;
	rho::StringW m_FlashMode;
	//rho::apiGenerator::CMethodResult* m_pImageCaptureCb; //Status Event: Will give the status that the audio has been recorded succesfully or not  
public:
	CCamera();
	virtual ~CCamera();	
    virtual BOOL getProperty(LPCTSTR szParameterName, WCHAR* szParameterValue);
    virtual BOOL setProperty(LPCTSTR szPropertyName, LPCTSTR szPropertyValue);
	virtual void getSupportedPropertyList(rho::Vector<rho::String>& arrayofNames);
private:
	/**
	* checks if two strings are equal
	*/
	BOOL cmp(LPCTSTR tc1, LPCTSTR tc2);
    


};
#endif