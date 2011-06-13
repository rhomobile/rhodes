/*
 ============================================================================
 Name		: CRhoCamera.h
 Author	  : Anton Antonov
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
 Description : CRhoCamera declaration
 ============================================================================
 */

#ifndef CRHOCAMERA_H
#define CRHOCAMERA_H

// INCLUDES
#include <e32std.h>
#include <e32base.h>
#include <f32file.h>
#include <gdi.h>
#include <ecam.h>

#include "rhodes.hrh"
// CLASS DECLARATION
class CWsBitmap;
class CImageEncoder;
class CEikonEnv;
class CRhodesAppView;
/**
 *  CRhoCamera
 * 
 */
class CRhoCamera : public CActive, MCameraObserver
	{
public: // Constructors and destructor 
      
		static CRhoCamera* NewL( CRhodesAppView* appView, const TRect& aRect ); 
         
        virtual ~CRhoCamera(); 
 
    private: 

        CRhoCamera(); 
 
        /** 
         * Symbian OS constructor. 
         */ 
       void ConstructL(CRhodesAppView* appView, const TRect& aRect); 
 
    public: // New Functions 
        /** 
         * Check whether the camera is being used by another app. 
         * @return, true if it used by another app.  
         */ 
        TBool IsCameraUsedByAnotherApp(); 
 
        /** 
         * Set the engine state	 
         */ 
        void SetEngineState( TEngineState aState ); 
 
        /** 
         * Get the engine state	 
         * @return EEngineState 
         */ 
        TEngineState GetEngineState(); 
 
        /** 
         * Returns whether the imageconversion is in progress or not	 
         */ 
        TBool IsImageConversionInProgress(); 
 
        /** 
         * Start to save image 
         */ 
        void StartToSaveImage(); 
 
        /** 
         * Starts view finding 
         * @return void 
         */ 
        void StartViewFinderL(); 
 
        /** 
         * Stops view finding 
         * @return void 
         */ 
        void StopViewFinder(); 
 
        /** 
         * Provides save and post-exposure images 
         * @return void 
         */ 
        void SnapL(); 
 
        /** 
         * Sets zoom on/off 
         * @param aEnable 
         * @return TInt, Offset for the zoom indicator bitmap 
         */ 
        TInt SetZoomFactorL( TBool aEnable ); 
 
        /** 
         * Converts and saves bitmap to JPEG image 
         * @param aQuality  
         * @param aNewFilePathAndName 
         * @param aFs  
         * @return void 
         */ 
        void SaveImageL(const TFileName* aNewFilePathAndName, RFs* aFs); 
 
        /** 
		 * Converts and saves bitmap to JPEG image
		 */
        void SaveImageL();
        
        /** 
         * Destructs JPEG encoder 
         * @return void 
         */ 
        void DeleteEncoder(); 
 
        /** 
         * Returns default display mode 
         * @return TDisplayMode 
         */ 
        TDisplayMode DisplayMode() const; 
 
        /** 
         * Sets Exposure time according to Camera mode 
         * @param Camera State 
         * @return void  
         */ 
        void SetMode( TCameraState aMode ); 
         
        /** 
         * Reserves camera 
         * @return void 
         */ 
        void ReserveCameraL(); 
 
        /** 
         * Releases camera 
         * @return void 
         */ 
        void ReleaseCamera(); 
 
        /** 
         * Switches off camera power 
         * @return void 
         */ 
        void PowerOff(); 
 
        /** 
         * Displays error message 
         * @param An error 
         * @return void 
         */ 
        void HandleError( TInt aError );   
         
        /** 
        * Notify the engine if the client rect size changes 
        * @param The new client rect 
        */ 
        void ClientRectChangedL(const TRect& aRect); 
 

        /** 
         * Starts view finding and prepares image capturing 
         * @return void 
         */ 
        void DoViewFinderL();  
        
    private: 
 
        /** 
         * Calculates portrait image size from bigger snapped image  
         * keeping the aspect ratio unchanged 
         * @return The portrait image size 
         */ 
        TRect Portrait( const CFbsBitmap* aBitmap); 
         
        /** 
         * Returns camera image format to be used with current display mode 
         * @return CCamera::TFormat 
         */ 
        CCamera::TFormat ImageFormat() const; 
 
        /** 
         * Returns highest color mode supported by HW 
         * @return CCamera::TFormat 
         */ 
        CCamera::TFormat ImageFormatMax() const; 
 
        /** 
         * Clips the viewfinding images according to portrait mode size. 
         * @param A bitmap to be clipped 
         * @return void 
         */ 
        void ClipL(const CFbsBitmap& aFrame); 
 
        /** 
         * Draws captured image on the screen, modifies if needed 
         * @return void 
         */ 
        void DrawL(); 
 

        /** 
         * Camera reservation is complete.
         * 
         * Called asynchronously when CCamera::Reserve() completes.
         * 
         * @param  aError An error on failure and KErrNone on success.
		 */
        void ReserveComplete(TInt aError);
 
        /** 
         * Indicates camera power on is complete.

		Called on completion of CCamera:PowerOn().

		@param  aError
				KErrNone on success, KErrInUse if the camera is in
				use by another client or KErrNoMemory if insufficient system memory is available.
		*/
		void PowerOnComplete(TInt aError);

		/** Tests whether transfer of view finder data has completed.

		Called periodically in response to the use of CCamera::StartViewFinderBitmapsL().

		@param  aFrame
				The view finder frame.
		*/
		void ViewFinderFrameReady(CFbsBitmap& aFrame);

		/** Transfers the current image from the camera to the client.

		Called asynchronously when CCamera::CaptureImage() completes.

		@param  aBitmap
				On return, a pointer to an image held in CFbsBitmap form if
				this was the format specified in CCamera::PrepareImageCaptureL().
		@param  aData
				On return, a pointer to an HBufC8 if this was the format specified
				in CCamera::PrepareImageCaptureL(). NULL if there was an error.
		@param  aError
				KErrNone on success or an error code on failure.
		*/
		void ImageReady(CFbsBitmap* aBitmap,HBufC8* aData,TInt aError);

		/** Passes a filled frame buffer to the client.

		Called asynchronously, when a buffer has been filled with the required number
		of video frames by CCamera::StartVideoCapture().

		@param  aFrameBuffer
				On return, a pointer to an MFrameBuffer if successful,
				or NULL if not successful.
		@param  aError
				KErrNone if successful, or an error code if not successful.
		*/
		void FrameBufferReady(MFrameBuffer* aFrameBuffer,TInt aError);
        	
    
    private: // Functions from base CActive classes  
        /** 
         * From CActive cancels the Active object, empty  
         * @return void 
         */ 
        void DoCancel() ; 
 
        /** 
         * From CActive, called when an asynchronous request has completed 
         * @return void 
         */ 
        void RunL() ; 

        void GetNextUsableFileName();
        
        char* GetNewFileName();
        
    private: //data  
        CEikonEnv* iEikEnv; // Not owned by this class 
        CCamera* iCamera; 
        TDisplayMode iDisplayMode; 
        CCamera::TFormat iColorDepth; 
        CCamera::TFormat iColorDepthHW; 
        TCameraInfo iInfo; 
        TInt iZoomFactor; 
        TBool iCapturePrepared;  
        CWsBitmap* iBitmapPortraitVF;  
        CFbsBitmap* iBitmapSave;
        CFbsBitmap* iBitmapSnap;
        CImageEncoder* iEncoder; 
        TBool iStart; 
        TBool iCameraReserved; 
        TBool iCameraReserveComplete; 
        TSize iLandscapeSize; 
        TSize iPortraitSize; 
        TCameraState iMode; 
        TBool iPowering; 
        TEngineState iState;
        TFileName iNewFileName;
        HBufC* iImagePath;
        
        CRhodesAppView* iAppView;
    }; 
#endif // CRHOCAMERA_H
