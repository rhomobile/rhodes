/**
 * \file emml.h
 * \brief Implimentation of the EMML 1.1 parser utilizing Regular Expressions for backward compatibility to EMML 1.0.
 * To be implimented by the Browser core and used to parse meta tags and the result of which to be passed on to
 * the plug-ins.
 */


#pragma once
#include "windows.h"

/**
 * Structure to hold name/value/module values
 * one per meta tag parameter or method
 */
typedef struct pbNameValue
{
	LPWSTR szModule;			///< string array for the module
	LPWSTR szName;				///< string array for the name of the pair
	LPWSTR szValue;				///< string array for the value
}	* LPPBNAMEVALUE;

/**
 * Structure to hold the search and replace patterns
 * from for a regular expression
 */
typedef struct pbRegExPair
{
	LPWSTR szSearchPattern;		///< string array pattern to search for
	LPWSTR szReplacePattern;	///< string array pattern to replace
	pbRegExPair* lpNextNode;	///< next node in the link list
}	* LPPBREGEXPAIR;


/**
 * Definition of a EMML callback function 
 * This function will be called one for each parsed meta function
 */
typedef LRESULT (CALLBACK* EMMLPROC) (LPPBNAMEVALUE);
typedef LRESULT (CALLBACK* EMMLPROFILEPROC) (LPPBNAMEVALUE, LPARAM);

#ifndef MAXURL
	#define MAXURL 2049 ///< The maximum allowable length of a URL in Internet Explorer		
#endif

#ifndef dim
	/**
	 * Macro to return the number of elements in an array
	 */
	#define dim(x) (sizeof(x) /sizeof(x[0]))
#endif

/**
 * Main EMML Parser Class 
 * An instance of this class should be created by anything that needs to parse
 * an EMML tag.
 */
class CEMML
{
public:
	CEMML(BOOL bRegex);
	~CEMML();

	static const DWORD CEMML::PET_SUCCESS = 0;						///< Parse EMML Tag return code - success
	static const DWORD CEMML::PET_NOTHINGTODO = -1;					///< Parse EMML Tag return code - warning, nothing to do!

	static const DWORD CEMML::EP_SUCCESS = 0;						///< EMML Profile return code - success
	static const DWORD CEMML::EP_NOTFOUND = -1;						///< EMML Profile return code - no profile found
	
	static const DWORD CEMML::EP_LOOKINGFORSTARTOFNAME = 0;			///< EMML Profile process state - looking for the start of the name
	static const DWORD CEMML::EP_LOOKINGFORENDOFNAME = 1;			///< EMML Profile process state - looking for the end of the name
	static const DWORD CEMML::EP_LOOKINGFORENDOFDATASECTION = 2;	///< EMML Profile process state - looking for the end of the data section
		

	protected:

	/**
	 * Name Value Pair Container Class 
	 * Class used for creating a link list of name value pairs.
	 */
	class CNameValuePair
	{
	public:
		CNameValuePair(void);
		~CNameValuePair(void);

		/**
		 * Function to create a copy of the name in 'this' object
		 * \param sName name to be copied
		 * \param iLen length of name
		 */
		void AddName(LPCWSTR sName, int iLen);
		
		/**
		 * Function to create a copy of the name in 'this' object
		 * \param sName name to be copied
		 * \param iLen length of name
		 */
		void AddDefaultMetaTagName(LPCWSTR sName, int iLen);
		
		/**
		 * Function to create a copy of the value in 'this' object
		 * \param sValue value to be copied
		 * \param iLen length of value
		 */
		void AddValue(LPCWSTR sValue, int iLen);
		
		/**
		 * Function to create a copy of the module in 'this' object
		 * \param sModule module name to be copied
		 * \param iLen length of module name
		 */
		void AddModule(LPCWSTR sModule, int iLen);

		/**
		 * Returns a pointer to the name value pair structure
		 * \return pointer the the structure containing the nave value pair data
		 */
		LPPBNAMEVALUE GetNVPPointer();

		/**
		 * Returns a pointer to the name value pair structure
		 * after swapping round the name and value if the name is null
		 * or an empty string.
		 * \return pointer the the structure containing the nave value pair data
		 */
		LPPBNAMEVALUE MakeMethod();

		CNameValuePair* m_pnvpNextEl;	///< pointer to the next element

	protected:
		/**
		 * Private function which checks if the passed char is
		 * valid or if it should be ignored.  Returns TRUE if
		 * it should be ignored.
		 * \param tcCharToCheck the character to check
		 * \return TRUE skip this character
		 * \return FALSE do not skip this character
		 */
		BOOL SkipChar(TCHAR tcCharToCheck);

		pbNameValue m_pbNVP; ///< sturcture to hold the name value pair data
		
	};

	/**
	 * EMML Profile Container Class 
	 * Class used for creating a binary tree list of named emml profiles.
	 * EMML Profiles are a named list of many EMML tags stored as name value pairs in a link list
	 */
	class CEMMLProfile
	{
	public:
		
		CEMMLProfile(void);
		~CEMMLProfile(void);
		
		CEMMLProfile* m_pepLeftEl;	///< pointer to the left branch
		CEMMLProfile* m_pepRightEl;	///< pointer to the right branch

		LPWSTR m_szProfileName; ///< pointer the the profile name

		CNameValuePair* m_pnvpHeadEl; ///< pointer to the head of the name vale pair list

		/**
		* Adds the profile name to the profile class so that 
		* can be used to find the profile at a later time
		* \param sName name of the profile
		* \param iLen length of sName
		*/
		void AddProfileName(LPCWSTR sName, int iLen);

		/**
		 * Private function which checks if the passed char is
		 * valid or if it should be ignored.  Returns TRUE if
		 * it should be ignored.
		 * \param tcCharToCheck the character to check
		 * \return TRUE skip this character
		 * \return FALSE do not skip this character
		 */
		static BOOL SkipChar(TCHAR tcCharToCheck);

		/**
		 * Creates a new EMML Profile Node 
		 * \param pepNode pointer to the head of the list
		 * \param sName name of the new node
		 * \param iLen length of sName
		 * \return pointer to the new node
		 */
		static CEMMLProfile * CreateNewEMMLProfileNode(CEMMLProfile * pepNode, LPCWSTR sName, int iLen);

		/**
		 * Finds an EMML Profile Node 
		 * \param pepNode pointer to the head of the list
		 * \param sName name of the new node
		 * \return pointer to the matching new
		 * \return NULL could not find node
		 */
		static CEMMLProfile * FindEMMLProfileNode(CEMMLProfile * pepNode, LPCWSTR sName);
	};

	/**
	 * Named EMML Profile List Container Class 
	 * Class used for storing a list of named EMML Profiles Lists.
	 * Named EMML Profiles are where you name a profile page so that 
	 * it can be accessed accross several pages and not destoryed 
	 * on each page load.
	 */
	class CNamedProfileList
	{
	public:
		CNamedProfileList(void);
		~CNamedProfileList(void);

		/**
		 * Function to create a copy of the name in 'this' object
		 * \param sName name to be copied
		 * \param iLen length of name
		 */
		void AddName(LPCWSTR sName, int iLen);
		
		/**
		 * Function to add the profile list (list of lists of emml tags)
		 * \param pepHead pointer to the head of the profile list
		 */
		void AddProfileTree(CEMMLProfile* pepHead);

		/**
		 * Function to return the pointer the EMML Profile List
		 * \return pointer the EMML Profile List
		 */
		CEMMLProfile* GetEPPointer();

		/**
		 * Creates a new EMML Profile List Node.  The new node is 
		 * named with the value of sName. 
		 * \param pnplNode pointer the the head of the named profile list
		 * \param szName Name of the EMML Profile List
		 * \return pointer to the new node.
		 */
		static CNamedProfileList * CreateNewEMMLProfileListNode(CNamedProfileList * pnplNode, LPCWSTR szName);

		/**
		 * Finds an EMML Profile List Node
		 * \param pnplNode pointer the the head of the named profile list
		 * \param szName Name of the EMML Profile List to find
		 * \return NULL is returned if no match is found
		 * \return pointer to the new node
		 */
		static CNamedProfileList * FindEMMLProfileListNode(CNamedProfileList * pnplNode, LPCWSTR szName);
		
		/**
		 * Deletes an EMML Profile List Node
		 * \param pnplNode pointer the the head of the named profile list
		 * \param szName Name of the EMML Profile List to delete
		 * \return NULL is returned if the entry is delete
		 * \return pointer to the head if the node to delete is not found
		 */
		static CNamedProfileList * DeleteEMMLProfileListNode(CEMML::CNamedProfileList * pnplNode, LPCWSTR szName);

		/**
		 * Private function which checks if the passed char is
		 * valid or if it should be ignored.  Returns TRUE if
		 * it should be ignored.
		 * \param tcCharToCheck the character to check
		 * \return TRUE skip this character
		 * \return FALSE do not skip this character
		 */
		static BOOL SkipChar(TCHAR tcCharToCheck);

	protected:
		CEMMLProfile* m_pProfileList;	///< pointer to the next element

		LPWSTR m_sNamedProfilesName; ///< Name of the Profile List

		CNamedProfileList* m_pnplPrevEl;	///< pointer to the prev element
		CNamedProfileList* m_pnplNextEl;	///< pointer to the next element
		
	};

public:
	/**
	 * Parses the http-equiv and contents/name parameters into the component parts;
	 * method, parameter and value.
	 * \param szHttpEquiv the http-equiv string to parse
	 * \param szContent the content or name string to parse
	 * \param lpEMMLProc the function to be called for each EMML tag parsed out of the http-equiv and content strings
	 * \return PET_NOTHINGTODO there was nothing to do
	 * \return PET_SUCCESS function complete successfully
	 * \return PET_FAILED not implimented
	 */
	DWORD ParseEMMLTag(LPCWSTR szHttpEquiv, LPCWSTR szContents, EMMLPROC lpEMMLProc);

	/**
	 * Finds an EMML Profile and issues each emml tag contained in the profile
	 * \param szProfileName name of the profile to find and issue
	 * \param lpEMMLProc the function to be called for each EMML tag parsed out of the http-equiv and content strings
	 * \param lParam void pointer to be passed to the EMMLPROC
	 * \return EP_NOTFOUND the profile was not found
	 * \return EP_SUCCESS function complete successfully
	 */
	DWORD ApplyEMMLProfile(LPCWSTR szProfileName, EMMLPROFILEPROC lpEMMLProc, LPARAM lParam);

	/**
	 * Imports an EMML Profile Page.  The profile is loaded
	 * from the local file system.  If szNamedProfileListName is 
	 * NULL, the profiles are stored on the temporary list
	 * and the previous entries are deleted.  If a name is
	 * supplied, a new NamedProfile node is created and the
	 * imported profiles are stored at that node.  If a 
	 * named profile list already exists for that name, the
	 * list is deleted and replaced with the new list.
	 * \param szProfileFileName location of the profile page in the local file system
	 * \param szNamedProfileListName (= NULL) optional name of the profile
	 * \return NULL the function failed
	 * \return number of profiles parsed
	 */
	DWORD ImportEMMLProfilePage(LPCWSTR szProfileFileName, LPCWSTR szNamedProfileListName = NULL);

	/**
	 * Deletes an EMML profile list.  If a name is supplied
	 * it finds the named list and deletes it.  else
	 * it deletes the temp list.
	 * \param szNamedProfileListName (= NULL) optional name of the profile
	 */
	void DeleteEMMLProfilePage(LPCWSTR szNamedProfileListName = NULL);

	/**
	 * Adds a Regular Expression search and replace string 
	 * for the httpEquiv strings
	 * \param lpcSearchPattern the search pattern
	 * \param lpcReplacePattern the replace pattern
	 * \return TRUE the function succeeded
	 * \return FALSE the function failed
	 */
	BOOL AddRegExPair_HE(LPCWSTR lpcSearchPattern, LPCWSTR lpcReplacePattern);

	/**
	 * Adds a Regular Expression search and replace string 
	 * for the contents strings
	 * \param lpcSearchPattern the search pattern
	 * \param lpcReplacePattern the replace pattern
	 * \return TRUE the function succeeded
	 * \return FALSE the function failed
	 */
	BOOL AddRegExPair_CT(LPCWSTR lpcSearchPattern, LPCWSTR lpcReplacePattern);
	
	DWORD AddStaticTag(LPCWSTR szHttpEquiv, LPCWSTR szContents);


	DWORD RunDefaultTags(EMMLPROC lpEMMLProc);

private:

	/**
	 * Parses EMML Profile Data
	 * \param szProfileData the profile data to parse
	 * \param iLen the length of the data
	 * \return NULL the function failed
	 * \return pointer to a namedvaluepair link list
	 */
	CEMML::CNameValuePair* ParseEMMLProfileData(LPCWSTR szProfileData, int iLen);
	
	/**
	 * Adds a Regular Expression search and replace string 
	 * \param RegExCurNode regular expression node
	 * \param lpcSearchPattern the search pattern
	 * \param lpcReplacePattern the replace pattern
	 * \return TRUE the function succeeded
	 * \return FALSE the function failed
	 */
	BOOL AddRegExPair(pbRegExPair *** RegExCurNode, LPCWSTR lpcSearchPattern, LPCWSTR lpcReplacePattern);
	DWORD ExecMetaTags(CNameValuePair *pnvpCurr,EMMLPROC lpEMMLProc);

	CEMMLProfile * m_pEMMLProfiles;				///< The temporary profile lists (deleted on each page load)
	CNamedProfileList * m_pNamedEMMLPLHead;		///< the head of the named profile lists
	CNameValuePair * m_DefaultTags;				///< the list of default tags
	CNameValuePair * m_DefaultCurrent;					///< the current default tag

	pbRegExPair * m_RegExPatternsHE;			///< the head of the http-equiv regular expression pattern list
	pbRegExPair * m_RegExPatternsCT;			///< the head of the contents regular expression pattern list

	pbRegExPair ** m_RegExCurNodeHE;			///< the tail of the http-equiv regular expression pattern list (without this it will take longer each new pattern to add)
	pbRegExPair ** m_RegExCurNodeCT;			///< the tail of the contents regular expression pattern list (without this it will take longer each new pattern to add)

public:
	BOOL m_bDoRegEx;							///< if TRUE (default) regular expressions will be processed for each EMML tag
};
