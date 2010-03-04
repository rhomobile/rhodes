/*
 ============================================================================
 Name		: Phonebook.cpp
 Author	  : Anton Antonov
 Version	 : 1.0
 Copyright   : Copyright (C) 2008 Rhomobile. All rights reserved.

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
 Description : CPhonebook implementation
 ============================================================================
 */

#include "Phonebook.h"

#include "ext\phonebook\phonebook.h"

#include <aknnotewrappers.h>
#include <cntdb.h>
#include <cntitem.h>
#include <cntfldst.h>

#include <cpbkcontactengine.h>
#include <cpbkcontactitem.h>
#include <stringloader.h>

#include "ContactsConstants.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <rhodes.rsg>

//IMPLEMENT_LOGCLASS(CPhonebook,"Phonebook");

CPhonebook::CPhonebook()
	{
	// No implementation required
	}

CPhonebook::~CPhonebook()
	{
		if ( iContactDb )
		{
			iContactDb->CloseTables();
	        delete iContactDb;
		}
	}

CPhonebook* CPhonebook::NewLC()
	{
	CPhonebook* self = new (ELeave)CPhonebook();
	CleanupStack::PushL(self);
	self->ConstructL();
	return self;
	}

CPhonebook* CPhonebook::NewL()
	{
	CPhonebook* self=CPhonebook::NewLC();
	CleanupStack::Pop(); // self;
	return self;
	}

void CPhonebook::ConstructL()
	{
		// Read name of the default database
		_LIT( KOrgContactFile,"" );
		
		TBuf<KMaxDatabasePathAndNameLength> orgContactFile( KOrgContactFile );
		CContactDatabase::GetDefaultNameL( orgContactFile );
		orgContactFile.LowerCase();
	
		TRAPD(err, iContactDb = CContactDatabase::OpenL( orgContactFile ););
		// Check if database already exist
	    if ( err == KErrNotFound )
		{
		    HBufC* text = StringLoader::LoadLC( R_CONTACTS_DB_NOT_FOUND );
		    CAknInformationNote* informationNote = new (ELeave) CAknInformationNote;
		    informationNote->ExecuteLD( *text );
		    CleanupStack::PopAndDestroy( text );
		    iContactDb = NULL;
		}
	}

/**
 * Auxilary functions
 */

char* CPhonebook::descriptorToStringL(const TDesC& aDescriptor)
{
    TInt length = aDescriptor.Length();
 
    if ( length > 0 )
	{
	    HBufC8* buffer = HBufC8::NewLC(length);
	    buffer->Des().Copy(aDescriptor);
	 
	    char* str = (char*)malloc(length + 1);
	    Mem::Copy(str, buffer->Ptr(), length);
	    str[length] = '\0';
	 
	    CleanupStack::PopAndDestroy(buffer);
	 
	    return str;
	}
    return NULL;
}

void CPhonebook::add2hash(VALUE* hash, const char* key, TPtrC& aValue )
{
    char* value = descriptorToStringL(aValue);
        
    if ( key && value )
	{
		//LOG(INFO) + "Adding field [" + key + ":" + value + "]";
		addStrToHash(*hash, key, value, strlen(value));
    }
        
    if ( value )
    	free( value );
}

VALUE CPhonebook::getFields(CContactItemFieldSet& fieldSet, char* id) 
{
	VALUE hash = createHash();
	
	// Get field ID
	//LOG(INFO) + "Adding field [id:" + id + "]";
	addStrToHash(hash, RUBY_PB_ID, id, strlen(id));
	
	// Get first name
    TInt findpos( fieldSet.Find( KUidContactFieldGivenName ) );

    // Check that the first name field is actually there.
    if ( (findpos > -1) || (findpos >= fieldSet.Count()) )
    {
        CContactItemField& firstNameField = fieldSet[findpos];
        CContactTextField* firstName = firstNameField.TextStorage();
        TPtrC value = firstName->Text();
        add2hash(&hash, RUBY_PB_FIRST_NAME, value);
    }
    else
	{
		addStrToHash(hash, RUBY_PB_FIRST_NAME, "", strlen(""));
	}

    // Get last name
    findpos = fieldSet.Find( KUidContactFieldFamilyName );

    // Check that the last name field is actually there.
    if ( (findpos > -1) || (findpos >= fieldSet.Count()) )
    {
        CContactItemField& lastNameField = fieldSet[ findpos ];
        CContactTextField* lastName = lastNameField.TextStorage();
        TPtrC value = lastName->Text();
        add2hash(&hash, RUBY_PB_LAST_NAME, value);
    }
    else
	{
		addStrToHash(hash, RUBY_PB_LAST_NAME, "", strlen(""));
	}
    
    // Get home phone
    findpos = fieldSet.Find( KUidContactFieldPhoneNumber );

    // Check that the last name field is actually there.
    if ( (findpos > -1) || (findpos >= fieldSet.Count()) )
    {
        CContactItemField& itemField = fieldSet[ findpos ];
        CContactTextField* textField = itemField.TextStorage();
        TPtrC value = textField->Text();
        add2hash(&hash, RUBY_PB_HOME_NUMBER, value);
        add2hash(&hash, RUBY_PB_MOBILE_NUMBER, value);
        add2hash(&hash, RUBY_PB_BUSINESS_NUMBER, value);
    }
    else
	{
		addStrToHash(hash, RUBY_PB_HOME_NUMBER, "", strlen(""));
		addStrToHash(hash, RUBY_PB_MOBILE_NUMBER, "", strlen(""));
		addStrToHash(hash, RUBY_PB_BUSINESS_NUMBER, "", strlen(""));
	}
    
    // Get email
    findpos = fieldSet.Find( KUidContactFieldEMail );

    // Check that the last name field is actually there.
    if ( (findpos > -1) || (findpos >= fieldSet.Count()) )
    {
        CContactItemField& itemField = fieldSet[ findpos ];
        CContactTextField* textField = itemField.TextStorage();
        TPtrC value = textField->Text();
        add2hash(&hash, RUBY_PB_EMAIL_ADDRESS, value);
    }
    else
	{
		addStrToHash(hash, RUBY_PB_EMAIL_ADDRESS, "", strlen(""));
	}
    
    // Get company
    findpos = fieldSet.Find( KUidContactFieldCompanyName );

    // Check that the last name field is actually there.
    if ( (findpos > -1) || (findpos >= fieldSet.Count()) )
    {
        CContactItemField& itemField = fieldSet[ findpos ];
        CContactTextField* textField = itemField.TextStorage();
        TPtrC value = textField->Text();
        add2hash(&hash, RUBY_PB_COMPANY_NAME, value);
    }
    else
	{
		addStrToHash(hash, RUBY_PB_COMPANY_NAME, "", strlen(""));
	}
    
	return hash;
}

VALUE CPhonebook::getallPhonebookRecords() 
{
	VALUE hash = createHash(); //retval
	
	iContactDb->SetDbViewContactType( KUidContactCard );

    TFieldType aFieldType1( KUidContactFieldFamilyName );
    TFieldType aFieldType2( KUidContactFieldGivenName );
    CContactDatabase::TSortPref sortPref1( aFieldType1 );
    CContactDatabase::TSortPref sortPref2( aFieldType2 );

    // Sort contacts by Family and Given Name
    CArrayFixFlat<CContactDatabase::TSortPref>* aSortOrder =
                    new (ELeave) CArrayFixFlat<CContactDatabase::TSortPref>(2);

    CleanupStack::PushL( aSortOrder );
    aSortOrder->AppendL( sortPref1 );
    aSortOrder->AppendL( sortPref2 );

    // The database takes ownership of the sort order array passed in
    iContactDb->SortL( aSortOrder );

    // The caller does not take ownership of this object.
    // so do not push it onto the CleanupStack
    const CContactIdArray* contacts = iContactDb->SortedItemsL();

    // Go through each contact item and
    // make items for listbox
    const TInt nc( contacts->Count() );

    for ( TInt i = 0; i < nc; i++ ) //For each ContactId
    {
        CContactItem* contact = NULL;
        // The caller takes ownership of the returned object.
        // So push it onto the CleanupStack
        contact = iContactDb->OpenContactL( (*contacts)[i] );
        CleanupStack::PushL( contact );

        char rid[20] = {0};
        sprintf( rid, "{%d}", contact->Id());
        
        if (rid) 
    	{
			//LOG(INFO) + "Adding record " + rid;
			CContactItemFieldSet& fieldSet = contact->CardFields();
			addHashToHash(hash,rid,getFields(fieldSet, rid));
		}
        
        iContactDb->CloseContactL( contact->Id() );
        CleanupStack::PopAndDestroy( contact );
    }
    
    return hash;
}

CContactItem* CPhonebook::openContact(char* id)
{
	TInt nID = -1;
	sscanf( id, "{%d}", &nID );
	
	if ( nID >= 0 )
	{
		CContactItem* contact = NULL;
	    // The caller takes ownership of the returned object.
	    // So push it onto the CleanupStack
	    contact = iContactDb->OpenContactL( nID );
	    CleanupStack::PushL( contact );
	    
	    return contact;
	}
	return NULL;
}

VALUE CPhonebook::getContact(char* id)
{
	CContactItem* contact = openContact(id);
    if ( contact )
	{
        CContactItemFieldSet& fieldSet = contact->CardFields();
        VALUE hash = getFields(fieldSet, id);
        
        iContactDb->CloseContactL( contact->Id() );
        CleanupStack::PopAndDestroy( contact );
        
        return hash;
	}
    
    return rho_ruby_get_NIL();
}

CContactCard* CPhonebook::createRecord()
{
	CContactCard* card = CContactCard::NewLC();
	return card;
}

TUid CPhonebook::getFieldId( char* prop )
{
	TUid fieldID = {-1};
	
	if ( strcmp(RUBY_PB_FIRST_NAME, prop) == 0 )
	{
		fieldID = KUidContactFieldGivenName;
	}
	else if ( strcmp(RUBY_PB_LAST_NAME, prop) == 0 )
	{
		fieldID = KUidContactFieldFamilyName;
	}
	else if ( strcmp(RUBY_PB_MOBILE_NUMBER, prop) == 0 ||
			strcmp(RUBY_PB_HOME_NUMBER, prop) == 0 ||
			strcmp(RUBY_PB_BUSINESS_NUMBER, prop) == 0 )
	{
		fieldID = KUidContactFieldPhoneNumber;
	}
	else if ( strcmp(RUBY_PB_EMAIL_ADDRESS, prop) == 0 )
	{
		fieldID = KUidContactFieldEMail;
	}
	else if ( strcmp(RUBY_PB_COMPANY_NAME, prop) == 0 )
	{
		fieldID = KUidContactFieldCompanyName;
	}
	
	return fieldID;
}

void ConvertToUnicode(RFs& session, TDes16& aUnicode, const char *str)
{
  	CCnvCharacterSetConverter *converter = CCnvCharacterSetConverter::NewL();
  	converter->PrepareToConvertToOrFromL(KCharacterSetIdentifierUtf8, session);
  
  	TPtrC8 ptr((const unsigned char*)str);

  	int state = CCnvCharacterSetConverter::KStateDefault;
  	converter->ConvertToUnicode(aUnicode, ptr, state);
  	
  	delete converter;
}

void CPhonebook::setRecordValue(CContactItem* contactItem, char* prop, char* value)
{
	if ( contactItem && prop && value )
	{
		TUid fieldId = CPhonebook::getFieldId(prop);
		
		if ( fieldId.iUid >= 0 ) //if supported id
		{
			TInt phoneFieldIndex =	contactItem->CardFields().Find(fieldId);

			RFs iSession;
			User::LeaveIfError(iSession.Connect());

			TBuf<100> textValue;
			ConvertToUnicode( iSession, textValue, value );

			if(phoneFieldIndex == KErrNotFound)
			{
				// The contact has no existing phone field, so add one.
				CContactItemField* addPhoneField = CContactItemField::NewLC(KStorageTypeText, fieldId);
			
				addPhoneField->SetMapping(fieldId);
				addPhoneField->TextStorage()->SetTextL(textValue);
			
				contactItem->AddFieldL(*addPhoneField); // Takes ownership
			
				CleanupStack::Pop(addPhoneField);
			}
			else
			{
				// Modify the existing phone field’s contents.
				CContactItemField& phoneField = contactItem->CardFields()[phoneFieldIndex];
				phoneField.TextStorage()->SetTextL(textValue);
			}

			iSession.Close();
		}	
	}
}

void CPhonebook::addRecord( CContactCard* card )
{
	if ( card )
	{
		TContactItemId cardId = iContactDb->AddNewContactL(*card);
		delete card;
	}
}

void CPhonebook::saveContact(CContactItem* contactItem)
{
	if ( contactItem )
	{
		iContactDb->CommitContactL( *contactItem );
	    CleanupStack::PopAndDestroy( contactItem );
	}
}

void CPhonebook::deleteContact(CContactItem* contactItem)
{
	if ( contactItem )
	{
		TContactItemId id = contactItem->Id();
		iContactDb->CloseContactL( id );
		CleanupStack::PopAndDestroy( contactItem );
		iContactDb->DeleteContactL( id );
	}
}
