#include "StdAfx.h"

//-----------------------------------------------------------------------------

CExpat::CExpat()
	{
	// Open and set up expat to parse the xml
	P = XML_ParserCreate(NULL);
	}

//-----------------------------------------------------------------------------

CExpat::~CExpat()
	{
	XML_ParserFree(P);
	}

//-----------------------------------------------------------------------------

void CExpat::SetElementHandler(XML_StartElementHandler start, XML_EndElementHandler end)
	{
	XML_SetElementHandler(P, start, end);
	}

//-----------------------------------------------------------------------------

int CExpat::Parse(const char *s, int len, int is_final)
	{
	return XML_Parse(P, s, len, is_final);
	}

//-----------------------------------------------------------------------------

enum XML_Error CExpat::GetErrorCode(void)
	{
	return XML_GetErrorCode(P);
	}

//-----------------------------------------------------------------------------

const XML_LChar *CExpat::ErrorString(enum XML_Error error)
	{
	return XML_ErrorString(error);
	}

//-----------------------------------------------------------------------------

void CExpat::SetUserData(void *user_data)
	{
	XML_SetUserData(P, user_data);
	}
