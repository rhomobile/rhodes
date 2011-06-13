/*
 ============================================================================
 Name		: Phonebook.h
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
 Description : CPhonebook declaration
 ============================================================================
 */

#ifndef PHONEBOOK_H
#define PHONEBOOK_H

// INCLUDES
#include <e32std.h>
#include <e32base.h>

#include "rhoruby/rhoruby.h"
//#include "logging/RhoLog.h"

#include <cntitem.h>

class CContactDatabase;
class CPbkContactItem;
class CContactItem;

// CLASS DECLARATION

/**
 *  CPhonebook
 * 
 */
class CPhonebook : public CBase
	{
    //DEFINE_LOGCLASS;
    
public:
	// Constructors and destructor

	/**
	 * Destructor.
	 */
	~CPhonebook();

	/**
	 * Two-phased constructor.
	 */
	static CPhonebook* NewL();

	/**
	 * Two-phased constructor.
	 */
	static CPhonebook* NewLC();

public:
	
	/**
	 * Get all phonebook records
	 * The caller take ownership of the object.
	 */
	VALUE getallPhonebookRecords();
	
	/**
	 * Open contact
	 * The caller take ownership of the object.
	 */
	CContactItem* openContact(char* id);
	
	/**
	 * Get contact
	 * The caller take ownership of the object.
	 */
	VALUE getContact(char* id);
	
	/**
	 * Commit contact
	 */
	void saveContact(CContactItem* contactItem);
	
	/**
	 * Delete contact
	 */
	void deleteContact(CContactItem* contactItem);
	
	/**
	 * Create contact
	 * The caller take ownership of the object.
	 */
	static CContactCard* createRecord();
	
	/**
	 * Set contact's field value 
	 */
	static void setRecordValue(CContactItem* contactItem, char* prop, char* value);
	
	/**
	 * Add record
	 */
	void addRecord( CContactCard* card );
	
private:

	/**
	 * Constructor for performing 1st stage construction
	 */
	CPhonebook();

	/**
	 * EPOC default constructor for performing 2nd stage construction
	 */
	void ConstructL();

	/**
	 * Auxilary functions
	 */

	/**
	 * Convert descriptor to c-style string. 
	 * The caller take ownership of the allocated memory.
	 */
	char* descriptorToStringL(const TDesC& aDescriptor);
	
	/**
	 * Insert descriptor value as string to the hash
	 */
	void add2hash(VALUE* hash, const char* key, TPtrC& aValue );
	
	/**
	 * Insert phonebook fields into the hash
	 */
	VALUE getFields(CContactItemFieldSet& fieldSet, char* id);
	
	static TUid getFieldId( char* prop );
	
private: //data
	CContactDatabase* iContactDb;
	};

#endif // PHONEBOOK_H
