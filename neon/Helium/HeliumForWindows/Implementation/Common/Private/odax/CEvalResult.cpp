#include "StdAfx.h"

//-----------------------------------------------------------------------------

// Copy constructor
CEvaluateResult::CEvaluateResult(CEvaluateResult& rhs)
	{
	Type = rhs.Type;

	switch(Type)
		{
		case STR_VALUE:
			// Allocate new memory and copy in contents
			Data.Str = new char[strlen(rhs.Data.Str) + 1];
			strcpy(Data.Str, rhs.Data.Str);
			break;

		case NUM_VALUE:
			Data.Num = rhs.Data.Num;
			break;
		}
	}

//-----------------------------------------------------------------------------

CEvaluateResult::CEvaluateResult()
	{
	Type = 0;
	// We are zeroing the same bit of memory here but it makes sure that
	// it is all zero, as we can't exactly know how big they are.
	Data.Num = 0;
	Data.Str = NULL;
	}

//-----------------------------------------------------------------------------

CEvaluateResult::CEvaluateResult(int num)
	{
	Type = NUM_VALUE;
	Data.Num = num;
	}

//-----------------------------------------------------------------------------

CEvaluateResult::CEvaluateResult(char *str)
	{
	Type = STR_VALUE;

	Data.Str = new char[strlen(str) + 1];
	strcpy(Data.Str, str);
	}

//-----------------------------------------------------------------------------

CEvaluateResult::~CEvaluateResult()
	{
	char *s = Data.Str;

	if ((Data.Str != NULL) && (Type == STR_VALUE)) 
		{
		delete []Data.Str;
		Data.Str = NULL;
		}
	}

//-----------------------------------------------------------------------------

CEvaluateResult &CEvaluateResult::operator = (CEvaluateResult &copy)
	{
	char *tmpstr;

	switch(copy.Type)
		{
		case NUM_VALUE:
			Type = NUM_VALUE;
			Data.Num = copy.Data.Num;
			break;

		case STR_VALUE:
			if ((Data.Str != NULL) && (Type == STR_VALUE)) 
				{
				delete Data.Str;
				Data.Str = NULL;
				}

			Type = STR_VALUE;
			
			// Make some memory to hold the copied string
			tmpstr = new char[strlen(copy.Data.Str) + 1];
			// Copy in string
			strcpy(tmpstr, copy.Data.Str);
			// Set our result pointer to it
			Data.Str = tmpstr;
			break;

//		default:
//			throw Exception(E_INVALID_RET_E, "Invalid return type.");
		}

	return *this;
	}

//-----------------------------------------------------------------------------

CEvaluateResult CEvaluateResult::operator && (CEvaluateResult rhs)
	{
	CEvaluateResult result_out;

	// Can't 'and' strings
//	if ((rhs.Type == STR_VALUE) || (Type == STR_VALUE))
//		throw Exception(E_AND_STRING, "Cannot logical 'and' strings.");

	// Switch anyway
	switch(Type)
		{
		case NUM_VALUE:
			result_out.Type = NUM_VALUE;
			result_out.Data.Num = Data.Num && rhs.Data.Num;
			break;
		}

	return result_out;
	}

//-----------------------------------------------------------------------------

CEvaluateResult CEvaluateResult::operator || (CEvaluateResult rhs)
	{
	CEvaluateResult result_out;

	// Can't 'or' strings
//	if ((rhs.Type == STR_VALUE) || (Type == STR_VALUE))
//		throw Exception(E_OR_STRING, "Cannot logical 'or' strings.");

	// Switch anyway
	switch(Type)
		{
		case NUM_VALUE:
			result_out.Type = NUM_VALUE;
			result_out.Data.Num = Data.Num || rhs.Data.Num;
			break;
		}

	return result_out;
	}

//-----------------------------------------------------------------------------

CEvaluateResult CEvaluateResult::operator == (CEvaluateResult rhs)
	{
	CEvaluateResult result_out;

//	if (((rhs.Type == STR_VALUE) && (Type == NUM_VALUE)) ||
//		((rhs.Type == NUM_VALUE) && (Type == STR_VALUE)))
//		throw Exception(E_EQ_TYPE, "Cannot apply equality operator to different types.");

	switch(Type)
		{
		case NUM_VALUE:
			result_out.Type = NUM_VALUE;
			result_out.Data.Num = Data.Num == rhs.Data.Num;
			break;

		case STR_VALUE:
			result_out.Type = NUM_VALUE;
			result_out.Data.Num = !(strcmp(Data.Str, rhs.Data.Str));
			break;
		}

	return result_out;
	}

//-----------------------------------------------------------------------------
