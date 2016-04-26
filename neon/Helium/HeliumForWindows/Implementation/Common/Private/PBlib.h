/**
 * \file PBlib.h
 * \brief Library file containing global functions used by Browser. 
 */
#ifndef _PBLIB_H_
#define _PBLIB_H_

#include <windows.h>

//  From defines.h
/**
 * The maximum allowable length of a URL
 */
#define MAXURL 2049				//maximum characters in a URL
#define MAX_URL MAXURL


/**
 * Function to replace insertion specifiers with passed strings. 
 * This function mimics the behaviour of wsprintf by replacing instances
 * %%s and %[number] with the passed optional parameters, which are assumed
 * to be TCHARs.
 * e.g. PBformat(0, 1, , destString, "%2%1", "world", "hello ") will populate destString with "hello world"
 * \param bEscapeOutput Whether the output string should be 'escaped', i.e. all characters output as the string '%' followed by the Hex equivilent of their ASCII characters.  This allows barcodes containing character sequences such as '...%de...' to be processed correctly.
 * \param iNoParams The number of optional parameters passed to the function
 * \param Destination An out parameter where the transformed string will be copied to
 * \param Format The input string to be parsed
 * \author Darryn Campbell
 * \date July 2008
 */
void PBformat(BOOL bEscapeOutput, int iNoParams, TCHAR *Destination, TCHAR *Format, ...);


/**
 * Function to replace insertion specifiers with passed strings.
 * This function mimics the behaviour of wsprintf by replacing instances
 * %%s and %[number] with the passed optional parameters, which are assumed
 * to be TCHARs
 * \param bEscapeOutput Whether the output string should be 'escaped', i.e. all characters output as the string '%' followed by the Hex equivilent of their ASCII characters.  This allows barcodes containing character sequences such as '...%de...' to be processed correctly.
 * \param iNoParams The number of parameters in the 'parameterList' TCHAR**
 * \param Destination An out parameter where the transformed string will be copied to
 * \param Format The input string to be parsed
 * \param parameterList Pointer to array of strings which comprise the substitution values
 * \author Darryn Campbell
 * \date August 2008
 */
void PBformat(BOOL bEscapeOutput, int iNoParams, TCHAR *Destination, TCHAR *Format, TCHAR** parameterList);

/**
 * Function to compare TCHARs - case insensitive
 * \param tc1 First TCHAR*
 * \param tc2 Second TCHAR*
 * \return TRUE if the two strings are identical ignoring case
 * \author James Morley-Smith
 * \date December 2004
 */
BOOL cmp(LPCTSTR tc1, LPCTSTR tc2);

/**
 * Function to compare TCHARs for a specified number of characters - case insensitive
 * \param tc1 First TCHAR*
 * \param tc2 Second TCHAR*
 * \param iChars Number of characters to compare (From start of string)
 * \return TRUE if the two strings are identical for the first iChars characters, ignoring case.
 * \author James Morley-Smith
 * \date December 2004
 */
BOOL cmp(LPCTSTR tc1, LPCTSTR tc2, int iChars);

/**
 * Function to convert the given string to its equivilent in Hex ASCII.  Upto a max length of MAXURL
 * \param stringToEscape The string to be converted into Hex ASCII.  
 * e.g. Escape('hi world'); will convert the passed string to '%%68%%69%%20%%77%%6f%%72%%69%%64'
 * \author Darryn Campbell
 * date\ July 2008
 */
void Escape(TCHAR* stringToEscape);

#endif // #ifndef _PBLIB_H_