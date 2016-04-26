/**
*  \file URLDecomposition.h
*  \brief File to house functions to decompose a URL into its constituent 
*  elements.  For example ftp://username:password@hostname:port/some/path/here/page.html
*/

#include <windows.h>

/**
*  Retrieve the port from the specified URL
*  \param tcURL URL from which to extract the port
*  \param lpRetStr [out] Populated by this function this will be the string
*                        representation of the port
*  \return -1 Error parsing URL
*  \return 0 No port has been specified
*  \return Length of string representing URL
*/
int GetPortFromURL(LPCTSTR tcURL, LPTSTR lpRetStr);

/**
*  Retrieve the username from the specified URL
*  \param tcURL URL from which to extract the Username
*  \param lpRetStr [out] Populated by this function this will be the username
*  \return -1 Error parsing Username
*  \return 0 No username has been specified
*  \return Length of username
*/
int GetUsernameFromURL(LPCTSTR tcURL, LPTSTR lpRetStr);

/**
*  Retrieve the password from the specified URL
*  \param tcURL URL from which to extract the password
*  \param lpRetStr [out] Populated by this function this will be the password
*  \return -1 Error parsing password
*  \return 0 No password has been specified
*  \return Length of password
*/
int GetPasswordFromURL(LPCTSTR tcURL, LPTSTR lpRetStr);

/**
*  Retrieve the Server address (or hostname) from the specified URL
*  \param tcURL URL from which to extract the server
*  \param lpRetStr [out] Populated by this function this will be the server
*  \return -1 Error parsing server
*  \return 0 No server has been specified
*  \return Length of server
*/
int GetServerFromURL(LPCTSTR tcURL, LPTSTR lpRetStr);

/**
*  Retrieve the file name and path from the URL
*  \param tcURL URL from which to extract the filename and path
*  \param lpRetStr [out] Populated by this function this will be the file name
*                        and path.
*  \return -1 Error parsing file name and path
*  \return 0 No file name and path has been specified
*  \return Length of file name and path
*/
int GetFileAndPathFromURL(LPCTSTR tcURL, LPTSTR lpRetStr);