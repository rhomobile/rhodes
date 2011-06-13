/*
 ============================================================================
 Name		: LogOptionsDialog.h
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
 Description : CLogOptionsDialog declaration
 ============================================================================
 */

#ifndef LOGOPTIONSDIALOG_H
#define LOGOPTIONSDIALOG_H

// INCLUDES
#include <e32std.h>
#include <e32base.h>
#include <akndialog.h>
#include <aknform.h>
#include <rhodes.rsg>

// CLASS DECLARATION

/**
 *  CLogOptionsDialog
 * 
 */
class CLogOptionsDialog : public CAknForm
	{
public:     // construction
    static TInt RunDlgLD( TInt aResouceId = R_LOGGING_OPTIONS_DIALOG);
    
    CLogOptionsDialog();
private:    // construction
    virtual ~CLogOptionsDialog();
    
    void setDialogItem( TInt aControlId, const char* text );
    void setHeaderItem( TInt aControlId, const char* text );

    char* getDialogItemText( TInt aControlId );
    
private:    // from CAknDialog
    void PreLayoutDynInitL(); // initialize controls
    TBool OkToExitL( TInt aButtonId ); // exit condition
	};

#endif // LOGOPTIONSDIALOG_H
