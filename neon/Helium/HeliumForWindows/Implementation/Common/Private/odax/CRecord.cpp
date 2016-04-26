#include "StdAfx.h"

//-----------------------------------------------------------------------------

CField::CField(const char *name, const char *value)
	{
	Name.SetStr(name);
	Value.SetStr(value);
	}

//-----------------------------------------------------------------------------
CRecord::CRecord(const char *name, const char *value)
	{
	AddField(name, value);
	}

//-----------------------------------------------------------------------------

CRecord::~CRecord()
	{
	Clear();
	}

//-----------------------------------------------------------------------------

void CRecord::AddField(const char *name, const char *value)
	{
	Fields.Add(new CField(name, value));
	}

//-----------------------------------------------------------------------------

void CRecord::Clear(void)
	{
	while (Fields.Count())
		Fields.Remove(0);
	}
