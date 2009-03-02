/*
** 2004 May 22
**
** The author disclaims copyright to this source code.  In place of
** a legal notice, here is a blessing:
**
**    May you do good and not evil.
**    May you find forgiveness for yourself and forgive others.
**    May you share freely, never taking more than you give.
**
******************************************************************************
**
** Functions wrapping 'int64' and 'double' functions to pass values by reference.
*/

#include <stdlib.h>
#include "sqlite3.h"
#include <string.h>

EXPORT_C int sqlite3_bind_double_ref(sqlite3_stmt *stmt, int iCol, double *val)
{
	return sqlite3_bind_double(stmt,iCol,*val);
}

EXPORT_C int sqlite3_bind_int64_ref(sqlite3_stmt *stmt, int iCol, sqlite_int64 *val)
{
	return sqlite3_bind_int64(stmt,iCol,*val);
}

EXPORT_C void sqlite3_column_double_ref(sqlite3_stmt *stmt, int iCol, double *val)
{
	*val = sqlite3_column_double(stmt,iCol);
}

EXPORT_C void sqlite3_column_int64_ref(sqlite3_stmt *stmt, int iCol, sqlite_int64 *val)
{
	*val = sqlite3_column_int64(stmt,iCol);
}

EXPORT_C unsigned int sqlite3_strlen(char *ptr)
{
	return strlen(ptr);
}
