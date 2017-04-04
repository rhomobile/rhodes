/*------------------------------------------------------------------------
* (The MIT License)
* 
* Copyright (c) 2008-2011 Rhomobile, Inc.
* 
* Permission is hereby granted, free of charge, to any person obtaining a copy
* of this software and associated documentation files (the "Software"), to deal
* in the Software without restriction, including without limitation the rights
* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
* copies of the Software, and to permit persons to whom the Software is
* furnished to do so, subject to the following conditions:
* 
* The above copyright notice and this permission notice shall be included in
* all copies or substantial portions of the Software.
* 
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
* THE SOFTWARE.
* 
* http://rhomobile.com
*------------------------------------------------------------------------*/

#include "RhoConvertWP8.h"
#include "common/StringConverter.h"
#include <collection.h>

using namespace Windows::Foundation::Collections;

namespace rho
{
namespace common
{

::Platform::String^ convertStringCToWP8(const char* str)
{
	rho::StringW strW;
	if (str)
		rho::common::convertToStringW(str, strW);
	return ref new Platform::String(strW.c_str());
}

::Platform::String^ convertStringToWP8(const String& str)
{
	rho::StringW strW = rho::common::convertToStringW(str);
	return ref new ::Platform::String(strW.c_str());
}

::Platform::String^ convertStringWToWP8(const StringW& str)
{
	return ref new ::Platform::String(str.c_str());
}

IVectorView<Platform::String^>^ convertArrayToWP8(const Vector<String>& arr)
{
	IVector<Platform::String^>^ res = ref new Platform::Collections::Vector<Platform::String^>();
	for (Vector<String>::const_iterator i = arr.begin(); i != arr.end(); ++i)
		res->Append(convertStringToWP8(*i));
	return res->GetView();
}

IMapView<Platform::String^, Platform::String^>^ convertHashToWP8(const Hashtable<String, String>& hash)
{
	IMap<Platform::String^, Platform::String^>^ res = ref new Platform::Collections::Map<Platform::String^, Platform::String^>();
	for(Hashtable<String, String>::const_iterator i = hash.begin(); i != hash.end(); ++i)
		res->Insert(convertStringToWP8(i->first), convertStringToWP8(i->second));
	return res->GetView();
}

String convertStringAFromWP8(::Platform::String^ str)
{
	return rho::common::convertToStringA(str->Data());
}

StringW convertStringWFromWP8(::Platform::String^ str)
{
	return rho::common::convertToStringW(str->Data());
}

Vector<rho::String> convertArrayFromWP8(IVectorView<Platform::String^>^ arr)
{
	Vector<rho::String> vec;
	vec.reserve(arr->Size);

	for (IIterator<Platform::String^>^ i = arr->First(); i->HasCurrent; i->MoveNext())
		vec.addElement(convertStringAFromWP8(i->Current));
	return vec;
}

Hashtable<rho::String, rho::String> convertHashFromWP8(IMapView<Platform::String^, Platform::String^>^ hash)
{
	Hashtable<rho::String, rho::String> ht;

	for (IIterator<IKeyValuePair<Platform::String^, Platform::String^>^>^ i = hash->First(); i->HasCurrent; i->MoveNext())
		ht.emplace(convertStringAFromWP8(i->Current->Key), convertStringAFromWP8(i->Current->Value));
	return ht;
}

Vector<rho::Hashtable<rho::String, rho::String> > convertArrayOfHashesFromWP8(IVectorView<IMapView<Platform::String^, Platform::String^>^>^ arr)
{
	Vector<rho::Hashtable<rho::String, rho::String> > vec;
	vec.reserve(arr->Size);

	for (IIterator<IMapView<Platform::String^, Platform::String^>^>^ i = arr->First(); i->HasCurrent; i->MoveNext())
		vec.addElement(convertHashFromWP8(i->Current));

	return vec;
}

Hashtable<rho::String, rho::Vector<rho::String> > convertHashOfArraysFromWP8(IMapView<Platform::String^, IVectorView<Platform::String^>^>^ hash)
{
	Hashtable<rho::String, rho::Vector<rho::String> > ht;

	for (IIterator<IKeyValuePair<Platform::String^, IVectorView<Platform::String^>^>^>^ i = hash->First(); i->HasCurrent; i->MoveNext())
		ht.emplace(convertStringAFromWP8(i->Current->Key), convertArrayFromWP8(i->Current->Value));
	return ht;
}

Hashtable<rho::String, rho::Hashtable<rho::String, rho::String> > convertHashOfHashesFromWP8(IMapView<Platform::String^, IMapView<Platform::String^, Platform::String^>^>^ hash)
{
	Hashtable<rho::String, rho::Hashtable<rho::String, rho::String> > ht;

	for (IIterator<IKeyValuePair<Platform::String^, IMapView<Platform::String^, Platform::String^>^>^>^ i = hash->First(); i->HasCurrent; i->MoveNext())
		ht.emplace(convertStringAFromWP8(i->Current->Key), convertHashFromWP8(i->Current->Value));
	return ht;
}

}
}
