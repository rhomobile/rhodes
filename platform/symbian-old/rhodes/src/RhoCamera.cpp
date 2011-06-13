/*
 ============================================================================
 Name		 : CRhoCamera.cpp
 Author	     : 
 Version	 : 1.0
 Copyright   : Copyright (C) 2008-2009 Rhomobile. All rights reserved.

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
 Description : CRhoCamera implementation
 ============================================================================
 */

#include "RhoCamera.h"
#include "rhodesAppView.h"

#include <imagecodecdata.h>
#include <imageconversion.h>

#include <aknglobalnote.h>
#include <aknviewappui.h>   
#include <eikenv.h>   
#include <barsread.h> 
#include <pathinfo.h>
#include <bautils.h>


#include "tcmalloc/rhomem.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

_LIT(KJpgFileExtension, ".jpg");
_LIT(KTimeFormat,"%F%D%M%H%1%T");

_LIT8(KMimeType,"image/jpeg"); 
 
// The name for the image file. 
_LIT(KImageFileName, "RhoImage"); 

extern "C" void perform_notification(char* callback, char* params);

CRhoCamera::CRhoCamera() :
	CActive(EPriorityStandard), iZoomFactor(NULL), iCapturePrepared(EFalse),
			iEncoder(NULL), iCameraReserved(EFalse)
	{
	CActiveScheduler::Add(this);
	iState = EEngineIdle;
	}

CRhoCamera* CRhoCamera::NewL(CRhodesAppView* appView, const TRect& aRect)
	{
	CRhoCamera* self = new (ELeave) CRhoCamera();
	CleanupStack::PushL(self);
	self->ConstructL(appView, aRect);
	CleanupStack::Pop(self);
	return self;
	}

CRhoCamera::~CRhoCamera()
	{
	if (iEncoder)
		delete iEncoder;
	if (iCamera)
		delete iCamera;
	if (iBitmapPortraitVF)
		delete iBitmapPortraitVF;
	if (iBitmapSave)
		delete iBitmapSave;
	if (iBitmapSnap)   
		delete iBitmapSnap;
	// Cancel any outstanding requests   
	Cancel();
	
	if (iImagePath)   
		delete iImagePath; 
	}

void CRhoCamera::ConstructL(CRhodesAppView* appView, const TRect& aRect)
	{
	iAppView = appView;
	iEikEnv = CEikonEnv::Static();
	if (!CCamera::CamerasAvailable())
		{
		HandleError(KErrHardwareNotAvailable);
		return;
		}

	iCamera = CCamera::NewL(*this, 0);

	iCameraReserveComplete = ETrue;//No requests issued yet   

	// Gets information about the camera device. refer to SDK for more info.   
	iCamera->CameraInfo(iInfo);
	iDisplayMode = DisplayMode();
	iColorDepth = ImageFormat();
	iColorDepthHW = ImageFormatMax();
	ClientRectChangedL(aRect); 
	
	TFileName path = PathInfo::PhoneMemoryRootPath(); //Default    
	path.Append( PathInfo::ImagesPath() );   
	iImagePath = HBufC::NewL( path.Length() );   
	iImagePath->Des().Copy( path );

	//Create snap image   
	iBitmapSnap = new (ELeave) CWsBitmap(iEikEnv->WsSession());   
	User::LeaveIfError( iBitmapSnap->Create(aRect.Size(), DisplayMode()));
	}

TDisplayMode CRhoCamera::DisplayMode() const
	{
	TInt color;
	TInt gray;
	TDisplayMode displayMode = iEikEnv->WsSession().GetDefModeMaxNumColors(
			color, gray);
	return displayMode;
	}

CCamera::TFormat CRhoCamera::ImageFormat() const
	{
	switch (iDisplayMode)
		{
		case EColor16M:
			return CCamera::EFormatFbsBitmapColor16M;
		case EColor64K:
			return CCamera::EFormatFbsBitmapColor64K;
		case EColor4K:
			return CCamera::EFormatFbsBitmapColor4K;
		default:
			return CCamera::EFormatFbsBitmapColor4K;
		}
	}

CCamera::TFormat CRhoCamera::ImageFormatMax() const
	{
	if (iInfo.iImageFormatsSupported & CCamera::EFormatFbsBitmapColor16M)
		{
		return CCamera::EFormatFbsBitmapColor16M;
		}
	else if (iInfo.iImageFormatsSupported & CCamera::EFormatFbsBitmapColor64K)
		{
		return CCamera::EFormatFbsBitmapColor64K;
		}
	else
		{
		return CCamera::EFormatFbsBitmapColor4K;
		}
	}

void CRhoCamera::StopViewFinder()
	{
	if (iCameraReserved && iCameraReserveComplete)
		{
		iCamera->StopViewFinder();
		iAppView->DrawImageNow(NULL);
		}
	}

void CRhoCamera::DoViewFinderL()
	{
	//Default, always supported by API   
	iCamera->SetExposureL();

	if (iZoomFactor && !iCapturePrepared)
		{
		//Camera released, restore zoom setting   
		iCamera->SetDigitalZoomFactorL(iZoomFactor);
		}

	if (iInfo.iOptionsSupported & TCameraInfo::EViewFinderBitmapsSupported)
		{
		//Preferred   
		if (iInfo.iOptionsSupported & TCameraInfo::EImageCaptureSupported
				&& !iCapturePrepared)
			{
			//Request largest image    
			iCamera->PrepareImageCaptureL(iColorDepthHW, 0);
			iCapturePrepared = ETrue;
			}

		// Start the view finding, and transfer the view finder data.   
        // Bitmaps are returned by MCameraObserver::ViewFinderFrameReady().    
        iCamera->StartViewFinderBitmapsL( iPortraitSize );   
		}
	else if (iInfo.iOptionsSupported & TCameraInfo::EViewFinderDirectSupported)
		{
		User::Leave(KErrNotSupported);//Not verified   
		}
	else
		{
		//Image can be taken without viewfinding   
		if (iInfo.iOptionsSupported & TCameraInfo::EImageCaptureSupported
				&& !iCapturePrepared)
			{
			iCamera->PrepareImageCaptureL(iColorDepthHW, 0);
			iCapturePrepared = ETrue;
			}
		}
	}

void CRhoCamera::StartViewFinderL()
	{
	ReserveCameraL();
	}

void CRhoCamera::SnapL()
	{
	if (iCameraReserved && iCameraReserveComplete && !iPowering)
		{
		//If the Engine is ready   
		iCamera->StopViewFinder();

		// According to on-board camera API, MCameraObserver::ImageReady()   
		// will be called back when the following API is called.   
		iCamera->CaptureImage();
		}
	else
		{
		User::Leave(KErrNotReady);
		}
	}


void CRhoCamera::SetMode(TCameraState aMode)
	{
	iMode = aMode;
	}

void CRhoCamera::DeleteEncoder()
	{
	delete iEncoder;
	iEncoder = NULL;

	delete iBitmapSave;
	iBitmapSave = NULL;
	}

void CRhoCamera::SaveImageL()   
    {       
    // Get the next usable file name   
    GetNextUsableFileName();   
   
    // Save the image   
    SaveImageL( &iNewFileName, &(iEikEnv->FsSession()) );   
    } 

void CRhoCamera::GetNextUsableFileName()   
    {      
    TInt index = 0;   
   
    do {   
        iNewFileName.Copy( iImagePath->Des() );   
        iNewFileName.Append( KImageFileName );   
   
        TBuf<50> buf;
		TTime time;
		time.HomeTime();
		time.FormatL( buf, KTimeFormat);
		
        iNewFileName.Append( buf );   
           
        iNewFileName.Append( KJpgFileExtension );   
        if ( !BaflUtils::FileExists( iEikEnv->FsSession(), iNewFileName ) )   
            break;   
   
        index ++;   
        } while ( 1 );   
    }

void CRhoCamera::SaveImageL(const TFileName* aNewFilePathAndName, RFs* aFs)
	{
	if (!iEncoder)
		{
		if (aFs && aNewFilePathAndName)
			{
			iEncoder = CImageEncoder::FileNewL(*aFs, *aNewFilePathAndName,
					KMimeType);
			}
		else
			{
			User::Leave(KErrArgument);
			}
		}

	// If it is not busy, start to do the conversion.   
	if (!IsActive())
		{
		iEncoder->Convert(&iStatus, *iBitmapSave);
		iState = EConvertingImage;

		SetActive();
		}
	
	//send callback
	if ( iAppView->GetCallbackUrl() )
		{
			char params[500] = {0};
			char* tmp = GetNewFileName();
			sprintf(params,"status=ok&image_uri=%s",tmp);
			free(tmp);
			
			tmp = iAppView->CanonicalizeURL( iAppView->GetCallbackUrl() );
			perform_notification(tmp, params);
			free(tmp);
		}
	}

char* CRhoCamera::GetNewFileName()
	{
		TInt length = iNewFileName.Length();
	 
		HBufC8* buffer = HBufC8::NewLC(length);
		buffer->Des().Copy(iNewFileName);
	 
		char* str = (char*)malloc(length + 1);
		Mem::Copy(str, buffer->Ptr(), length);
		str[length] = '\0';
	 
		CleanupStack::PopAndDestroy(buffer);
	 
		return str;
	}

void CRhoCamera::ReserveCameraL()
	{
	iStart = ETrue;
	if (iCameraReserved && !iCameraReserveComplete)
		{
		//Ongoing, no need to issue again               
		User::Leave(0);
		}

	if (!iCameraReserved && iCameraReserveComplete)
		{
		iCameraReserved = ETrue;
		iCameraReserveComplete = EFalse;
		iCamera->Reserve();//Async   
		}

	if (iCameraReserved && iCameraReserveComplete)
		{
		if (!iPowering)
			{
			iPowering = ETrue;
			iCamera->PowerOn();
			}
		else
			{
			User::Leave(0);
			}
		}
	}

void CRhoCamera::ReleaseCamera()
	{
	iStart = EFalse;
	if (iCameraReserved && iCameraReserveComplete)
		{
		iCamera->Release();
		iCameraReserved = EFalse;
		}

	iCapturePrepared = EFalse;
	}

void CRhoCamera::PowerOff()
	{
	if (!iPowering && iCameraReserved)
		{
		iCamera->PowerOff();
		ReleaseCamera();
		}
	}

void CRhoCamera::ReserveComplete(TInt aError)   
    {   
    iCameraReserveComplete = ETrue;   
    if ( aError )   
        {   
        iCameraReserved = EFalse;   
        HandleError( aError );   
        }   
       
    if ( iStart )   
        {   
        iPowering = ETrue;   
        iCamera->PowerOn();   
        }   
    else   
        {   
        ReleaseCamera();   
        }   
    }   
   
void CRhoCamera::PowerOnComplete(TInt aError)   
    {   
    HandleError( aError );   
    iPowering = EFalse;   
    if ( iStart ) //The Operation is not cancelled   
        {   
        TRAPD( err, DoViewFinderL() );   
        HandleError( err );   
        }   
    else   
        {   
        ReleaseCamera();   
        }   
    }   
   
void CRhoCamera::ViewFinderFrameReady(CFbsBitmap& aFrame)   
    {   
		TRAPD(ignore, ClipL(aFrame));   
		//iAppView->DrawImageNow(iBitmapPortraitVF);
		iCamera->StopViewFinder();
		iCamera->CaptureImage();
    }   
   
void CRhoCamera::ImageReady(CFbsBitmap* aBitmap,HBufC8* /*aData*/,   
                                      TInt aError)   
    {   
    TInt err(KErrNone);   
    if ( !aError )   
        {   
        iBitmapSave = aBitmap;   
        TRAP(err, DrawL());   
        HandleError( err );   
        }   
    else   
        {   
        HandleError( aError );     
        }   
    }   
     
void CRhoCamera::FrameBufferReady(MFrameBuffer*  /*aFrameBuffer*/,   
                                            TInt /*aError*/)   
    {   
    // We are not using video capture   
    }  

void CRhoCamera::ClipL(const CFbsBitmap& aFrame)
	{
	TSize size = aFrame.SizeInPixels();
	TInt x1 = (size.iWidth - iPortraitSize.iWidth) / 2;
	TInt x2 = x1 + iPortraitSize.iWidth;
	TInt y1 = (size.iHeight - iPortraitSize.iHeight) / 2;
	TInt y2 = y1 + iPortraitSize.iHeight;

	CFbsBitGc* fbsBitGc = CFbsBitGc::NewL(); //graphic context   
	CleanupStack::PushL(fbsBitGc);
	CFbsBitmapDevice* portraitImageDevice = CFbsBitmapDevice::NewL(
			iBitmapPortraitVF);
	fbsBitGc->Activate(portraitImageDevice);

	fbsBitGc->BitBlt(TPoint(0, 0), &aFrame, TRect(x1, y1, x2, y2));

	delete portraitImageDevice;
	CleanupStack::PopAndDestroy(fbsBitGc);
	}

TRect CRhoCamera::Portrait(const CFbsBitmap* aBitmap)
	{
	TRect portrait = TRect();
	if (aBitmap)
		{
		TSize size = aBitmap->SizeInPixels();
		TInt portx = iPortraitSize.iWidth * size.iWidth / iLandscapeSize.iWidth;
		TInt porty = iPortraitSize.iHeight * size.iHeight
				/ iLandscapeSize.iHeight;
		TInt x1 = (size.iWidth - portx) / 2;
		TInt x2 = x1 + portx;
		TInt y1 = (size.iHeight - porty) / 2;
		TInt y2 = y1 + porty;
		portrait.SetRect(x1, y1, x2, y2);
		}
	return portrait;
	}

void CRhoCamera::DrawL()
	{
	CFbsBitGc* fbsBitGc = CFbsBitGc::NewL(); //graphic context   
	CleanupStack::PushL(fbsBitGc);
	User::LeaveIfError((*iBitmapSnap).Resize(iPortraitSize));

	CFbsBitmapDevice* bmpDevice = CFbsBitmapDevice::NewL(iBitmapSnap);
	fbsBitGc->Activate(bmpDevice);

	TRect portraitRect = Portrait(iBitmapSave);
	//Shrink to snap image size   
	fbsBitGc->DrawBitmap(TRect(iPortraitSize), iBitmapSave, portraitRect);
	delete bmpDevice;

	//Full color image   
	CFbsBitmapDevice* bmpDeviceSave = CFbsBitmapDevice::NewL(iBitmapSave);
	CleanupStack::PushL(bmpDeviceSave);
	fbsBitGc->Activate(bmpDeviceSave);
	fbsBitGc->DrawBitmap(TRect(iPortraitSize), iBitmapSave, portraitRect);
	//To be saved   
	User::LeaveIfError(iBitmapSave->Resize(iPortraitSize));
	CleanupStack::PopAndDestroy(bmpDeviceSave);

	CleanupStack::PopAndDestroy(fbsBitGc);

	// Start to save the image.   
	StartToSaveImage();
	}

TInt CRhoCamera::SetZoomFactorL(TBool aEnable)
	{
	TInt bitmapCount = ECameraZoomLimit - ECameraZoom2Uid;
	//both 0 and 1 indicate that zoom functionality is not supported         
	if (iInfo.iMaxZoomFactor != 0 && iInfo.iMaxZoomFactor != 1)
		{
		if (aEnable && iZoomFactor < iInfo.iMaxZoom)
			{
			iZoomFactor++;
			}
		if (!aEnable && iZoomFactor > iInfo.iMinZoom)
			{
			iZoomFactor--;
			}
		iCamera->SetZoomFactorL(iZoomFactor);
		//Zoom ind. bitmap offset    
		return (iInfo.iMaxZoom > bitmapCount) ? KErrNotFound : iZoomFactor - 1;
		}
	if (iInfo.iMaxDigitalZoomFactor != 0 && iInfo.iMaxDigitalZoomFactor != 1)
		{
		if (aEnable && iZoomFactor < iInfo.iMaxDigitalZoom)
			{
			iZoomFactor++;
			}
		if (!aEnable && iZoomFactor > 0)
			{
			iZoomFactor--;
			}
		iCamera->SetDigitalZoomFactorL(iZoomFactor);
		iCapturePrepared = EFalse;
		//Zoom ind. bitmap offset    
		return (iInfo.iMaxDigitalZoom > bitmapCount) ? KErrNotFound
				: iZoomFactor - 1;
		}
	return KErrNotFound;
	}

void CRhoCamera::HandleError(TInt aError)
	{
	TInt reason(KErrNone);
	switch (aError)
		{
		case KErrNone:
			reason = KErrNone;
			break;
		case KErrNoMemory:
			iEikEnv->HandleError(aError);
			reason = ECameraOverflow;
			break;
		case KErrInUse:
			reason = KErrInUse; 
			break;
		case KErrHardwareNotAvailable:
			reason = ECameraHwFailure;
			break;
		case KErrTimedOut:
			reason = ECameraOverflow;
			break;
		default:
			iEikEnv->HandleError(aError);
			reason = ECameraOverflow;
		}

	if (reason)
		{
		char message[256] = {0};
		sprintf(message,"%s %d", "Error: ", reason);
		//showMessageBox(buf);
		
		if ( iAppView->GetCallbackUrl() )
		{
			char params[500] = {0};
			sprintf(params,"status=error&message=%s",message);
			
			char* tmp = iAppView->CanonicalizeURL( iAppView->GetCallbackUrl() );
			perform_notification(tmp, params);
			free(tmp);
		}
		
		delete iBitmapSave;
		iBitmapSave = NULL;
		}
	}

void CRhoCamera::DoCancel()
	{
	if (iState == EConvertingImage)
		{
		iEncoder->Cancel();
		DeleteEncoder();
		}
	}

void CRhoCamera::RunL()
	{
	if (iStatus == KErrNone)
		{
		switch (iState)
			{
			case EStartToSaveImage:
				{
				SaveImageL();
				break;
				}
			case EConvertingImage:
				{
				DeleteEncoder(); //Release captured image file   
				// Shows the status to "Image saved"   
				iState = EConverted;
				break;
				}
			default:
				break;
			}
		}
	}

void CRhoCamera::StartToSaveImage()
	{
	TRequestStatus* status = (&iStatus);
	iState = EStartToSaveImage;
	SetActive();

	User::RequestComplete(status, KErrNone);
	}

TBool CRhoCamera::IsImageConversionInProgress()
	{
	return (iState == EConvertingImage || iState == EStartToSaveImage);
	}

TEngineState CRhoCamera::GetEngineState()
	{
	return iState;
	}

void CRhoCamera::SetEngineState(TEngineState aState)
	{
	iState = aState;
	}

TBool CRhoCamera::IsCameraUsedByAnotherApp()
	{
	return (!iCameraReserved);
	}

void CRhoCamera::ClientRectChangedL(const TRect& aRect)
	{
	// The given client rect size is the same as the landscape picture size   
	iLandscapeSize = aRect.Size();
	// In portrait mode the height is the same, but the width needs to be   
	// calculated according to the aspect ratio   
	iPortraitSize = TSize((aRect.Size().iHeight * aRect.Size().iHeight
			/ aRect.Size().iWidth), aRect.Size().iHeight);

	iBitmapPortraitVF = new (ELeave) CWsBitmap(iEikEnv->WsSession());
	TInt createError = iBitmapPortraitVF->Create(iPortraitSize, iDisplayMode);
	User::LeaveIfError(createError);
	}
