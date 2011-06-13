/*
 ============================================================================
 Name		: LogOptionsDialog.cpp
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
 Description : CLogOptionsDialog implementation
 ============================================================================
 */

#include "LogOptionsDialog.h"
#include <eikedwin.h>

#include "rhodes.hlp.hrh"
#include "rhodes.hrh"
#include "rhodes.pan"
#include "avkon.hrh"
#include <AknPopupHeadingPane.h>
#include "eiktxlbx.h"

#include "logging/RhoLog.h"

CLogOptionsDialog::CLogOptionsDialog()
	{
	}

CLogOptionsDialog::~CLogOptionsDialog()
	{
	
	}

TInt CLogOptionsDialog::RunDlgLD( TInt aResouceId )
    {
    CLogOptionsDialog* dlg = new (ELeave) CLogOptionsDialog();
    dlg->PrepareLC( R_LOGGING_OPTIONS_DIALOG );
    return dlg->RunLD();
    }

void CLogOptionsDialog::PreLayoutDynInitL()
	{
		setDialogItem(EMessageClassesId, LOGCONF().getEnabledCategories().c_str());
		setDialogItem(EExcludeMessageClassesId, LOGCONF().getDisabledCategories().c_str());
		
		// Get a pointer to the control.
		CEikTextListBox* myComboBox= STATIC_CAST(CEikTextListBox*, ControlOrNull(ELogLevelChoiceListId));
		if ( myComboBox )
			{
			TInt severity = LOGCONF().getMinSeverity();
			if ( severity < 0 || severity > 3 )
				severity = 0;
			
			myComboBox->View()->SelectItemL( severity );
			}

		TSize size;
		size.iHeight = 150;
		size.iWidth = 240;
		
		CAknDialog::SetSize(size);
		
		CAknDialog::DrawNow();
		CAknDialog::SizeChanged();
		
		TPoint pos;
		pos.iX = 0;
		pos.iY = 293 - 150;
		
		CAknDialog::SetPosition(pos);
		
		CAknDialog::ActivateL();		
						
				
    }

void CLogOptionsDialog::setDialogItem( TInt aControlId, const char* text )
	{
		if ( text )
			{
			TPtrC8 ptr(reinterpret_cast<const TUint8*>(text));
			HBufC* buffer = HBufC::NewL(ptr.Length());
			buffer->Des().Copy(ptr);
			
			TPtrC aDescriptor;
			aDescriptor.Set(*buffer);
					
			STATIC_CAST(CEikEdwin*,Control(aControlId))->SetTextL(&aDescriptor);
			delete buffer;
			}
	}

void CLogOptionsDialog::setHeaderItem( TInt aControlId, const char* text )
	{
		if ( text )
			{
			TPtrC8 ptr(reinterpret_cast<const TUint8*>(text));
			HBufC* buffer = HBufC::NewL(ptr.Length());
			buffer->Des().Copy(ptr);
			
			TPtrC aDescriptor;
			aDescriptor.Set(*buffer);
					
			STATIC_CAST(CAknPopupHeadingPane*,Control(aControlId))->SetTextL(aDescriptor);
			delete buffer;
			}
	}

TBool CLogOptionsDialog::OkToExitL( TInt aButtonId )
    {
    if ( EAknSoftkeyOk == aButtonId )
        {
        CEikTextListBox* myComboBox= STATIC_CAST(CEikTextListBox*, ControlOrNull(ELogLevelChoiceListId));
        if ( myComboBox )
        	{
				for(TInt i = myComboBox->View()->TopItemIndex(); i <=  myComboBox->View()->BottomItemIndex(); i++)
				  {
					 if( myComboBox->View()->ItemIsSelected(i))
					 {
						 LOGCONF().setMinSeverity(i);
					 }
				  }
        	}
        
        char* str = getDialogItemText(EMessageClassesId);
        if ( str )
        	{
			LOGCONF().setEnabledCategories(str);
			free(str);
        	}
        
		str = getDialogItemText(EExcludeMessageClassesId);
		if ( str )
			{
			LOGCONF().setDisabledCategories(str);
			free(str);
			}
		
		LOGCONF().saveToFile();
		
        }
    return ETrue;
    }

char* CLogOptionsDialog::getDialogItemText( TInt aControlId )
	{
		char* str = NULL;
		HBufC* pBuf = STATIC_CAST(CEikEdwin*,Control(aControlId))->GetTextInHBufL();
		if ( pBuf )
    	{
			TPtrC aDescriptor;
			aDescriptor.Set(*pBuf);
			
			TInt length = aDescriptor.Length();
		 
			HBufC8* buffer = HBufC8::NewLC(length);
			buffer->Des().Copy(aDescriptor);
		 
			str = (char*)malloc(length + 1);
			Mem::Copy(str, buffer->Ptr(), length);
			str[length] = '\0';
		 
			CleanupStack::PopAndDestroy(buffer);
    			
    	}

		return str;
	}
