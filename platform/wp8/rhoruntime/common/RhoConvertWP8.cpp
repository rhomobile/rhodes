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

::Platform::String^ convertStringCToWP8(char* str)
{
    std::string s_str = std::string(str);
    std::wstring wid_str = std::wstring(s_str.begin(), s_str.end());
    const wchar_t* w_char = wid_str.c_str();
    return ref new Platform::String(w_char);
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

IVectorView<Platform::String^>^ convertArrayWP8(const Vector<String>& arr)
{
    // TODO: convert string array
    return ref new Platform::Collections::VectorView<Platform::String^>();
}

IMapView<Platform::String^, Platform::String^>^ convertHashWP8(const Hashtable<String, String>& hash)
{
    // TODO: convert string hash
    return ref new Platform::Collections::MapView<Platform::String^, Platform::String^>();
}

String convertStringAFromWP8(::Platform::String^ str)
{
    return rho::common::convertToStringA(str->Data());
}

StringW convertStringWFromWP8(::Platform::String^ str)
{
    return rho::common::convertToStringW(str->Data());
}

Vector<rho::String> convertArrayFromWP8(::Windows::Foundation::Collections::IVectorView<Platform::String^>^ arr)
{
    Vector<rho::String> vec;
    // TODO: convert string array from C#
    return vec;
}

Hashtable<rho::String, rho::String> convertHashFromWP8(::Windows::Foundation::Collections::IMapView<Platform::String^, Platform::String^>^ hash)
{
    Hashtable<rho::String, rho::String> ht;
    // TODO: convert string hash from C#
    return ht;
}

}
}
